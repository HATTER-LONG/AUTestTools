#include "TrainingCode/FunctionInfoTestCode.h"
#include "catch2/catch.hpp"
#include "function/AnalysisMessageHelper.h"
#include "function/SourceCodeAnalysisFunc.h"
#include "function/utilities.h"
#include "spdlog/spdlog.h"

class FuncDefListernAnalysisTools
{
public:
    FuncDefListernAnalysisTools()
    {
        REQUIRE(!m_analysisFilePath.empty());
        REQUIRE(!m_comipleCommandFilePath.empty());
    }
    std::string m_analysisFilePath { MyFunction::TRAININGCODE_FILEPATH + "FunctionInfoTestCode.cxx" };
    std::string m_comipleCommandFilePath { MyFunction::COMPILECOMMANDS_INFOFILE };
    std::string m_id { "FuncDeclWithCallExpr_v1" };
};

TEST_CASE_METHOD(FuncDefListernAnalysisTools, "Test analysis code tool functions Get normally", "[Source code analysis function]")
{
    GIVEN("Source Code function ID")
    {
        WHEN("Try to obtain the FuncDeclWithCallExpr_'v1' source code message function from factory")
        {
            auto fundeclanalysisptr = MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass(m_id);
            THEN("Check return value") { REQUIRE(fundeclanalysisptr != nullptr); }
        }
    }
}

TEST_CASE_METHOD(FuncDefListernAnalysisTools, "Test analysis code tool functions normally", "[Source code analysis function]")
{
    WHEN("start analysis code infomation")
    {
        static MyFunction::SourceCodeErrorMessageList errormessage;
        static MyFunction::SourceCodeFunctionMessageMap funcmessagemap;

        THEN("start analysis and Check return value")
        {
            auto fundeclanalysisptr = MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass(m_id);
            fundeclanalysisptr->setFilePathToAnalysis(m_analysisFilePath);
            fundeclanalysisptr->setCompileDatabase(m_comipleCommandFilePath);
            bool result = fundeclanalysisptr->startToAnalysisSourceCode(funcmessagemap, errormessage);

            REQUIRE(result == true);
        }

        THEN("Check infomation about source code")
        {
            REQUIRE(funcmessagemap.size() == TESTCXX_FUNCTIONCOUNT);
            REQUIRE(errormessage.size() == TESTCXX_ERRORCOUNT);
            REQUIRE(MyFunction::getHasBodyOfFunctionMap(funcmessagemap).size() == TESTCXX_DEFINEDFUNC);
            REQUIRE(MyFunction::getNoHasBodyOfFunctionmap(funcmessagemap).size() == TESTCXX_NOBODYFUNC);
        }
    }
}