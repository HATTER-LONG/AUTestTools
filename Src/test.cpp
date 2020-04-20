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

    bool operator==(const Badabang &o) const;
};

void testif_else()
{
    if (1)
    {
        int a = 10;
    }
    else if (2)
    {
        int b = 20;
    }
    else
    {
        int c = 20;
    }
}
template <typename T>
void bar(T &&t);
