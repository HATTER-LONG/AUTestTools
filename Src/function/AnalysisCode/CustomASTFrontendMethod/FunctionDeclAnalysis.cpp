#include "FuncDeclASTFrontendAction.hpp"
#include "FunctionDeclAnalysis.h"
#include "Infra/FactoryTemplate.h"
#include "function/utilities.h"
#include "spdlog/spdlog.h"

#include <clang/Tooling/Tooling.h>
#include <cmath>
#include <string>
using namespace clang::tooling;

static Infra::ProductClassRegistrar<MyFunction::SourceCodeAnalysisFunc, MyFunction::FunctionDeclAnalysis> funcdeclAnalysisMethod(
    MyFunction::FunctionDeclAnalysis::getFactoryID());
namespace MyFunction
{
class MyFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    std::unique_ptr<clang::FrontendAction> create() override { return std::unique_ptr<clang::FrontendAction>(MyFrontendAction); }
    void setPtr(clang::FrontendAction* Tmpptr) { MyFrontendAction = Tmpptr; }

private:
    clang::FrontendAction* MyFrontendAction;
};

bool FunctionDeclAnalysis::startToAnalysisSourceCode(
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

    const char* argv[3];
    for (int i = 0; i < argc; i++)
    {
        spdlog::info("argc = [{}], argv = [{}]\n", i, argv_tmp[i]);
        argv[i] = argv_tmp[i];
    }

    clang::tooling::CommonOptionsParser op(argc, argv, ToolingSampleCategory);
    std::vector<std::string> sourcepath;
    sourcepath.emplace_back(argv[1]);
    clang::tooling::ClangTool Tool(op.getCompilations(), sourcepath);
    ArgumentsAdjuster ardj1 = getInsertArgumentAdjuster(MyFunction::CLANG_ARGS2APPEND.c_str());
    Tool.appendArgumentsAdjuster(ardj1);

    Functionmessage.clear();
    Errormessage.clear();

    auto* tmpFrontendAction = new FuncDeclAnalysisFrontendAction(Functionmessage, Errormessage);
    MyFrontendActionFactory frontACtionFactory;
    frontACtionFactory.setPtr(tmpFrontendAction);
    // 0 on success; 1 if any error occurred; 2 if there is no error but some files are skipped due to missing
    // compile commands.
    int errorcode = Tool.run(&frontACtionFactory);
    if (errorcode != 0)
    {
        spdlog::info("[{}] error for analysis file[{}] errnocode[{}]", __FUNCTION__, SourceCodeFilePath, errorcode);
        return false;
    }
    return true;
}

}   // namespace MyFunction
