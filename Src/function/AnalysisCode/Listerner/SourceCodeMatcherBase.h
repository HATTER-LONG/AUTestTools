#pragma once

#include "function/AnalysisMessage.h"
#include "function/SourceCodeAnalysisFunc.h"

#include <assert.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
namespace MyFunction
{

class DeclarationMatcherCallBackBase : public clang::ast_matchers::MatchFinder::MatchCallback
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
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) { assert(false); }
    virtual void config(const ConfigInfo& Config) = 0;
    clang::PrintingPolicy m_policy;
};

using DeclarationMatcherPtr = std::unique_ptr<DeclarationMatcherCallBackBase>;

class SourceCodeErrorAnalysis : public clang::DiagnosticConsumer
{
public:
    SourceCodeErrorAnalysis(SourceCodeErrorMessageList& ErrorMessageList)
            : m_errorMessageListRef(ErrorMessageList)
    {
    }
    ~SourceCodeErrorAnalysis() override = default;
    void HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel, const clang::Diagnostic& Info) override
    {
        clang::SmallString<100> outStr;
        Info.FormatDiagnostic(outStr);

        llvm::raw_svector_ostream diagMessageStream(outStr);
        auto sourceLocal = clang::FullSourceLoc(Info.getLocation(), Info.getSourceManager()).getFileLoc();
        int line = sourceLocal.getLineNumber();
        std::string filename(sourceLocal.getPresumedLoc().getFilename());

        filename += ":";
        filename += std::to_string(line);
        m_errorMessageListRef.push_back(SourceCodeErrorMessage(DiagLevel, diagMessageStream.str().str(), filename));
    }

private:
    SourceCodeErrorMessageList& m_errorMessageListRef;
};
}   // namespace MyFunction