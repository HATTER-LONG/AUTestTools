#include "Tools/InputParamSaver.hpp"
#include "catch2/catch.hpp"
#include "function/AnalysisMessage.h"
#include "function/SourceCodeAnalysisFunc.h"
#include "function/utilities.h"
#include "spdlog/spdlog.h"
#include "test.h"
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
    auto fundeclanalysisptr = MyFunction::g_SourceCodeAnalysisFactory::instance().GetProductClass("level_1");
    REQUIRE(fundeclanalysisptr != nullptr);
    MyFunction::SourceCodeErrorMessageList errormessage;
    MyFunction::SourceCodeFunctionMessageMap funcmessagemap;
    {
        fundeclanalysisptr->SetCompileDatabase(comipleCommandFilePath);
        fundeclanalysisptr->SetFilePathToAnalysis(analysisFilePath);
        bool result = fundeclanalysisptr->StartToAnalysisSourceCode(funcmessagemap, errormessage);
        REQUIRE(result);
    }

    {
        REQUIRE(errormessage.size() == TESTCXX_ERRORCOUNT);
    }

    {
        REQUIRE(funcmessagemap.size() == TESTCXX_FUNCTIONCOUNT);
    }
}
