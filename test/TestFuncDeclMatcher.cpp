#include "TrainingCode/FunctionInfoTestCode.h"
#include "catch2/catch.hpp"
#include "function/AnalysisMessageHelper.h"
#include "function/SourceCodeAnalysisFunc.h"
#include "function/utilities.h"
#include "spdlog/spdlog.h"

TEST_CASE("Test Level_'ID' analysis code tool functions Get normally", "[Source code analysis function]")
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
            auto fundeclanalysisptr = MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass(ID);
            THEN("Check return value") { REQUIRE(fundeclanalysisptr != nullptr); }
        }
        WHEN("start analysis code infomation")
        {
            static MyFunction::SourceCodeErrorMessageList errormessage;
            static MyFunction::SourceCodeFunctionMessageMap funcmessagemap;

            THEN("start analysis and Check return value")
            {
                auto fundeclanalysisptr = MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass(ID);
                fundeclanalysisptr->setFilePathToAnalysis(analysisFilePath);
                fundeclanalysisptr->setCompileDatabase(comipleCommandFilePath);
                bool result = fundeclanalysisptr->startToAnalysisSourceCode(funcmessagemap, errormessage);

                REQUIRE(result == true);
            }

            THEN("Check infomation about source code funcdecl") { REQUIRE(funcmessagemap.size() == TESTCXX_FUNCTIONCOUNT); }
            THEN("Check infomation about error info") { REQUIRE(errormessage.size() == TESTCXX_ERRORCOUNT); }
            THEN("Check infomation about function has body list")
            {
                REQUIRE(MyFunction::getHasBodyOfFunctionMap(funcmessagemap).size() == TESTCXX_DEFINEDFUNC);
            }
            THEN("Check infomation about function no body list")
            {
                REQUIRE(MyFunction::getNoHasBodyOfFunctionmap(funcmessagemap).size() == TESTCXX_NOBODYFUNC);
            }
        }
    }
}
