#include "fakeit/catch/fakeit.hpp"
#include <string>

using namespace std;
using namespace fakeit;
struct SomeInterface
{
    virtual int foo(int) = 0;
    virtual int bar(string) = 0;
};

TEST_CASE("fake it test", "fake it")
{
    Mock<SomeInterface> mock;

    When(Method(mock, foo)).AlwaysReturn(1);

    SomeInterface& i = mock.get();

    // Production code
    i.foo(1);

    // Verify method mock.foo was invoked.
    Verify(Method(mock, foo));

    // Verify method mock.foo was invoked with specific arguments.
    Verify(Method(mock, foo).Using(1));

    REQUIRE(i.foo(1) == 1);
}