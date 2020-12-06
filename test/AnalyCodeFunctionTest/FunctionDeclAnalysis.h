#pragma once

#include "AnalysisMessage.h"
namespace MyFunction
{
class FunctionDeclAnalysis
{
public:
    FunctionDeclAnalysis(std::string filepath = "", std::string compiledatabase = "");

    int StartToAnalysis();

    void SetFilePathToAnalysis(std::string filepath) { sourceCodeFilePath = filepath; }
    void SetCompileDatabase(std::string compiledatabase) { compiledDatabasePath = compiledatabase; }
    const SourceCodeErrorMessageList& GetErrorMessageRef() const;
    const SourceCodeFunctionMessageMap& GetFunctionMessageRef() const;

private:
    SourceCodeErrorMessageList sourceCodeErrorMessage;
    SourceCodeFunctionMessageMap sourceCodeFunctionMessage;

    std::string sourceCodeFilePath;
    std::string compiledDatabasePath;
};

} // namespace MyFunction