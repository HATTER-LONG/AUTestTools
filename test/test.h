#ifndef __TEST__H__
#define __TEST__H__
#include <string>
class TestClass2
{
public:
    TestClass2();
    ~TestClass2();

    std::string TestFunction(int test); // { int b = 4; };
};

class TestClass3
{
public:
    TestClass3();
    ~TestClass3();

    std::string TestFunction3(int test); // { int b = 4; };
};
#endif