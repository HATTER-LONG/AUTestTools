#include "function/AnalysisMessage.h"

#include <clang-c/Index.h>
#include <clang/AST/DeclObjC.h>
#include <clang/AST/Type.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/CommentOptions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/raw_ostream.h>
#include <string>

namespace MyFunction
{

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");
class MatcherNodeFuncCall : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    MatcherNodeFuncCall(MyFunction::SourceCodeFunctionMessageMap& Functionmessage)
            : FunctionMessageRef(Functionmessage)
    {
    }
    void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override
    {
        clang::LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        clang::PrintingPolicy Policy(LangOpts);
        if (auto const* functionDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDeclWithCall"))
        {
            auto iter = FunctionMessageRef.find(functionDecl->getQualifiedNameAsString());
            if (iter == FunctionMessageRef.end())
            {
                std::string functionname;
                std::vector<std::string> functionparms;
                functionname = functionDecl->getQualifiedNameAsString();
                functionparms.push_back(functionDecl->getReturnType().getAsString());
                getParams(functionparms, functionDecl);

                iter = FunctionMessageRef
                           .insert(SourceCodeFunctionMessageMap::value_type(
                               functionname, SourceCodeFunctionMessage(functionname, functionparms)))
                           .first;
            }
            if (auto const* callexprdec = Result.Nodes.getNodeAs<clang::CallExpr>("callExprFunction"))
            {
                const auto* func = callexprdec->getDirectCallee();
                auto callexprIter = FunctionMessageRef.find(func->getQualifiedNameAsString());
                if (callexprIter == FunctionMessageRef.end())
                {
                    std::string functionname;
                    std::vector<std::string> functionparms;
                    functionname = func->getQualifiedNameAsString();
                    functionparms.push_back(func->getReturnType().getAsString());
                    getParams(functionparms, func);
                    FunctionMessageRef.insert(SourceCodeFunctionMessageMap::value_type(
                        functionname, SourceCodeFunctionMessage(functionname, functionparms)));
                }
                iter->second.addFunctionWhichCallExpr(func->getQualifiedNameAsString());
            }
        }
    }

    void getParams(FunctionParamList& Functionparms, const clang::FunctionDecl* Func)
    {
        clang::LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        clang::PrintingPolicy Policy(LangOpts);
        for (unsigned int i = 0; i < Func->getNumParams(); i++)
        {
            std::string paramwithname;
            const auto* param = Func->getParamDecl(i);
            paramwithname += clang::QualType::getAsString(param->getType().split(), Policy);
            // paramwithname += "  ";
            // paramwithname += func->getParamDecl(i)->getNameAsString();
            Functionparms.push_back(paramwithname);
        }
    }

private:
    SourceCodeFunctionMessageMap& FunctionMessageRef;
};

class SourceCodeErrorAnalysis : public clang::DiagnosticConsumer
{
public:
    SourceCodeErrorAnalysis(SourceCodeErrorMessageList& ErrorMessageList)
            : ErrorMessageListRef(ErrorMessageList)
    {
    }
    ~SourceCodeErrorAnalysis() override = default;
    void HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel, const clang::Diagnostic& Info) override
    {
        clang::SmallString<100> OutStr;
        Info.FormatDiagnostic(OutStr);

        llvm::raw_svector_ostream DiagMessageStream(OutStr);
        auto aa = clang::FullSourceLoc(Info.getLocation(), Info.getSourceManager()).getFileLoc();
        int Line = aa.getLineNumber();
        std::string filename(aa.getPresumedLoc().getFilename());

        filename += ":";
        filename += std::to_string(Line);
        ErrorMessageListRef.push_back(SourceCodeErrorMessage(DiagLevel, DiagMessageStream.str().str(), filename));
    }

private:
    SourceCodeErrorMessageList& ErrorMessageListRef;
};

class FuncDeclAnalysisFrontendAction : public clang::ASTFrontendAction
{
public:
    FuncDeclAnalysisFrontendAction(SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage)
    {
        ErrorAnalysis = new SourceCodeErrorAnalysis(Errormessage);
        NodeFuncCall = new MatcherNodeFuncCall(Functionmessage);
    }
    ~FuncDeclAnalysisFrontendAction() override
    {
        delete NodeFuncCall;
        NodeFuncCall = nullptr;
    }
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef /*file*/) override
    {
        using namespace clang::ast_matchers;
        auto FuncDeclMatcher = functionDecl(isExpansionInMainFile(),
            anyOf(forEachDescendant(callExpr().bind("callExprFunction")), unless(forEachDescendant(callExpr()))))
                                   .bind("FunctionDeclWithCall");
        Finder.addMatcher(FuncDeclMatcher, NodeFuncCall);
        // setClient 第二个参数默认为 true 迁移 DiagnosticConsumer 所有权
        CI.getDiagnostics().setClient(ErrorAnalysis);
        return Finder.newASTConsumer();
    }

private:
    MatcherNodeFuncCall* NodeFuncCall;
    clang::ast_matchers::MatchFinder Finder;
    SourceCodeErrorAnalysis* ErrorAnalysis;
};

}   // namespace MyFunction