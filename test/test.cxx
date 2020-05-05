#include "spdlog/spdlog.h"
#include "test.h"
#include <iostream>

extern int testfunction(int a);
enum class Cpp11Enum
{
    RED = 10,
    BLUE = 20
};

struct Wowza
{
    virtual ~Wowza() = default;
    virtual void foo(int i = 0) = 0;
};

struct Badabang : Wowza
{
    void foo(int) override;

    bool operator==(const Badabang& o) const;
};

void testif_else(int ww)
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
}

class testclassparent
{
public:
    testclassparent()
    {
        spdlog::info("{} call\n", __FUNCTION__);
        parentFunction(1, 'a');
    }

    ~testclassparent()
    {
        int adsds = 0;
        int adsdsd = 0;
    }

    void parentFunction(int test1, char test2) { spdlog::info("{} call {} -- {}\n", __FUNCTION__, test1, test2); }
};
class testclass : public testclassparent
{
    testclass()
    {
        spdlog::info("{} call\n", __FUNCTION__);
        testfunction(3);
        parentFunction(3, 'c');
        TestClass2 a;
        a.TestFunction(4);
    }
};


TestClass3::TestClass3()
{
    int adsds = 0;
    spdlog::info("{} {}call\n", __FUNCTION__, adsds);
}
TestClass3::~TestClass3()
{
    int adsds = 0;
    spdlog::info("{} {}call\n", __FUNCTION__, adsds);
}
std::string TestClass3::TestFunction3(int test)
{
    int sss;
    spdlog::info("{} {}{}call\n", __FUNCTION__, test, sss);
    return "a";
}
template <typename T> void bar(T&& t);
