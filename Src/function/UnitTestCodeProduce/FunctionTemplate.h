#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace MyFunction
{

using namespace std;

inline void subreplace(string& ResourceStr, const string& SubStr, const string& NewStr)
{
    string::size_type pos = 0;
    while ((pos = ResourceStr.find(SubStr)) != string::npos)   //替换所有指定子串
    {
        ResourceStr.replace(pos, SubStr.length(), NewStr);
    }
}

inline const string getClassName(const string& SrcStr)
{
    string::size_type posSubstringStart;
    string::size_type posSeparator;

    posSeparator = SrcStr.find("::");
    posSubstringStart = 0;
    if (string::npos != posSeparator)
    {
        cout << SrcStr.substr(posSubstringStart, posSeparator - posSubstringStart) << endl;
        return SrcStr.substr(posSubstringStart, posSeparator - posSubstringStart);
    }
    return "";
}

inline const string getFunctionName(const string& SrcStr)
{
    string::size_type posSubstringStart;
    string::size_type posSeparator;

    posSeparator = SrcStr.size();
    posSubstringStart = SrcStr.find("::") + 2;
    if (string::npos != posSeparator)
    {
        cout << SrcStr.substr(posSubstringStart, posSeparator - posSubstringStart) << endl;
        return SrcStr.substr(posSubstringStart, posSeparator - posSubstringStart);
    }
    return "";
}

static string CppFunctionTemplate = "\n\
${PARAMRETURN} ${CLASSNAME}::${FUNCTIONNAME}(${PARAM})\n\
{\n\
    ${RETURNVALUE}\n\
}";

static string FunctionTemplate = "\n\
${PARAMRETURN} ${FUNCTIONNAME}(${PARAM})\n\
{\n\
    ${RETURNVALUE}\n\
}";

static string CatchUnitTestCaseTemplate = "\n\
TEST_CASE(\"${TESTNAME}\",\"${TESTTAGS}\") \n\
{\
    ${TESTSECTION}\
}";

static string CatchUnitTestSectionTemplate = "\n\
    SECTION(\"${SECTIONNAME}\")\n\
    {\
        REQUIRE(${SECTIONCHECK})\n\
    }\n\
";
}   // namespace MyFunction
