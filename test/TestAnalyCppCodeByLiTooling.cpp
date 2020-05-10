#include "catch2/catch.hpp"
#include "spdlog/spdlog.h"

#include "Function/AnalyseCodeTree.h"
#include "Tools/InputParamSaver.hpp"
TEST_CASE("Analysis of the code by libTooling", "[Auto Analysis Code By AST Tree]")
{
    const std::vector<std::string> param = InputParamSaver::instance()->getSaver();

    SECTION("Test Fuction argv")
    {
        spdlog::info("Success\n");
        const char* argv[param.size()];
        for (auto i = 0; i < param.size(); i++)
        {
            argv[i] = param[i].c_str();
        }
        FunctionToAnalyzeCodeError(param.size(), argv);
        // FunctionToAnalyzeCodeTree(param.size(), argv);
    }
}