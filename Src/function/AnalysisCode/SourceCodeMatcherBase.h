#pragma once

#include "function/AnalysisMessage.h"

#include <assert.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
namespace MyFunction
{

class DeclarationMatcherCallBackBase : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    virtual clang::ast_matchers::DeclarationMatcher matcher()
    {
        assert(false);
        using namespace clang::ast_matchers;
        return functionDecl().bind("Do not use this");
    };
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) { assert(false); }
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