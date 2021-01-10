#pragma once

#include "SourceCodeMatcherBase.h"
#include "function/SourceCodeAnalysisFunc.h"
// TODO: 整理注释相关，测试构造是否可以放入 private 中禁止直接构建
namespace MyFunction
{
class SourceCodeMessageAnalysis : public SourceCodeAnalysisFunc
{
public:
    SourceCodeMessageAnalysis()
            : SourceCodeAnalysisFunc()
    {
    }
    ~SourceCodeMessageAnalysis() override = default;

    void setFilePathToAnalysis(const std::string& Filepath) override { SourceCodeFilePath = Filepath; }
    void setCompileDatabase(const std::string& Compiledatabase) override { CompiledDatabasePath = Compiledatabase; }

protected:
    void setMatchMethodAndCallBackFunc(std::unique_ptr<DeclarationMatcherCallBackBase> Matcher)
    {
        MatcherWithCallBack = std::move(Matcher);
    };
    bool startToAnalysisSourceCode(
        SourceCodeFunctionMessageMap& /*Functionmessage*/, SourceCodeErrorMessageList& Errormessage) override;

private:
    std::string SourceCodeFilePath;
    std::string CompiledDatabasePath;

    std::unique_ptr<DeclarationMatcherCallBackBase> MatcherWithCallBack;
};

}   // namespace MyFunction