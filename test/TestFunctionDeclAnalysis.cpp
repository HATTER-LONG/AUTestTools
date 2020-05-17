#include "catch2/catch.hpp"
#include "spdlog/spdlog.h"

#include "Tools/InputParamSaver.hpp"
#include "function/FunctionDeclAnalysis.h"

#include <utility>
#include <vector>



TEST_CASE("FunctionDeclAnalysis base test", "[function decl analysis test]")
{
    auto* fundeclanalysisptr = new FunctionDeclAnalysis("./test/test.cxx", "./build/compile_commands.json");
    REQUIRE(fundeclanalysisptr != nullptr);

    {
        int result = fundeclanalysisptr->StartToAnalysis();
        spdlog::info("StartToAnalysis Result is {}\n", result);
    }

    {
        std::vector<SourceCodeErrorMessage> tmpvector = fundeclanalysisptr->GetErrorMessage();
        REQUIRE(!tmpvector.empty());
        for (auto a : tmpvector)
        {
            spdlog::info("errorLevel[{}] && message[{}] && filepos[{}]", a.GetErrorLevel(), a.GetErrorMessage(),
                         a.GetErrorPos());
        }
    }

    {
        std::map<std::string, SourceCodeFunctionMessage> tmpvector = fundeclanalysisptr->GetFunctionMessage();
        REQUIRE(!tmpvector.empty());
    }
}
