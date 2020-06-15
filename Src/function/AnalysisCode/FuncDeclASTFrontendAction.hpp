#include "function/FunctionDeclAnalysis.h"
#include "spdlog/spdlog.h"

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

namespace MFunction
{

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");
class MatcherNodeFuncCall : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    MatcherNodeFuncCall(MFunction::SourceCodeFunctionMessageMap& functionmessage) : functionMessageRef(functionmessage)
    {
    }
    void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override
    {
        std::string callexprname;
        std::string callexprparms;
        clang::LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        clang::PrintingPolicy Policy(LangOpts);
        if (auto const* functionDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctiondFeclWithCall"))
        {
            auto iter = functionMessageRef.find(functionDecl->getQualifiedNameAsString());
            if (iter == functionMessageRef.end())
            {
                std::string functionname;
                std::vector<std::string> functionparms;
                functionname = functionDecl->getQualifiedNameAsString();
                functionparms.push_back(functionDecl->getReturnType().getAsString());
                for (unsigned int i = 0; i < functionDecl->getNumParams(); i++)
                {
                    std::string paramwithname;
                    auto param = functionDecl->getParamDecl(i);
                    paramwithname += clang::QualType::getAsString(param->getType().split(), Policy);
                    paramwithname += "  ";
                    paramwithname += functionDecl->getParamDecl(i)->getNameAsString();
                    functionparms.push_back(paramwithname);
                }

                iter = functionMessageRef
                           .insert(SourceCodeFunctionMessageMap::value_type(
                               functionname, SourceCodeFunctionMessage(functionname, functionparms)))
                           .first;
            }
            if (auto const* callexprtdec = Result.Nodes.getNodeAs<clang::CallExpr>("callExprFunction"))
            {
                auto func = callexprtdec->getDirectCallee();
                auto callexprIter = functionMessageRef.find(func->getQualifiedNameAsString());
                if (callexprIter == functionMessageRef.end())
                {
                    std::string functionname;
                    std::vector<std::string> functionparms;
                    functionname = func->getQualifiedNameAsString();
                    functionparms.push_back(func->getReturnType().getAsString());
                    for (unsigned int i = 0; i < func->getNumParams(); i++)
                    {
                        std::string paramwithname;
                        auto param = func->getParamDecl(i);
                        paramwithname += clang::QualType::getAsString(param->getType().split(), Policy);
                        paramwithname += "  ";
                        paramwithname += func->getParamDecl(i)->getNameAsString();
                        functionparms.push_back(paramwithname);
                    }
                    functionMessageRef.insert(SourceCodeFunctionMessageMap::value_type(
                        functionname, SourceCodeFunctionMessage(functionname, functionparms)));
                }
                iter->second.AddFunctionWhichCallExpr(func->getQualifiedNameAsString());
            }
        }
    }

private:
    SourceCodeFunctionMessageMap& functionMessageRef;
};

class SourceCodeErrorAnalysis : public clang::DiagnosticConsumer
{
public:
    SourceCodeErrorAnalysis(SourceCodeErrorMessageList& errorMessageList) : errorMessageListRef(errorMessageList) {}
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
        errorMessageListRef.push_back(SourceCodeErrorMessage(DiagLevel, DiagMessageStream.str().str(), filename));
    }

private:
    SourceCodeErrorMessageList& errorMessageListRef;
};

class FuncDeclAnalysisFrontendAction : public clang::ASTFrontendAction
{
public:
    FuncDeclAnalysisFrontendAction(SourceCodeErrorMessageList& errormessage,
                                   SourceCodeFunctionMessageMap& functionmessage)
    {
        errorAnalysis = new SourceCodeErrorAnalysis(errormessage);
        nodeFuncCall = new MatcherNodeFuncCall(functionmessage);
    }
    ~FuncDeclAnalysisFrontendAction() override
    {
        delete nodeFuncCall;
        nodeFuncCall = nullptr;
    }
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI,
                                                          clang::StringRef /*file*/) override
    {
        using namespace clang::ast_matchers;
        auto FuncDeclMatcher =
            functionDecl(isExpansionInMainFile(), anyOf(forEachDescendant(callExpr().bind("callExprFunction")),
                                                        unless(forEachDescendant(callExpr()))))
                .bind("FunctiondFeclWithCall");
        finder.addMatcher(FuncDeclMatcher, nodeFuncCall);
        // setClient 第二个参数默认为 true 迁移 DiagnosticConsumer 所有权
        CI.getDiagnostics().setClient(errorAnalysis);
        return finder.newASTConsumer();
    }

private:
    MatcherNodeFuncCall* nodeFuncCall;
    clang::ast_matchers::MatchFinder finder;
    SourceCodeErrorAnalysis* errorAnalysis;
};

} // namespace MFunction