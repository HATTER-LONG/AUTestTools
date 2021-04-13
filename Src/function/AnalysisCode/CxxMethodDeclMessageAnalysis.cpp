#include "CxxMethodDeclMessageAnalysis.h"

#include "Listerner/ClassMemberDelListern.hpp"
#include "SourceCodeMessageAnalysis.h"

namespace MyFunction
{
static Infra::ProductClassRegistrar<SourceCodeAnalysisFunc, CxxMethodDeclMessageAnalysis> cxxMethodDeclAnalysisMethod(
    CxxMethodDeclMessageAnalysis::getFactoryID());

CxxMethodDeclMessageAnalysis::CxxMethodDeclMessageAnalysis()
        : SourceCodeMessageAnalysis()
{
}
bool CxxMethodDeclMessageAnalysis::startToAnalysisSourceCode(
    SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage)
{
    setMatchMethodAndCallBackFunc(DeclarationMatcherPtr(std::make_unique<ClassMemberDefLister>(Functionmessage)));
    return SourceCodeMessageAnalysis::startToAnalysisSourceCode(Errormessage);
}

void CxxMethodDeclMessageAnalysis::setConfigToAnalysis(const ConfigInfo& Config)
{
    SourceCodeMessageAnalysis::setConfigToAnalysis(Config);
}
}   // namespace MyFunction