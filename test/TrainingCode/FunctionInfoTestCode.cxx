#include "FunctionInfoTestCode.h"

extern int ExternFunctionHasNoBody(int param);

int SimpleFunction(int unuseparam)
{
    int unuse = 0;
    return 0;
}

int TrainCallOtherFunction(int param)
{
    ExternFunctionHasNoBody(param);
    SimpleFunction(param);
    return 0;
}
