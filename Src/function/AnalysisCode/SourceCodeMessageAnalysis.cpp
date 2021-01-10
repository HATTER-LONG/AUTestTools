#include "FunctionDefLister.hpp"
#include "SourceCodeMessageAnalysis.h"
#include "function/utilities.h"

#include <algorithm>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <memory>
#include <spdlog/spdlog.h>

static llvm::cl::OptionCategory flt_cat("func-decl-list-am");
namespace MyFunction
{
using CommonOptionsParserPtr = std::unique_ptr<clang::tooling::CommonOptionsParser>;

void SourceCodeMessageAnalysis::paramInitialize(std::vector<std::string>& ParamList)
{
    ParamList.emplace_back(std::string("__READY_TO_ANALYSIS__"));
    ParamList.emplace_back(SourceCodeFilePath);
    if (!CompiledDatabasePath.empty())
    {
        std::string tmpstr = "-p=" + CompiledDatabasePath;
        ParamList.emplace_back(tmpstr);
    }
}

CommonOptionsParserPtr SourceCodeMessageAnalysis::getCommonOptionsParser()
{
    const int defaultArgc = 3;

    std::vector<std::string> paramList;
    paramInitialize(paramList);

    const char* argv[defaultArgc] = {
        nullptr,
    };
    for (int i = 0; i < paramList.size(); i++)
    {
        argv[i] = paramList[i].c_str();
    }
    int argc = paramList.size();
    return CommonOptionsParserPtr(std::make_unique<clang::tooling::CommonOptionsParser>(argc, argv, flt_cat));
}

bool SourceCodeMessageAnalysis::startToAnalysisSourceCode(
    SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage)
{
    return startToAnalysisSourceCode(Errormessage);
}

bool SourceCodeMessageAnalysis::startToAnalysisSourceCode(SourceCodeErrorMessageList& ErrorMessage)
{
    std::vector<std::string> SourceCodePathList;
    SourceCodePathList.emplace_back(SourceCodeFilePath);

    auto commonOptionsParserVar = getCommonOptionsParser();
    clang::tooling::ClangTool Tool(commonOptionsParserVar->getCompilations(), SourceCodePathList);
    clang::tooling::ArgumentsAdjuster ardj1 = clang::tooling::getInsertArgumentAdjuster(MyFunction::CLANG_ARGS2APPEND.c_str());
    Tool.appendArgumentsAdjuster(ardj1);

    clang::ast_matchers::MatchFinder finder;
    if (MatcherWithCallBack != nullptr)
    {
        finder.addMatcher(MatcherWithCallBack->matcher(), MatcherWithCallBack.get());
    }
    else
    {
        spdlog::warn("No matcher has set to analysis source code, please check it");
    }

    SourceCodeErrorAnalysis diagnosticConsumer(ErrorMessage);
    Tool.setDiagnosticConsumer(&diagnosticConsumer);
    int ret = Tool.run(clang::tooling::newFrontendActionFactory(&finder).get());
    if (ret != 0)
    {
        spdlog::info("[{}] error for analysis file[{}] errnocode[{}]", __FUNCTION__, SourceCodeFilePath, ret);
        return false;
    }
    return true;
}
}   // namespace MyFunction