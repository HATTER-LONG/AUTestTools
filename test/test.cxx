#include "spdlog/spdlog.h"
#include <iostream>
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

void testif_else()
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
        spdlog::info("{} call\n", __FUNCTION__);
        parentFunction(2, 'b');
    }

    void parentFunction(int test1, char test2) { spdlog::info("{} call {} -- {}\n", __FUNCTION__, test1, test2); }
};
class testclass : public testclassparent
{
    testclass()
    {
        spdlog::info("{} call\n", __FUNCTION__);
        parentFunction(3, 'c');
    }
};
template <typename T> void bar(T&& t);
