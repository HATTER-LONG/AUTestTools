#include "FunctionDefLister.hpp"
#include "SourceCodeMessageAnlysis.h"
#include "function/utilities.h"
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
static llvm::cl::OptionCategory flt_cat("func-decl-list-am");
static Infra::ProductClassRegistrar<MyFunction::SourceCodeAnalysisFunc, MyFunction::SourceCodeMessageAnalysis>
    funcdeclAnalysisMethod(MyFunction::SourceCodeMessageAnalysis::GetFactoryID());
namespace MyFunction
{
bool SourceCodeMessageAnalysis::StartToAnalysisSourceCode(SourceCodeFunctionMessageMap& functionmessage,
                                                          SourceCodeErrorMessageList& errormessage)
{
    int argc = 2;
    char argv_tmp[3][128] = {
        {0},
    };
    strcpy(argv_tmp[0], "__READY_TO_ANALYSIS__");
    strncpy(argv_tmp[1], sourceCodeFilePath.c_str(), sourceCodeFilePath.length());
    if (!compiledDatabasePath.empty())
    {
        argc++;
        std::string tmpstr = "-p=" + compiledDatabasePath;
        strncpy(argv_tmp[2], tmpstr.c_str(), tmpstr.length());
    }

    const char* argv[3] = {
        nullptr,
    };
    for (int i = 0; i < argc; i++)
    {
        spdlog::info("argc = [{}], argv = [{}]\n", i, argv_tmp[i]);
        argv[i] = argv_tmp[i];
    }

    clang::tooling::CommonOptionsParser op(argc, argv, flt_cat);
    std::vector<std::string> sourcepath;
    sourcepath.emplace_back(sourceCodeFilePath);
    clang::tooling::ClangTool Tool(op.getCompilations(), sourcepath);
    clang::tooling::ArgumentsAdjuster ardj1 = clang::tooling::getInsertArgumentAdjuster(MyFunction::CLANG_ARGS2APPEND.c_str());
    Tool.appendArgumentsAdjuster(ardj1);

    functionmessage.clear();
    errormessage.clear();

    clang::ast_matchers::MatchFinder finder;
    FunctionDefLister fp(functionmessage);
    finder.addMatcher(fp.matcher(), &fp);
    int ret = Tool.run(clang::tooling::newFrontendActionFactory(&finder).get());
    if (ret != 0)
    {
        spdlog::info("[{}] error for analysis file[{}] errnocode[{}]", __FUNCTION__, sourceCodeFilePath, ret);
        return false;
    }
    return true;
}
} // namespace MyFunction