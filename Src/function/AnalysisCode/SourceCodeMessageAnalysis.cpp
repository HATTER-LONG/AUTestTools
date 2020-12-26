#include "FunctionDefLister.hpp"
#include "SourceCodeMessageAnlysis.h"
#include "function/utilities.h"

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
static llvm::cl::OptionCategory flt_cat("func-decl-list-am");
static Infra::ProductClassRegistrar<MyFunction::SourceCodeAnalysisFunc, MyFunction::SourceCodeMessageAnalysis>
    funcdeclAnalysisMethod(MyFunction::SourceCodeMessageAnalysis::getFactoryID());
namespace MyFunction
{
bool SourceCodeMessageAnalysis::startToAnalysisSourceCode(
    SourceCodeFunctionMessageMap& Functionmessage, SourceCodeErrorMessageList& Errormessage)
{
    int argc = 2;
    char argv_tmp[3][128] = {
        { 0 },
    };
    strcpy(argv_tmp[0], "__READY_TO_ANALYSIS__");
    strncpy(argv_tmp[1], SourceCodeFilePath.c_str(), SourceCodeFilePath.length());
    if (!CompiledDatabasePath.empty())
    {
        argc++;
        std::string tmpstr = "-p=" + CompiledDatabasePath;
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
    sourcepath.emplace_back(SourceCodeFilePath);
    clang::tooling::ClangTool Tool(op.getCompilations(), sourcepath);
    clang::tooling::ArgumentsAdjuster ardj1 = clang::tooling::getInsertArgumentAdjuster(MyFunction::CLANG_ARGS2APPEND.c_str());
    Tool.appendArgumentsAdjuster(ardj1);

    Functionmessage.clear();
    Errormessage.clear();

    clang::ast_matchers::MatchFinder finder;
    FunctionDefLister fp(Functionmessage);
    finder.addMatcher(fp.matcher(), &fp);
    SourceCodeErrorAnalysis diagnosticConsumer(Errormessage);
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