#include "function/AnalysisMessage.h"
#include "function/SourceCodeAnalysisFunc.h"
#include "function/UnitTestCodeProduce.h"
#include "spdlog/spdlog.h"

void help(int Argc)
{
    if (Argc < 3)
    {
        spdlog::info("Usage:  Automated_UnitTest_Tools targetFile_path compile_commands_path");
        exit(-1);
    }
}

int main(int argc, char* argv[])
{
    help(argc);
    auto sourceCodeMessagePtr = MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass("FuncDeclWithCallExpr_v1");
    sourceCodeMessagePtr->setCompileDatabase(std::string(argv[2]));
    sourceCodeMessagePtr->setFilePathToAnalysis(std::string(argv[1]));

    MyFunction::SourceCodeFunctionMessageMap functionmessage;
    MyFunction::SourceCodeErrorMessageList errormessage;
    sourceCodeMessagePtr->startToAnalysisSourceCode(functionmessage, errormessage);

    for (auto& function : functionmessage)
    {
        if (!function.second.getFunctionHasBodyInfo())
        {
            spdlog::info("Function Name is {} generate body is", function.first);
            auto functionInfo = function.second;
            MyFunction::UnitTestCodeProduceFunc generate;
            auto functionSourceCode = generate.createMockSourceCode(functionInfo);
            spdlog::info("{}", functionSourceCode);
        }
    }

    return 0;
}