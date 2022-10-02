
#include "TrainingCode/FunctionInfoTestCode.h"
#include "catch2/catch_test_macros.hpp"
#include "function/AnalysisMessageHelper.h"
#include "function/SourceCodeAnalysisFunc.h"
#include "function/utilities.h"
#include "spdlog/spdlog.h"

using namespace nlohmann;

class CxxMethodDefListernAnalysisTools
{
public:
    CxxMethodDefListernAnalysisTools()
    {
        REQUIRE(!m_analysisFilePath.empty());
        REQUIRE(!m_comipleCommandFilePath.empty());
    }
    auto initCxxMethodDeclAnalysisPtr(ConfigInfo& Config)
    {
        auto cxxmethoddeclanalysisptr =
            MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass(
                m_id);
        cxxmethoddeclanalysisptr->setCompileDatabase(m_comipleCommandFilePath);
        cxxmethoddeclanalysisptr->setFilePathToAnalysis(m_analysisFilePath);
        cxxmethoddeclanalysisptr->setConfigToAnalysis(Config);
        return cxxmethoddeclanalysisptr;
    }
    std::string m_analysisFilePath { MyFunction::TRAININGCODE_FILEPATH
                                     + "FunctionInfoTestCode.cxx" };
    std::string m_comipleCommandFilePath {
        MyFunction::COMPILECOMMANDS_INFOFILE
    };
    std::string m_id { "CxxMethodDeclWithName_v1" };
    MyFunction::SourceCodeFunctionMessageMap m_functionMessage;
    MyFunction::SourceCodeErrorMessageList m_errorMessage;
};

TEST_CASE_METHOD(CxxMethodDefListernAnalysisTools,
    "Test class decl analysis code tool functions Get normally",
    "[Source code analysis function]")
{
    GIVEN("Source Code function ID")
    {
        WHEN("Try to obtain the CxxMethodDeclWithName_'v1' source code message "
             "function from factory")
        {
            auto cxxmethoddeclanalysisptr =
                MyFunction::g_SourceCodeAnalysisFactory::instance()
                    .getProductClass(m_id);
            THEN("Check return value")
            {
                REQUIRE(cxxmethoddeclanalysisptr != nullptr);
            }
        }
        WHEN("Config analysis function error")
        {
            ConfigInfo config;
            config["ErrorClassName"] = "Error";
            auto cxxmethoddeclanalysisptr =
                initCxxMethodDeclAnalysisPtr(config);
            THEN("start analysis check return value")
            {
                REQUIRE_FALSE(
                    cxxmethoddeclanalysisptr->startToAnalysisSourceCode(
                        m_functionMessage, m_errorMessage));
            }
        }
        WHEN("Config correct parm and analysis")
        {
            ConfigInfo config;
            config["ClassName"] = "HeadFileDefinedClass";
            auto cxxmethoddeclanalysisptr =
                initCxxMethodDeclAnalysisPtr(config);
            THEN("start analysis check return value")
            {
                REQUIRE(cxxmethoddeclanalysisptr->startToAnalysisSourceCode(
                    m_functionMessage, m_errorMessage));
                REQUIRE(m_functionMessage.size() == TESTCXX_DECLMETHOD);
                REQUIRE(m_errorMessage.size() == TESTCXX_ERRORCOUNT);
            }
        }
    }
}
