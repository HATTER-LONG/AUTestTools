#include "FunctionInfoTestCode.h"

extern int externFunctionHasNoBody(int param);



int simpleFunction(int unuseparam)
{
    int unuse = 0;
    return 0;
}

int trainCallOtherFunction(int param)
{
    externFunctionHasNoBody(param);
    headFileDefinedFunc();
    simpleFunction(param);
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