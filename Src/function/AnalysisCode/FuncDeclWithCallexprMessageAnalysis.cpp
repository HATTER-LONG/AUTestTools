#include "FuncDeclWithCallexprMessageAnalysis.h"
#include "Listerner/FunctionDefListern.hpp"

namespace MyFunction
{
static Infra::ProductClassRegistrar<SourceCodeAnalysisFunc,
    FuncDeclWithCallExprMessage>
    funcdeclAnalysisMethod(FuncDeclWithCallExprMessage::getFactoryID());

bool FuncDeclWithCallExprMessage::startToAnalysisSourceCode(
    SourceCodeFunctionMessageMap& functionmessage,
    SourceCodeErrorMessageList& errormessage)
{
    setMatchMethodAndCallBackFunc(DeclarationMatcherPtr(
        std::make_unique<FunctionDefLister>(functionmessage)));
    return SourceCodeMessageAnalysis::startToAnalysisSourceCode(errormessage);
}

}   // namespace MyFunction
