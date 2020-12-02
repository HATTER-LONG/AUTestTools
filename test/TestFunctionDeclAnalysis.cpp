#include "catch2/catch.hpp"
#include "spdlog/spdlog.h"

#include "Tools/InputParamSaver.hpp"
#include "function/FunctionDeclAnalysis.h"
#include "test.h"

#include "function/utilities.h"
#include <utility>
#include <vector>


TEST_CASE("FunctionDeclAnalysis base test", "[function decl analysis test]")
{
    std::string analysisFilePath = MyFunction::TESTCODEFILE;
    std::string comipleCommandFilePath = MyFunction::COMPILECOMMANDS_INFOFILE;

    REQUIRE(!analysisFilePath.empty());
    REQUIRE(!comipleCommandFilePath.empty());

    spdlog::info("analysis file pathname is {}, compile command file path is {}", analysisFilePath.c_str(),
                 comipleCommandFilePath.c_str());
    auto* fundeclanalysisptr = new MyFunction::FunctionDeclAnalysis(analysisFilePath, comipleCommandFilePath);
    REQUIRE(fundeclanalysisptr != nullptr);

    {
        int result = fundeclanalysisptr->StartToAnalysis();
        // 0 on success; 1 if any error occurred; 2 if there is no error but some files are skipped due to missing
        // compile commands.
        REQUIRE(result == 0);
    }

    {
        const MyFunction::SourceCodeErrorMessageList& tmpvector = fundeclanalysisptr->GetErrorMessageRef();
        REQUIRE(tmpvector.size() == TESTCXX_ERRORCOUNT);
    }

    {
        const MyFunction::SourceCodeFunctionMessageMap& tmpvector = fundeclanalysisptr->GetFunctionMessageRef();
        REQUIRE(tmpvector.size() == TESTCXX_FUNCTIONCOUNT);
    }
}
