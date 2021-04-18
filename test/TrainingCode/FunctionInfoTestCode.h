#pragma once

#include <string>
#include <vector>

#define TESTCXX_ERRORCOUNT 2
#define TESTCXX_FUNCTIONCOUNT 4
#define TESTCXX_DEFINEDFUNC 3
#define TESTCXX_NOBODYFUNC 1

#define TESTCXX_DECLMETHOD 6

static std::vector<std::string> definedFuncNameList { "SimpleFunction", "TrainCallOtherFunction", "headFileDefinedFunc" };
static std::vector<std::string> noneBodyFuncNameList { "externFunctionHasNoBody" };
inline int headFileDefinedFunc()
{
    return 0;
}

class HeadFileDefinedClass
{
public:
    HeadFileDefinedClass() = default;
    ~HeadFileDefinedClass() = default;

public:
    bool classPublicFuncWithInline() { return true; }
    bool classPublicFuncWithNormalDef();

private:
    bool classPrivateFuncWithInline() { return false; }
    bool classPrivateFuncWithNormalDef();
};