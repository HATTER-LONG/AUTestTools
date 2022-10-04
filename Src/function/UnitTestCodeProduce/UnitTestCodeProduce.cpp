#include "FunctionTemplate.h"
#include "function/UnitTestCodeProduce.h"
#include "spdlog/spdlog.h"
using namespace MyFunction;
using namespace std;

string UnitTestCodeProduceFunc::createMockSourceCode(const MyFunction::SourceCodeFunctionMessage& info)
{
    string functionParam;
    getParamList(functionParam, info.getFunctionParam());

    string returnValue;
    getFuncReturnValue(returnValue, info.getFunctionParam()[0]);

    string functionCode = functionTemplate;
    subreplace(functionCode, "${PARAMRETURN}", info.getFunctionParam()[0]);
    subreplace(functionCode, "${FUNCTIONNAME}", info.getFunctionName());
    subreplace(functionCode, "${PARAM}", functionParam);
    subreplace(functionCode, "${RETURNVALUE}", returnValue);
    return (functionCode);
}

string UnitTestCodeProduceFunc::createMockSourceCode(const MyFunction::SourceCodeFunctionMessageMap& infoList)
{
    string listFunctionCode;
    for (const auto& a : infoList)
    {
        listFunctionCode += createMockSourceCode(a.second);
        listFunctionCode += "\n";
    }

    return (listFunctionCode);
}

void UnitTestCodeProduceFunc::getParamList(string& functionParam, const MyFunction::FunctionParamList& info)
{
    for (size_t i = 1; i < info.size(); i++)
    {
        functionParam += (info[i] + " arg" + to_string(i));
        if (i != info.size() - 1)
        {
            functionParam += ", ";
        }
    }
}

void UnitTestCodeProduceFunc::getFuncReturnValue(string& returnValue, const string& returnType)
{
    if (returnType == "void")
    {
        returnValue = "";
    }
    else if (returnType.find('&') != string::npos)
    {
        returnValue = "return " + std::string("*this;");
    }
    else
    {
        returnValue = "return " + returnType + "{};";
    }
}

string UnitTestCodeProduceFunc::createUnitTestCode(const MyFunction::SourceCodeFunctionMessage& func, const UnitTestInfo& info)
{
    string unitTestCode = catchUnitTestCaseTemplate;
    string unitTestSectionCode;
    for (const auto& a : info.m_testSection)
    {
        unitTestSectionCode += getSectionCode(func, a);
    }
    subreplace(unitTestCode, "${TESTSECTION}", unitTestSectionCode);

    string tmpTestTag;
    for (const auto& a : info.m_testTags)
    {
        tmpTestTag += "[" + a + "]";
    }
    subreplace(unitTestCode, "${TESTTAGS}", tmpTestTag);
    subreplace(unitTestCode, "${TESTNAME}", info.m_testName);
    return (unitTestCode);
}

string UnitTestCodeProduceFunc::getSectionCode(
    const MyFunction::SourceCodeFunctionMessage& func, const UnitTestSectionInfo& sectionInfo)
{
    string tmpSectionCode = catchUnitTestSectionTemplate;
    subreplace(tmpSectionCode, "${SECTIONNAME}", sectionInfo.m_sectionName);

    string tmpFuncCallName = func.getFunctionName();
    string tmpFuncParam;
    getParamList(tmpFuncParam, func.getFunctionParam());
    tmpFuncCallName += "(" + tmpFuncParam + ")";
    subreplace(tmpSectionCode, "${SECTIONCHECK}", tmpFuncCallName + sectionInfo.m_checkInfo);

    return tmpSectionCode;
}