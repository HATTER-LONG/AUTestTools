
#include "TrainingCode/FunctionInfoTestCode.h"
#include "catch2/catch.hpp"
#include "function/AnalysisMessageHelper.h"
#include "function/SourceCodeAnalysisFunc.h"
#include "function/utilities.h"
#include "spdlog/spdlog.h"

class CxxMethodDefListernAnalysisTools
{
public:
    CxxMethodDefListernAnalysisTools()
    {
        REQUIRE(!m_analysisFilePath.empty());
        REQUIRE(!m_comipleCommandFilePath.empty());
    }
    std::string m_analysisFilePath { MyFunction::TRAININGCODE_FILEPATH + "FunctionInfoTestCode.cxx" };
    std::string m_comipleCommandFilePath { MyFunction::COMPILECOMMANDS_INFOFILE };
    std::string m_id { "CxxMethodDeclWithName_v1" };
};

TEST_CASE_METHOD(CxxMethodDefListernAnalysisTools, "Test class decl analysis code tool functions Get normally",
    "[Source code analysis function]")
{
    GIVEN("Source Code function ID")
    {
        WHEN("Try to obtain the CxxMethodDeclWithName_'v1' source code message function from factory")
        {
            auto cxxmethoddeclanalysisptr = MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass(m_id);
            THEN("Check return value") { REQUIRE(cxxmethoddeclanalysisptr != nullptr); }
        }
    }
}