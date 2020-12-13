#pragma once

#include "function/SourceCodeAnalysisFunc.h"

namespace MyFunction
{
class SourceCodeMessageAnalysis : public SourceCodeAnalysisFunc
{
public:
    SourceCodeMessageAnalysis() : SourceCodeAnalysisFunc() {}
    ~SourceCodeMessageAnalysis() override = default;
    bool StartToAnalysisSourceCode(SourceCodeFunctionMessageMap& functionmessage,
                                   SourceCodeErrorMessageList& errormessage) override;

    void SetFilePathToAnalysis(std::string filepath) override { sourceCodeFilePath = filepath; }
    void SetCompileDatabase(std::string compiledatabase) override { compiledDatabasePath = compiledatabase; }
    static std::string GetFactoryID() { return "level_2"; };

private:
    std::string sourceCodeFilePath;
    std::string compiledDatabasePath;
};

} // namespace MyFunction