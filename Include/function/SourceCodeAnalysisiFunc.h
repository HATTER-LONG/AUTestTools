#pragma once

#include "function/AnalysisMessage.h"
#include <string>
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
    virtual bool StartToAnalysisSourceCode(SourceCodeFunctionMessageMap& functionmessage,
                                           SourceCodeErrorMessageList& errormessage) = 0;

    /**
     * @brief Set the File Path To Analysis object
     *
     * @param filepath
     */
    virtual void SetFilePathToAnalysis(std::string filepath) = 0;

    /**
     * @brief Set the file path to Compile Database object
     *
     * @param compiledatabase
     */
    virtual void SetCompileDatabase(std::string compiledatabase) = 0;
};

} // namespace MyFunction