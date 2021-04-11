#pragma once

#include <string>
#include <vector>

#define TESTCXX_ERRORCOUNT 1
#define TESTCXX_FUNCTIONCOUNT 4
#define TESTCXX_DEFINEDFUNC 3
#define TESTCXX_NOBODYFUNC 1
static std::vector<std::string> DefinedFuncNameList { "SimpleFunction", "TrainCallOtherFunction", "headFileDefinedFunc" };
static std::vector<std::string> NoneBodyFuncNameList { "externFunctionHasNoBody" };
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