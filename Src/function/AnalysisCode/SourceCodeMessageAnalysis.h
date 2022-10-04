#pragma once

#include "Listerner/SourceCodeMatcherBase.h"
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

    void setFilePathToAnalysis(const std::string& filepath) override
    {
        m_sourceCodeFilePath = filepath;
    }
    void setCompileDatabase(const std::string& compiledatabase) override
    {
        m_compiledDatabasePath = compiledatabase;
    }
    void setConfigToAnalysis(const ConfigInfo& config) override
    {
        m_jsonConfigToMatcher = config;
    };

protected:
    void setMatchMethodAndCallBackFunc(DeclarationMatcherPtr matcher)
    {
        m_matcherWithCallBack = std::move(matcher);
    };

    bool startToAnalysisSourceCode(
        SourceCodeFunctionMessageMap& functionmessage,
        SourceCodeErrorMessageList& errormessage) override;
    bool startToAnalysisSourceCode(SourceCodeErrorMessageList& errormessage);

private:
    using CommonOptionsParserExpected =
        llvm::Expected<clang::tooling::CommonOptionsParser>;
    CommonOptionsParserExpected getCommonOptionsParser();
    void paramInitialize(std::vector<std::string>& paramList);

private:
    std::string m_sourceCodeFilePath;
    std::string m_compiledDatabasePath;
    ConfigInfo m_jsonConfigToMatcher;
    DeclarationMatcherPtr m_matcherWithCallBack;
};

}   // namespace MyFunction
