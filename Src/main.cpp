#include "function/AnalysisMessage.h"
#include "function/SourceCodeAnalysisFunc.h"
#include "spdlog/spdlog.h"

void help(int Argc)
{
    if (Argc < 3)
        spdlog::info("Usage:  Automated_UnitTest_Tools targetFile_path compile_commands_path");
}

int main(int argc, char* argv[])
{
    help(argc);

    auto sourceCodeMessagePtr = MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass("level_1");

    sourceCodeMessagePtr->setCompileDatabase(std::string(argv[2]));
    MyFunction::SourceCodeFunctionMessageMap functionmessage;
    MyFunction::SourceCodeErrorMessageList errormessage;
    sourceCodeMessagePtr->startToAnalysisSourceCode(functionmessage, errormessage);

    for (auto& function : functionmessage)
    {
        spdlog::info("Function Name is {}", function.first);
    }

    return 0;
}