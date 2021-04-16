#include "FunctionInfoTestCode.h"

extern int externFunctionHasNoBody(int Param);

int simpleFunction(int Param)
{
    int unuse = 0;
    return 0;
}

int trainCallOtherFunction(int Param)
{
    int unuse2 = 0;
    externFunctionHasNoBody(Param);
    headFileDefinedFunc();
    simpleFunction(Param);
    return 0;
}

bool HeadFileDefinedClass::classPublicFuncWithNormalDef()
{
    return true;
}

bool HeadFileDefinedClass::classPrivateFuncWithNormalDef()
{
    return false;
}