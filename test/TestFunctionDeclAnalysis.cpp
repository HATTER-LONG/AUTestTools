#include "catch2/catch.hpp"
#include "spdlog/spdlog.h"

#include "Tools/InputParamSaver.hpp"
#include "function/FunctionDeclAnalysis.h"
#include "test.h"

#include <utility>
#include <vector>



TEST_CASE("FunctionDeclAnalysis base test", "[function decl analysis test]")
{
    std::string analysisFilePath = "./test/test.cxx";
    std::string comipleCommandFilePath = "./build/compile_commands.json";
    if (InputParamSaver::instance()->getSaver().size() == 3)
    {
        analysisFilePath = InputParamSaver::instance()->getSaver()[1];
        comipleCommandFilePath = InputParamSaver::instance()->getSaver()[2];
    }

    spdlog::info("analysis file pathname is {}, compile command file path is {}", analysisFilePath.c_str(),
                 comipleCommandFilePath.c_str());
    auto* fundeclanalysisptr = new MFunction::FunctionDeclAnalysis(analysisFilePath, comipleCommandFilePath);
    REQUIRE(fundeclanalysisptr != nullptr);

    {
        int result = fundeclanalysisptr->StartToAnalysis();
        // 0 on success; 1 if any error occurred; 2 if there is no error but some files are skipped due to missing
        // compile commands.
        REQUIRE(result == 0);
    }

    {
        const MFunction::SourceCodeErrorMessageList& tmpvector = fundeclanalysisptr->GetErrorMessageRef();
        REQUIRE(tmpvector.size() == TESTCXX_ERRORCOUNT);
    }

    {
        const MFunction::SourceCodeFunctionMessageMap& tmpvector = fundeclanalysisptr->GetFunctionMessageRef();
        REQUIRE(tmpvector.size() == TESTCXX_FUNCTIONCOUNT);
    }
}
