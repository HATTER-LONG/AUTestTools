#include "Tools/InputParamSaver.hpp"
#include "TrainingCode/test.h"
#include "catch2/catch.hpp"
#include "function/SourceCodeAnalysisFunc.h"
#include "function/utilities.h"
#include "spdlog/spdlog.h"

// ! 此用例用于测试 Level_1 CustomASTFront 的方法，已经弃用 关闭此测试用例
// TODO: 整理抽离相关实现代码并删除
TEST_CASE("FunctionDeclAnalysis base test", "[.][function decl analysis test]")
{
    std::string analysisFilePath = MyFunction::TRAININGCODE_FILEPATH + "test.cxx";
    std::string comipleCommandFilePath = MyFunction::COMPILECOMMANDS_INFOFILE;
    REQUIRE(!analysisFilePath.empty());
    REQUIRE(!comipleCommandFilePath.empty());
    spdlog::info("analysis file pathname is {}, compile command file path is {}", analysisFilePath.c_str(),
        comipleCommandFilePath.c_str());
    auto fundeclanalysisptr = MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass("level_1");
    REQUIRE(fundeclanalysisptr != nullptr);
    MyFunction::SourceCodeErrorMessageList errormessage;
    MyFunction::SourceCodeFunctionMessageMap funcmessagemap;
    {
        fundeclanalysisptr->setCompileDatabase(comipleCommandFilePath);
        fundeclanalysisptr->setFilePathToAnalysis(analysisFilePath);
        bool result = fundeclanalysisptr->startToAnalysisSourceCode(funcmessagemap, errormessage);
        REQUIRE(result);
    }
    {
        REQUIRE(errormessage.size() == TESTCXX_ERRORCOUNT);
    }

    {
        REQUIRE(funcmessagemap.size() == TESTCXX_FUNCTIONCOUNT);
    }
}