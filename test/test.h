#ifndef __TEST__H__
#define __TEST__H__
#include <string>

//注意修改 test.h 与 test.cpp 文件的同时需要更新以下两个宏，避免单元测试失败
#define TESTCXX_ERRORCOUNT 6
#define TESTCXX_FUNCTIONCOUNT 25

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