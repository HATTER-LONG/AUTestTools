#pragma once

#include "AnalysisMessageHelper.h"
#include "Infra/FactoryTemplate.h"
#include "nlohmann/json.hpp"

#include <string>

using ConfigInfo = nlohmann::json;

namespace MyFunction
{
/**
 * @brief 源码分析工具虚接口类  Source analysis tool virtual interface class
 *
 */
class SourceCodeAnalysisFunc
{
public:
    SourceCodeAnalysisFunc() = default;
    virtual ~SourceCodeAnalysisFunc() = default;

    /**
     * @brief Start to analysis source code
     *
     * @param[out] functionmessage
     * @param[out] errormessage
     * @return bool : success or not
     */
    virtual bool startToAnalysisSourceCode(
        SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage) = 0;

    /**
     * @brief Set the File Path To Analysis object
     *
     * @param filepath
     */
    virtual void setFilePathToAnalysis(const std::string& Filepath) = 0;

    /**
     * @brief Set the file path to Compile Database object
     *
     * @param compiledatabase
     */
    virtual void setCompileDatabase(const std::string& Compiledatabase) = 0;

    /**
     * @brief Set config to Compile Analysis object
     *
     * @param Json 具体内容参照组件接口定义
     */
    virtual void setConfigToAnalysis(const ConfigInfo& Config) = 0;
};

using g_SourceCodeAnalysisFactory = Infra::ProductClassFactory<MyFunction::SourceCodeAnalysisFunc>;
}   // namespace MyFunction