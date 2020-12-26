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
    simpleFunction(param);
    return 0;
}
