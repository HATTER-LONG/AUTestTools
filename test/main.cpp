//#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do
#define CATCH_CONFIG_RUNNER
#include "Tools/InputParamSaver.hpp"
#include "catch2/catch.hpp"
#include "spdlog/spdlog.h"


int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        spdlog::info("{}={}", i, argv[i]);
    }
    InputParamSaver::instance()->ToSaver(argc, argv);
    return Catch::Session().run();
}