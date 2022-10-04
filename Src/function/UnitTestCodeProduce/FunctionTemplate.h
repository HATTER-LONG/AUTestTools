#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace MyFunction
{

using namespace std;

inline void subreplace(
    string& resourceStr, const string& subStr, const string& newStr)
{
    string::size_type pos = 0;
    while (
        (pos = resourceStr.find(subStr)) != string::npos)   // 替换所有指定子串
    {
        resourceStr.replace(pos, subStr.length(), newStr);
    }
}

inline const string getClassName(const string& srcStr)
{
    string::size_type posSubstringStart;
    string::size_type posSeparator;

    posSeparator = srcStr.find("::");
    posSubstringStart = 0;
    if (string::npos != posSeparator)
    {
        cout << srcStr.substr(
            posSubstringStart, posSeparator - posSubstringStart)
             << endl;
        return srcStr.substr(
            posSubstringStart, posSeparator - posSubstringStart);
    }
    return "";
}

inline const string getFunctionName(const string& srcStr)
{
    string::size_type posSubstringStart;
    string::size_type posSeparator;

    posSeparator = srcStr.size();
    posSubstringStart = srcStr.find("::") + 2;
    if (string::npos != posSeparator)
    {
        cout << srcStr.substr(
            posSubstringStart, posSeparator - posSubstringStart)
             << endl;
        return srcStr.substr(
            posSubstringStart, posSeparator - posSubstringStart);
    }
    return "";
}

static string cppFunctionTemplate = "\n\
${PARAMRETURN} ${CLASSNAME}::${FUNCTIONNAME}(${PARAM})\n\
{\n\
    ${RETURNVALUE}\n\
}";

static string functionTemplate = "\n\
${PARAMRETURN} ${FUNCTIONNAME}(${PARAM})\n\
{\n\
    ${RETURNVALUE}\n\
}";

static string catchUnitTestCaseTemplate = "\n\
TEST_CASE(\"${TESTNAME}\",\"${TESTTAGS}\") \n\
{\
    ${TESTSECTION}\
}";

static string catchUnitTestSectionTemplate = "\n\
    SECTION(\"${SECTIONNAME}\")\n\
    {\
        REQUIRE(${SECTIONCHECK})\n\
    }\n\
";
}   // namespace MyFunction
