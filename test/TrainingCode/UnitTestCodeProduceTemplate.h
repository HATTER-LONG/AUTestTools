#include "spdlog/spdlog.h"

namespace MyTestNameSpace
{
class TestA
{
public:
    TestA();
    ~TestA();

    int functionPublic(int);

private:
    int functionPrivate(void);

    int m_member;
};
}   // namespace MyTestNameSpace