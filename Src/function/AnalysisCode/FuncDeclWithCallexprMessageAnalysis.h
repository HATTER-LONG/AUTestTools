#pragma once

#include "SourceCodeMessageAnalysis.h"

namespace MyFunction
{
class FuncDeclWithCallExprMessage : public SourceCodeMessageAnalysis
{
public:
    FuncDeclWithCallExprMessage() = default;
    ~FuncDeclWithCallExprMessage() = default;
    bool startToAnalysisSourceCode(
        SourceCodeFunctionMessageMap& functionmessage,
        SourceCodeErrorMessageList& errormessage) override;
    static std::string getFactoryID() { return "FuncDeclWithCallExpr_v1"; };
};
}   // namespace MyFunction
