#pragma once

#include "SourceCodeMessageAnalysis.h"
#include "function/SourceCodeAnalysisFunc.h"

namespace MyFunction
{
class CxxMethodDeclMessageAnalysis : public SourceCodeMessageAnalysis
{
public:
    CxxMethodDeclMessageAnalysis() = default;
    ~CxxMethodDeclMessageAnalysis() = default;

    bool startToAnalysisSourceCode(
        SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage) override;

    /**
     * @brief 类函数信息配置
     *
     * @param 目标类名称 ConfigInfo["ClassName"] = "TargetClassName"
     */
    void setConfigToAnalysis(const ConfigInfo& Config) override;

    static std::string getFactoryID() { return "CxxMethodDeclWithName_v1"; };
};
}   // namespace MyFunction