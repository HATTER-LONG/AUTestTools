#include "FunctionTemplate.h"
#include "function/UnitTestCodeProduce.h"
#include "spdlog/spdlog.h"
using namespace MyFunction;
using namespace std;

string UnitTestCodeProduceFunc::createMockSourceCode(const MFunction::SourceCodeFunctionMessage& info)
{
    string functionParam;
    getParamList(functionParam, info.GetFunctionParam());

    string returnValue;
    getFuncReturnValue(returnValue, info.GetFunctionParam()[0]);

    string functionCode = FunctionTemplate;
    subreplace(functionCode, "${PARAMRETURN}", info.GetFunctionParam()[0]);
    subreplace(functionCode, "${FUNCTIONNAME}", info.GetFunctionName());
    subreplace(functionCode, "${PARAM}", functionParam);
    subreplace(functionCode, "${RETURNVALUE}", returnValue);
    return (functionCode);
}

string UnitTestCodeProduceFunc::createMockSourceCode(const MFunction::SourceCodeFunctionMessageMap& infoList)
{
    string listFunctionCode;
    for (const auto& a : infoList)
    {
        listFunctionCode += createMockSourceCode(a.second);
        listFunctionCode += "\n";
    }

    return (listFunctionCode);
}

void UnitTestCodeProduceFunc::getParamList(string& functionParam, const MFunction::FunctionParamList& info)
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
    else
    {
        returnValue = "return " + returnType + "();";
    }
}

string UnitTestCodeProduceFunc::createUnitTestCode(string testName, string Tag) { return (string("")); }

string UnitTestCodeProduceFunc::createUnitTestSectionCode(string sectionName, string checkInfo) { return (string("")); }
