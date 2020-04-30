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
template <typename T> void bar(T&& t);
