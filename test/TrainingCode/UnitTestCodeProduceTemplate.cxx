#include "TestMethodTemplate.h"
#include "UnitTestCodeProduceTemplate.h"

extern int cTypeFunctionByExtern(std::string StrParm, int IntParm);

namespace MyTestNameSpace
{
TestA::TestA()
        : m_member(0)
{
    cTypeFunctionByExtern(__FUNCTION__, 0);
}

TestA::~TestA() { }

int TestA::functionPublic(int Parm)
{
    cTypeFunctionByExtern(__FUNCTION__, 0);
    return functionPrivate() + Parm;
}

int TestA::functionPrivate(void)
{
    MyTestNameSpace2::TestB testB;
    testB.functionPublic(100);
    m_member++;
    return 0;
}

}   // namespace MyTestNameSpace

#include "sometestcode.hpp"