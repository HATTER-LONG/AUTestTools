#include "spdlog/spdlog.h"
#include "test.h"
#include <iostream>

extern int testExternFunction(int param);
enum class Cpp11Enum
{
    RED = 10,
    BLUE = 20
};

enum ClassicalEnum
{
    BLACK = 10
};
struct StructTypeVirtualClass
{
    virtual ~StructTypeVirtualClass() = default;
    virtual void foo(int i = 0) = 0;
};

struct StructTypeDerivedClass : StructTypeVirtualClass
{
    void foo(int) override;

    bool operator==(const StructTypeDerivedClass& o) const;
};

void test_if_else_branch(int unlessparam)
{
    int h = 1;
    if (int b = 1)
    {
        int a = 10;
    }
    else if (h == 1)
    {
        int b = 20;
    }
    else
    {
        int c = 20;
    }

    for (int i = 0; int b = 2 + i < 10; i++)
    {
        h++;
    }
}

std::string notcallall(TestClass3 b)
{
    TestClass3 tmp = b;
    for (int i = 0; int b = 2 + i < 10; i++)
    {
        int h = 0;
        h++;
    }
    return std::string("my test");
}

class TestClassTypeParent
{
public:
    TestClassTypeParent()
    {
        spdlog::info("{} call\n", __FUNCTION__);
        parentFunction(1, 'a');
    }

    ~TestClassTypeParent()
    {
        int adsds = 0;
        int adsdsd = 0;
    }

    void parentFunction(int test1, char test2) { spdlog::info("{} call {} -- {}\n", __FUNCTION__, test1, test2); }
};
class testclass : public TestClassTypeParent
{
    testclass()
    {
        spdlog::info("{} call\n", __FUNCTION__);
        testExternFunction(3);
        parentFunction(3, 'c');
        TestClass2 a;
        a.TestFunction(4);
    }
};


TestClass3::TestClass3()
{
    int unless = 0;
    spdlog::info("{} {}call\n", __FUNCTION__, unless);
}
TestClass3::~TestClass3()
{
    int unless = 0;
    spdlog::info("{} {}call\n", __FUNCTION__, unless);
}
std::string TestClass3::TestFunction3(int test)
{
    int sss;
    spdlog::info("{} {}{}call\n", __FUNCTION__, test, sss);
    return "a";
}
template <typename T>
void bar(T&& t);
