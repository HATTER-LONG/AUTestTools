#include "FuncDeclWithCallexprMessageAnalysis.h"
#include "Listerner/FunctionDefListern.hpp"

namespace MyFunction
{
static Infra::ProductClassRegistrar<SourceCodeAnalysisFunc, FuncDeclWithCallExprMessage> funcdeclAnalysisMethod(
    FuncDeclWithCallExprMessage::getFactoryID());


FuncDeclWithCallExprMessage::FuncDeclWithCallExprMessage()
        : SourceCodeMessageAnalysis()
{
}

bool FuncDeclWithCallExprMessage::startToAnalysisSourceCode(
    SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage)
{
    setMatchMethodAndCallBackFunc(DeclarationMatcherPtr(std::make_unique<FunctionDefLister>(Functionmessage)));
    return SourceCodeMessageAnalysis::startToAnalysisSourceCode(Errormessage);
}
}   // namespace MyFunction