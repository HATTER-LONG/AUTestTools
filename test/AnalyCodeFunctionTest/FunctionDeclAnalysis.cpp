#include "FuncDeclASTFrontendAction.hpp"
#include "function/FunctionDeclAnalysis.h"
#include "function/utilities.h"
#include <clang/Tooling/Tooling.h>
#include <string>
using namespace clang::tooling;
namespace MyFunction
{
class MyFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    std::unique_ptr<clang::FrontendAction> create() override
    {
        return std::unique_ptr<clang::FrontendAction>(myFrontendAction);
    }
    void setPtr(clang::FrontendAction* tmpptr) { myFrontendAction = tmpptr; }

private:
    clang::FrontendAction* myFrontendAction;
};

FunctionDeclAnalysis::FunctionDeclAnalysis(std::string filepath, std::string compiledatabase)
        : sourceCodeFilePath(filepath), compiledDatabasePath(compiledatabase)
{
}

int FunctionDeclAnalysis::StartToAnalysis()
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

    sourceCodeErrorMessage.clear();
    sourceCodeFunctionMessage.clear();

    auto* tmpFrontendAction = new FuncDeclAnalysisFrontendAction(sourceCodeErrorMessage, sourceCodeFunctionMessage);
    MyFrontendActionFactory frontACtionFactory;
    frontACtionFactory.setPtr(tmpFrontendAction);
    return Tool.run(&frontACtionFactory);
}

const SourceCodeErrorMessageList& FunctionDeclAnalysis::GetErrorMessageRef() const { return sourceCodeErrorMessage; }

const SourceCodeFunctionMessageMap& FunctionDeclAnalysis::GetFunctionMessageRef() const
{
    return sourceCodeFunctionMessage;
};

} // namespace MyFunction
