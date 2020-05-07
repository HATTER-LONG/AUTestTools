#include "Function/AnalyseCodeTree.h"
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

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace spdlog;
static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");
class Func_Call : public MatchFinder::MatchCallback
{
public:
    void run(const MatchFinder::MatchResult& Result) override
    {
        std::string classname;
        std::string functionname;
        std::string functionparms;
        std::string callexprname;
        std::string callexprparms;

        clang::LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        clang::PrintingPolicy Policy(LangOpts);
        if (auto const* functionDecl = Result.Nodes.getNodeAs<FunctionDecl>("FunctiondFeclWithCall"))
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
                    functionparms += QualType::getAsString(param->getType().split(), Policy);
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
        if (auto const* classdecl = Result.Nodes.getNodeAs<CXXRecordDecl>("methodclass"))
        {
            classname = classdecl->getNameAsString();
        }

        if (auto const* callexprtdec = Result.Nodes.getNodeAs<CallExpr>("callExprFunction"))
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
                        callexprparms += QualType::getAsString(param->getType().split(), Policy);
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

    // void onEndOfTranslationUnit() {}
};

class BlankDiagConsumer : public clang::DiagnosticConsumer
{
public:
    BlankDiagConsumer() = default;
    ~BlankDiagConsumer() override = default;
    void HandleDiagnostic(DiagnosticsEngine::Level DiagLevel, const Diagnostic& Info) override
    {
        SmallString<100> OutStr;
        Info.FormatDiagnostic(OutStr);

        llvm::raw_svector_ostream DiagMessageStream(OutStr);
        auto aa = FullSourceLoc(Info.getLocation(), Info.getSourceManager()).getFileLoc();
        int Line = aa.getLineNumber();
        spdlog::info("{} DiagLevel = {} Message = {} at Line = {}", __FUNCTION__, DiagLevel,
                     DiagMessageStream.str().str().c_str(), Line);
    }
};

class TestTextDiagConsumer : public clang::TextDiagnosticPrinter
{
public:
    TestTextDiagConsumer(raw_ostream& os, DiagnosticOptions* diags, bool OwnsOutputStream = false)
            : TextDiagnosticPrinter(os, diags, OwnsOutputStream)
    {
    }
    void HandleDiagnostic(DiagnosticsEngine::Level DiagLevel, const Diagnostic& Info) override
    {
        TextDiagnosticPrinter::HandleDiagnostic(DiagLevel, Info);
        spdlog::info("{} DiagLevel = {} Ok", __FUNCTION__, DiagLevel);
    }
};
class MyFrontendAction : public ASTFrontendAction
{
public:
    MyFrontendAction() = default;
    void EndSourceFileAction() override
    {
        auto& DE = getCompilerInstance().getDiagnostics();
        spdlog::info("{} Warning\n", DE.getNumWarnings());
    }
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI, StringRef file) override
    {
        llvm::errs() << "** Creating AST consumer for: " << file << "\n";
        auto FuncDeclMatcher =
            functionDecl(isExpansionInMainFile(),
                         anyOf(hasAncestor(cxxRecordDecl().bind("methodclass")), unless(hasAncestor(cxxRecordDecl()))),
                         anyOf(forEachDescendant(callExpr().bind("callExprFunction")),
                               unless(forEachDescendant(callExpr().bind("callExprFunction")))))
                .bind("FunctiondFeclWithCall");
        Finder.addMatcher(FuncDeclMatcher, &FuncCall);
        CI.getDiagnostics().setClient(&ctr);
        return Finder.newASTConsumer();
    }

private:
    Func_Call FuncCall;
    MatchFinder Finder;
    BlankDiagConsumer ctr;
};


int FunctionToAnalyseCodeTree(int argc, const char** argv)
{
    auto FuncDeclMatcher =
        functionDecl(isExpansionInMainFile(),
                     anyOf(hasAncestor(cxxRecordDecl().bind("methodclass")), unless(hasAncestor(cxxRecordDecl()))),
                     anyOf(forEachDescendant(callExpr().bind("callExprFunction")),
                           unless(forEachDescendant(callExpr().bind("callExprFunction")))))
            .bind("FunctiondFeclWithCall");
    CommonOptionsParser OptionsParser(argc, argv, ToolingSampleCategory);
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
    Func_Call FuncCall;
    MatchFinder Finder;
    Finder.addMatcher(FuncDeclMatcher, &FuncCall);
    return Tool.run(newFrontendActionFactory(&Finder).get());
}

int FunctionToAnalyseCodeError(int argc, const char** argv)
{
    CommonOptionsParser op(argc, argv, ToolingSampleCategory);
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    // ClangTool::run accepts a FrontendActionFactory, which is then used to
    // create new objects implementing the FrontendAction interface. Here we use
    // the helper newFrontendActionFactory to create a default factory that will
    // return a new MyFrontendAction object every time.
    // To further customize this, we could create our own factory class.
    return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}