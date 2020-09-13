#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace MyFunction
{

using namespace std;

inline void subreplace(string& resource_str, const string& sub_str, const string& new_str)
{
    string::size_type pos = 0;
    while ((pos = resource_str.find(sub_str)) != string::npos) //替换所有指定子串
    {
        resource_str.replace(pos, sub_str.length(), new_str);
    }
}

inline const string GetClassName(const string& srcStr)
{
    string::size_type posSubstringStart;
    string::size_type posSeparator;

    posSeparator = srcStr.find("::");
    posSubstringStart = 0;
    if (string::npos != posSeparator)
    {
        cout << srcStr.substr(posSubstringStart, posSeparator - posSubstringStart) << endl;
        return srcStr.substr(posSubstringStart, posSeparator - posSubstringStart);
    }
    return "";
}

inline const string GetFunctionName(const string& srcStr)
{
    string::size_type posSubstringStart;
    string::size_type posSeparator;

    posSeparator = srcStr.size();
    posSubstringStart = srcStr.find("::") + 2;
    if (string::npos != posSeparator)
    {
        cout << srcStr.substr(posSubstringStart, posSeparator - posSubstringStart) << endl;
        return srcStr.substr(posSubstringStart, posSeparator - posSubstringStart);
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
TEST_CASE(\"${TESTNAME}\",\"${TESTTAGS}\") \
{\n\
    ${TESTSECTION}\
}\n\
";

static string CatchUnitTestSectionTemplate = "\n\
SECTION(${SECTIONNAME})\n\
{\n\
    REQUIRE(${SECTIONCHECK})\n\
}";
} // namespace MyFunction
