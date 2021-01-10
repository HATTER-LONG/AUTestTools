#include "FuncDeclWithCallexprMessageAnalysis.h"
#include "FunctionDefLister.hpp"
#include "SourceCodeMessageAnlysis.h"
#include "function/AnalysisMessage.h"

namespace MyFunction
{

static Infra::ProductClassRegistrar<MyFunction::SourceCodeAnalysisFunc, MyFunction::FuncDeclWithCallExprMessage>
    funcdeclAnalysisMethod(MyFunction::FuncDeclWithCallExprMessage::getFactoryID());

FuncDeclWithCallExprMessage::FuncDeclWithCallExprMessage()
        : SourceCodeMessageAnalysis()
{
}

bool FuncDeclWithCallExprMessage::startToAnalysisSourceCode(
    SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage)
{
    setMatchMethodAndCallBackFunc(
        std::unique_ptr<DeclarationMatcherCallBackBase>(std::make_unique<FunctionDefLister>(Functionmessage)));
    return SourceCodeMessageAnalysis::startToAnalysisSourceCode(Functionmessage, Errormessage);
}
}   // namespace MyFunction