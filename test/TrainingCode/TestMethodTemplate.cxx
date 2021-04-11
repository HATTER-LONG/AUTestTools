#include "TestMethodTemplate.h"
#include "spdlog/spdlog.h"

using namespace MyTestNameSpace2;

int cTypeFunctionByExtern(std::string StrParm, int IntParm)
{
    spdlog::info("print input str parm is {}", StrParm);
    return IntParm;
}

TestB::TestB()
        : m_member(0)
{
    cTypeFunctionByExtern(__FUNCTION__, 0);
}

TestB::~TestB() { }

int TestB::functionPublic(int Parm)
{
    cTypeFunctionByExtern(__FUNCTION__, 0);
    return functionPrivate() + Parm;
}

int TestB::functionPrivate(void)
{
    m_member++;
    return 0;
}