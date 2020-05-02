//#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do
#define CATCH_CONFIG_RUNNER
#include "Tools/InputParamSaver.hpp"
#include "catch2/catch.hpp"



int main(int argc, char* argv[])
{
    InputParamSaver::instance()->ToSaver(argc, argv);
    return Catch::Session().run();
}