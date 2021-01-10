#pragma once

#include "SourceCodeMessageAnlysis.h"

namespace MyFunction
{
class FuncDeclWithCallExprMessage : public SourceCodeMessageAnalysis
{
public:
    FuncDeclWithCallExprMessage();
    ~FuncDeclWithCallExprMessage() = default;
    bool startToAnalysisSourceCode(
        SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage) override;
    static std::string getFactoryID() { return "FuncDeclWithCallExpr"; };
};
}   // namespace MyFunction