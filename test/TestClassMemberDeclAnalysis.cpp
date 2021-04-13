
#include "TrainingCode/FunctionInfoTestCode.h"
#include "catch2/catch.hpp"
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
        // TODO: 完成传入错误配置后需要进行的校验
        json object;
        object["ClassName"] = "zzzz";
        spdlog::info("is_object = {}| object = \n{}", object.is_object(), object.dump(1, '\t'));
        std::string aa;
        try
        {
            object.at("ClassName").get_to(aa);
            object.at("ClassName2").get_to(aa);
        }
        catch (json::exception& a)
        {
            spdlog::info("error is a = {}", a.what());
        }

        spdlog::info("222 object = \n{}", object.dump(1, '\t'));
    }
}