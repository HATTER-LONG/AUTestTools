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

// TODO: 1. 完善代码生成相关单元测试，以及构造析构带有返回值问题
// TODO: 2. 完成自动生成未实现的代码桩函数，以及自动检测关联的类方法桩实现
// TODO: 3. 工厂实现自定义参数扩展
// TODO: 4. 完成根据头文件以及目标函数名称进行桩函数自动生成
// TODO: 5. 使用 llvm 提供的入参解析

int main(int argc, char* argv[])
{
    help(argc);

    if (argc == 3)
    {
        auto sourceCodeMessagePtr =
            MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass("FuncDeclWithCallExpr_v1");
        sourceCodeMessagePtr->setCompileDatabase(std::string(argv[2]));
        sourceCodeMessagePtr->setFilePathToAnalysis(std::string(argv[1]));

        MyFunction::SourceCodeFunctionMessageMap functionmessage;
        MyFunction::SourceCodeErrorMessageList errormessage;
        if (sourceCodeMessagePtr->startToAnalysisSourceCode(functionmessage, errormessage))
        {
            std::string functionSourceCode;
            MyFunction::UnitTestCodeProduceFunc generate;
            for (auto& function : functionmessage)
            {
                if (!function.second.getFunctionHasBodyInfo())
                {
                    // spdlog::info("Function Name is {} generate body is", function.first);
                    auto functionInfo = function.second;
                    functionSourceCode += generate.createMockSourceCode(functionInfo);
                }
            }
            spdlog::info("{}", functionSourceCode);
        }
        for (auto& error : errormessage)
        {
            spdlog::info("error message = {}", error.getErrorMessage());
        }
    }
    else
    {
        auto sourceCodeMessagePtr =
            MyFunction::g_SourceCodeAnalysisFactory::instance().getProductClass("CxxMethodDeclWithName_v1");
        sourceCodeMessagePtr->setCompileDatabase(std::string(argv[2]));
        sourceCodeMessagePtr->setFilePathToAnalysis(std::string(argv[1]));
        ConfigInfo config;
        config["ClassName"] = argv[3];
        sourceCodeMessagePtr->setConfigToAnalysis(config);
        MyFunction::SourceCodeFunctionMessageMap functionmessage;
        MyFunction::SourceCodeErrorMessageList errormessage;
        if (sourceCodeMessagePtr->startToAnalysisSourceCode(functionmessage, errormessage))
        {
            std::string functionSourceCode;
            MyFunction::UnitTestCodeProduceFunc generate;
            for (auto& function : functionmessage)
            {
                if (!function.second.getFunctionHasBodyInfo())
                {
                    // spdlog::info("Function Name is {} generate body is", function.first);
                    auto functionInfo = function.second;
                    functionSourceCode += generate.createMockSourceCode(functionInfo);
                }
            }
            spdlog::info("{}", functionSourceCode);
        }
    }
}