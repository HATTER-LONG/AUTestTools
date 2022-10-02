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

    void setFilePathToAnalysis(const std::string& Filepath) override
    {
        m_sourceCodeFilePath = Filepath;
    }
    void setCompileDatabase(const std::string& Compiledatabase) override
    {
        m_compiledDatabasePath = Compiledatabase;
    }
    void setConfigToAnalysis(const ConfigInfo& Config) override
    {
        m_jsonConfigToMatcher = Config;
    };

protected:
    void setMatchMethodAndCallBackFunc(DeclarationMatcherPtr Matcher)
    {
        m_matcherWithCallBack = std::move(Matcher);
    };

    bool startToAnalysisSourceCode(
        SourceCodeFunctionMessageMap& Functionmessage,
        SourceCodeErrorMessageList& Errormessage) override;
    bool startToAnalysisSourceCode(SourceCodeErrorMessageList& Errormessage);

private:
    using CommonOptionsParserExpected =
        llvm::Expected<clang::tooling::CommonOptionsParser>;
    CommonOptionsParserExpected getCommonOptionsParser();
    void paramInitialize(std::vector<std::string>& ParamList);

private:
    std::string m_sourceCodeFilePath;
    std::string m_compiledDatabasePath;
    ConfigInfo m_jsonConfigToMatcher;
    DeclarationMatcherPtr m_matcherWithCallBack;
};

}   // namespace MyFunction
