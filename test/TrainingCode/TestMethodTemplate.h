#include <string>

namespace MyTestNameSpace2
{
class TestB
{
public:
    TestB();
    ~TestB();

    int functionPublic(int);

private:
    int functionPrivate(void);

    int m_member;
};
}   // namespace MyTestNameSpace2