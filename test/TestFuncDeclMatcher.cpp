#include "TrainingCode/FunctionInfoTestCode.h"
#include "catch2/catch.hpp"
#include "function/SourceCodeAnalysisFunc.h"
#include "function/utilities.h"
#include "spdlog/spdlog.h"



TEST_CASE("Test Level_2 analysis code tool functions Get normally", "[Source code analysis function]")
{
    std::string analysisFilePath = MyFunction::TRAININGCODE_FILEPATH + "FunctionInfoTestCode.cxx";
    std::string comipleCommandFilePath = MyFunction::COMPILECOMMANDS_INFOFILE;

    REQUIRE(!analysisFilePath.empty());
    REQUIRE(!comipleCommandFilePath.empty());

    std::string ID = "level_2";
    GIVEN("Source Code function ID")
    {
        WHEN("Try to obtain the Level_'ID' source code message function from factory")
        {
            auto fundeclanalysisptr = MyFunction::g_SourceCodeAnalysisFactory::instance().GetProductClass(ID);
            THEN("Check return value") { REQUIRE(fundeclanalysisptr != nullptr); }
        }
        WHEN("start analysis code infomation")
        {
            static MyFunction::SourceCodeErrorMessageList errormessage;
            static MyFunction::SourceCodeFunctionMessageMap funcmessagemap;

            THEN("start analysis and Check return value")
            {
                auto fundeclanalysisptr = MyFunction::g_SourceCodeAnalysisFactory::instance().GetProductClass(ID);
                fundeclanalysisptr->SetFilePathToAnalysis(analysisFilePath);
                fundeclanalysisptr->SetCompileDatabase(comipleCommandFilePath);
                bool result = fundeclanalysisptr->StartToAnalysisSourceCode(funcmessagemap, errormessage);

                REQUIRE(result == true);
            }

            THEN("Check infomation about source code") { REQUIRE(funcmessagemap.size() == TESTCXX_FUNCTIONCOUNT); }
        }
    }
}
