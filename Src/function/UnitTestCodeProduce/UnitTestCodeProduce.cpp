#include "FunctionTemplate.h"
#include "function/UnitTestCodeProduce.h"
#include "spdlog/spdlog.h"
using namespace MyFunction;
using namespace std;

string UnitTestCodeProduceFunc::createMockSourceCode(const MyFunction::SourceCodeFunctionMessage& Info)
{
    string functionParam;
    getParamList(functionParam, Info.getFunctionParam());

    string returnValue;
    getFuncReturnValue(returnValue, Info.getFunctionParam()[0]);

    string functionCode = FunctionTemplate;
    subreplace(functionCode, "${PARAMRETURN}", Info.getFunctionParam()[0]);
    subreplace(functionCode, "${FUNCTIONNAME}", Info.getFunctionName());
    subreplace(functionCode, "${PARAM}", functionParam);
    subreplace(functionCode, "${RETURNVALUE}", returnValue);
    return (functionCode);
}

string UnitTestCodeProduceFunc::createMockSourceCode(const MyFunction::SourceCodeFunctionMessageMap& InfoList)
{
    string listFunctionCode;
    for (const auto& a : InfoList)
    {
        listFunctionCode += createMockSourceCode(a.second);
        listFunctionCode += "\n";
    }

    return (listFunctionCode);
}

void UnitTestCodeProduceFunc::getParamList(string& FunctionParam, const MyFunction::FunctionParamList& Info)
{
    for (size_t i = 1; i < Info.size(); i++)
    {
        FunctionParam += (Info[i] + " arg" + to_string(i));
        if (i != Info.size() - 1) { FunctionParam += ", "; }
    }
}

void UnitTestCodeProduceFunc::getFuncReturnValue(string& ReturnValue, const string& ReturnType)
{
    if (ReturnType == "void") { ReturnValue = ""; }
    else
    {
        ReturnValue = "return " + ReturnType + "();";
    }
}

string UnitTestCodeProduceFunc::createUnitTestCode(const MyFunction::SourceCodeFunctionMessage& Func, const UnitTestInfo& Info)
{
    string unitTestCode = CatchUnitTestCaseTemplate;
    string unitTestSectionCode;
    for (const auto& a : Info.TestSection)
    {
        unitTestSectionCode += getSectionCode(Func, a);
    }
    subreplace(unitTestCode, "${TESTSECTION}", unitTestSectionCode);

    string tmpTestTag;
    for (const auto& a : Info.TestTags)
    {
        tmpTestTag += "[" + a + "]";
    }
    subreplace(unitTestCode, "${TESTTAGS}", tmpTestTag);
    subreplace(unitTestCode, "${TESTNAME}", Info.TestName);
    return (unitTestCode);
}

string UnitTestCodeProduceFunc::getSectionCode(
    const MyFunction::SourceCodeFunctionMessage& Func, const UnitTestSectionInfo& SectionInfo)
{
    string tmpSectionCode = CatchUnitTestSectionTemplate;
    subreplace(tmpSectionCode, "${SECTIONNAME}", SectionInfo.SectionName);

    string tmpFuncCallName = Func.getFunctionName();
    string tmpFuncParam;
    getParamList(tmpFuncParam, Func.getFunctionParam());
    tmpFuncCallName += "(" + tmpFuncParam + ")";
    subreplace(tmpSectionCode, "${SECTIONCHECK}", tmpFuncCallName + SectionInfo.CheckInfo);

    return tmpSectionCode;
}