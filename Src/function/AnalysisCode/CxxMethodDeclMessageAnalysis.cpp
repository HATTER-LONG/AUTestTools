#include "CxxMethodDeclMessageAnalysis.h"
#include "Listerner/ClassMemberDelListern.hpp"
#include "SourceCodeMessageAnalysis.h"

namespace MyFunction
{
static Infra::ProductClassRegistrar<SourceCodeAnalysisFunc,
    CxxMethodDeclMessageAnalysis>
    cxxMethodDeclAnalysisMethod(CxxMethodDeclMessageAnalysis::getFactoryID());

bool CxxMethodDeclMessageAnalysis::startToAnalysisSourceCode(
    SourceCodeFunctionMessageMap& functionmessage,
    SourceCodeErrorMessageList& errormessage)
{
    setMatchMethodAndCallBackFunc(DeclarationMatcherPtr(
        std::make_unique<ClassMemberDefLister>(functionmessage)));
    return SourceCodeMessageAnalysis::startToAnalysisSourceCode(errormessage);
}

void CxxMethodDeclMessageAnalysis::setConfigToAnalysis(const ConfigInfo& config)
{
    SourceCodeMessageAnalysis::setConfigToAnalysis(config);
}
}   // namespace MyFunction
