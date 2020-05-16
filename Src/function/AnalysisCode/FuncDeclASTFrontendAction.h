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


static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");
class MatcherNodeFuncCall : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override
    {
        std::string classname;
        std::string functionname;
        std::string functionparms;
        std::string callexprname;
        std::string callexprparms;

        clang::LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        clang::PrintingPolicy Policy(LangOpts);
        if (auto const* functionDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctiondFeclWithCall"))
        {
            if (!functionDecl->hasBody())
            {
                return;
            }
            functionname = functionDecl->getNameAsString();
            functionname += " | ";
            functionname += functionDecl->getQualifiedNameAsString();
            functionparms = "Return: ";
            functionparms += functionDecl->getReturnType().getAsString();
            if (functionDecl->getNumParams() > 0)
            {
                functionparms += " | Param: ";
                for (unsigned int i = 0; i < functionDecl->getNumParams(); i++)
                {
                    auto param = functionDecl->getParamDecl(i);
                    functionparms += clang::QualType::getAsString(param->getType().split(), Policy);
                    functionparms += "  ";
                    functionparms += functionDecl->getParamDecl(i)->getNameAsString();
                    functionparms += " | ";
                }
            }
            else
            {
                functionparms += " | Param: NULL";
            }
        }
        if (auto const* classdecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("methodclass"))
        {
            classname = classdecl->getNameAsString();
        }

        if (auto const* callexprtdec = Result.Nodes.getNodeAs<clang::CallExpr>("callExprFunction"))
        {
            auto func = callexprtdec->getDirectCallee();
            callexprname = func->getNameInfo().getAsString();
            if (!callexprname.empty())
            {
                callexprname += " | ";
                callexprname += func->getQualifiedNameAsString();
                callexprparms = "Return: ";
                callexprparms += func->getReturnType().getAsString();
                if (func->getNumParams() > 0)
                {
                    callexprparms += " | Param: ";
                    for (unsigned int i = 0; i < func->getNumParams(); i++)
                    {
                        auto param = func->getParamDecl(i);
                        callexprparms += clang::QualType::getAsString(param->getType().split(), Policy);
                        callexprparms += "  ";
                        callexprparms += func->getParamDecl(i)->getNameAsString();
                        callexprparms += " | ";
                    }
                }
                else
                {
                    callexprparms += " | Param: NULL";
                }
            }
        }
        else
        {
            callexprparms = "NULL";
            callexprname = "CALL NULL";
        }

        spdlog::info("analysis result classname[{}] function[{} type:{}]  callexpr[{} type:{}]\n", classname.c_str(),
                     functionname.c_str(), functionparms.c_str(), callexprname.c_str(), callexprparms.c_str());
    }
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
    FuncDeclAnalysisFrontendAction(SourceCodeErrorMessageList& errormessage)
    {
        errorAnalysis = new SourceCodeErrorAnalysis(errormessage);
    }
    ~FuncDeclAnalysisFrontendAction() override = default;
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file) override
    {
        llvm::errs() << "** Creating AST consumer for: " << file << "\n";
        using namespace clang::ast_matchers;
        auto FuncDeclMatcher =
            functionDecl(isExpansionInMainFile(),
                         anyOf(hasAncestor(cxxRecordDecl().bind("methodclass")), unless(hasAncestor(cxxRecordDecl()))),
                         anyOf(forEachDescendant(callExpr().bind("callExprFunction")),
                               unless(forEachDescendant(callExpr().bind("callExprFunction")))))
                .bind("FunctiondFeclWithCall");
        finder.addMatcher(FuncDeclMatcher, &nodeFuncCall);
        // setClient 第二个参数默认为 true 迁移 DiagnosticConsumer 所有权
        CI.getDiagnostics().setClient(errorAnalysis);
        return finder.newASTConsumer();
    }

private:
    MatcherNodeFuncCall nodeFuncCall;
    clang::ast_matchers::MatchFinder finder;
    SourceCodeErrorAnalysis* errorAnalysis;
};