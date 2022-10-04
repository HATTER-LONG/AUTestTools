#pragma once

#include "function/AnalysisMessage.h"
#include "function/SourceCodeAnalysisFunc.h"

#include <assert.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
namespace MyFunction
{

class DeclarationMatcherCallBackBase
        : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    DeclarationMatcherCallBackBase()
            : m_policy(clang::LangOptions {})
    {
        m_policy.adjustForCPlusPlus();
    }
    virtual clang::ast_matchers::DeclarationMatcher matcher()
    {
        assert(false);
        using namespace clang::ast_matchers;
        return functionDecl().bind("Do not use this");
    };
    virtual void getParams(
        FunctionParamList& functionparms, const clang::FunctionDecl* func)
    {
        for (unsigned int i = 0; i < func->getNumParams(); i++)
        {
            std::string paramwithname;
            const auto* param = func->getParamDecl(i);
            paramwithname += clang::QualType::getAsString(
                param->getType().split(), m_policy);
            // paramwithname += "  ";
            // paramwithname += func->getParamDecl(i)->getNameAsString();
            functionparms.push_back(paramwithname);
        }
    }
    virtual void run(
        const clang::ast_matchers::MatchFinder::MatchResult& Result)
    {
        assert(false);
    }
    virtual bool config(const ConfigInfo& config) = 0;
    clang::PrintingPolicy m_policy;
};

using DeclarationMatcherPtr = std::unique_ptr<DeclarationMatcherCallBackBase>;

class SourceCodeErrorAnalysis : public clang::DiagnosticConsumer
{
public:
    SourceCodeErrorAnalysis(SourceCodeErrorMessageList& errorMessageList)
            : m_errorMessageListRef(errorMessageList)
    {
    }
    ~SourceCodeErrorAnalysis() override = default;
    void HandleDiagnostic(clang::DiagnosticsEngine::Level diagLevel,
        const clang::Diagnostic& info) override
    {
        clang::SmallString<100> outStr;
        info.FormatDiagnostic(outStr);

        llvm::raw_svector_ostream const diagMessageStream(outStr);
        auto sourceLocal =
            clang::FullSourceLoc(info.getLocation(), info.getSourceManager())
                .getFileLoc();
        int const line = sourceLocal.getLineNumber();
        std::string filename(sourceLocal.getPresumedLoc().getFilename());

        filename += ":";
        filename += std::to_string(line);
        m_errorMessageListRef.push_back(SourceCodeErrorMessage(
            diagLevel, diagMessageStream.str().str(), filename));
    }

private:
    SourceCodeErrorMessageList& m_errorMessageListRef;
};
}   // namespace MyFunction
