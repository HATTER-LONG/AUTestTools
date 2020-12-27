#pragma once
#include "function/AnalysisMessage.h"

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <spdlog/common.h>
namespace MyFunction
{
class FunctionDefLister : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    // clang-format off

    auto matcher()
    {
        using namespace clang::ast_matchers;
        return functionDecl(
                    isDefinition(),
                    unless(isExpansionInSystemHeader()),
                    forEachDescendant(callExpr().bind("callExprInfo"))
                )
                .bind("FunctionDecl");
    }
    // clang-format on

    FunctionDefLister(SourceCodeFunctionMessageMap& Ref)
            : FunctionMessageRef(Ref)
    {
    }


    void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override
    {
        clang::LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        clang::PrintingPolicy Policy(LangOpts);
        if (auto const* functionDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl"))
        {
            auto iter = FunctionMessageRef.find(functionDecl->getQualifiedNameAsString());
            if (iter == FunctionMessageRef.end())
            {
                std::string functionname;
                std::vector<std::string> functionparms;
                functionname = functionDecl->getQualifiedNameAsString();
                functionparms.push_back(functionDecl->getReturnType().getAsString());
                getParams(functionparms, functionDecl);
                iter = insertFuncToMapRef(functionDecl);
            }
            if (auto const* callexprdec = Result.Nodes.getNodeAs<clang::CallExpr>("callExprInfo"))
            {
                if (const auto* func = callexprdec->getDirectCallee())
                {
                    auto callexprIter = FunctionMessageRef.find(func->getQualifiedNameAsString());
                    if (callexprIter == FunctionMessageRef.end()) { insertFuncToMapRef(func); }
                    iter->second.addFunctionWhichCallExpr(func->getQualifiedNameAsString());
                }
            }
        }
    }

    SourceCodeFunctionMessageMap::iterator insertFuncToMapRef(const clang::FunctionDecl* Func)
    {
        std::string functionname;
        std::vector<std::string> functionparms;
        functionname = Func->getQualifiedNameAsString();
        functionparms.push_back(Func->getReturnType().getAsString());
        getParams(functionparms, Func);
        return FunctionMessageRef
            .insert(SourceCodeFunctionMessageMap::value_type(
                functionname, SourceCodeFunctionMessage(functionname, functionparms, Func->hasBody())))
            .first;
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
}   // namespace MyFunction