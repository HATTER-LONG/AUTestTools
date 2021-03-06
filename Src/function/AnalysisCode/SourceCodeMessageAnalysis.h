#pragma once

#include "SourceCodeMatcherBase.h"
#include "function/SourceCodeAnalysisFunc.h"

#include <clang/Tooling/CommonOptionsParser.h>
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
    void setMatchMethodAndCallBackFunc(DeclarationMatcherPtr Matcher) { MatcherWithCallBack = std::move(Matcher); };

    bool startToAnalysisSourceCode(
        SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage) override;
    bool startToAnalysisSourceCode(SourceCodeErrorMessageList& Errormessage);

private:
    using CommonOptionsParserPtr = std::unique_ptr<clang::tooling::CommonOptionsParser>;
    CommonOptionsParserPtr getCommonOptionsParser();
    void paramInitialize(std::vector<std::string>& ParamList);

private:
    std::string SourceCodeFilePath;
    std::string CompiledDatabasePath;

    DeclarationMatcherPtr MatcherWithCallBack;
};

}   // namespace MyFunction