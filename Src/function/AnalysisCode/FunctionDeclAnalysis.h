#pragma once

#include "function/SourceCodeAnalysisiFunc.h"
namespace MyFunction
{
class FunctionDeclAnalysis : public SourceCodeAnalysisFunc
{
public:
    FunctionDeclAnalysis() : SourceCodeAnalysisFunc() {}
    ~FunctionDeclAnalysis() override = default;
    bool StartToAnalysisSourceCode(SourceCodeFunctionMessageMap& functionmessage,
                                   SourceCodeErrorMessageList& errormessage) override;

    void SetFilePathToAnalysis(std::string filepath) override { sourceCodeFilePath = filepath; }
    void SetCompileDatabase(std::string compiledatabase) override { compiledDatabasePath = compiledatabase; }
    static std::string GetFactoryID() { return "level_1"; };

private:
    std::string sourceCodeFilePath;
    std::string compiledDatabasePath;
};

} // namespace MyFunction