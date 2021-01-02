#pragma once

#include "function/SourceCodeAnalysisFunc.h"
namespace MyFunction
{
class FunctionDeclAnalysis : public SourceCodeAnalysisFunc
{
public:
    FunctionDeclAnalysis()
            : SourceCodeAnalysisFunc()
    {
    }
    ~FunctionDeclAnalysis() override = default;
    bool startToAnalysisSourceCode(
        SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage) override;

    void setFilePathToAnalysis(const std::string& Filepath) override { SourceCodeFilePath = Filepath; }
    void setCompileDatabase(const std::string& Compiledatabase) override { CompiledDatabasePath = Compiledatabase; }
    static std::string getFactoryID() { return "level_1"; };

private:
    std::string SourceCodeFilePath;
    std::string CompiledDatabasePath;
};

}   // namespace MyFunction