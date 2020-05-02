#include "catch2/catch.hpp"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <string>

TEST_CASE("Modern C++ used algorithm", "[algorithm]")
{
    SECTION("Simple C array used in a modern C++ iterator")
    {
        int integers[] = {2, 5, 8, 22, 45, 67, 99};
        std::reverse(std::begin(integers), std::end(integers));
        REQUIRE(integers[0] == 99);
    }

    SECTION("string used in a modern C++ iterator")
    {
        std::string text{"the big brown fox jumps over the lazy dog!"};
        std::reverse(std::begin(text) + 3, std::end(text) - 4);
    }

    SECTION("Algorithm sort & foreach use")
    {
        std::vector<std::string> names = {"nn", "aa", "dds"};
        std::sort(std::begin(names), std::end(names));
        std::vector<std::string> namesaftersort = {"aa", "dds", "nn"};
        REQUIRE(names == namesaftersort);
    }

    SECTION("Algorithm equal use by default compare func")
    {
        const std::vector<std::string> names1{"Peter", "Harry", "Marc"};
        const std::vector<std::string> names2{"Peter", "Harry", "Marc"};

        const bool isEqual = std::equal(std::begin(names1), std::end(names1), std::begin(names2),
                                        std::end(names2)); // defatule use == to compare
        REQUIRE(isEqual == true);
    }

    SECTION("Algorithm equal use by custom compare func")
    {
        const std::vector<std::string> names1{"Peter", "Harry", "Marc"};
        const std::vector<std::string> names2{"Peter", "Harry", "Marc"};

        const bool isEqual = std::equal(
            std::begin(names1), std::end(names1), std::begin(names2), std::end(names2),
            [](const std::string& string1, const std::string& string2) { return (string1.compare(string2) == 0); });
        REQUIRE(isEqual == true);
    }
}