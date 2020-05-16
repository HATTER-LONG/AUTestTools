#include "../test/Tools/InputParamSaver.hpp"
#include "./FuncDeclASTFrontendAction.h"
#include "function/FunctionDeclAnalysis.h"
#include "spdlog/spdlog.h"
#include <clang/Frontend/FrontendAction.h>
FunctionDeclAnalysis::FunctionDeclAnalysis(std::string filepath, std::string compiledatabase)
        : sourceCodeFilePath(filepath), compiledDatabasePath(compiledatabase)
{
    spdlog::info("[{}] filepath = [{}] complieddatabase = [{}]\n", __FUNCTION__, filepath.c_str(),
                 compiledatabase.c_str());
}
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
bool FunctionDeclAnalysis::StartToAnalysis()
{
    int argc = 2;
    const char* argv[3];
    argv[1] = sourceCodeFilePath.c_str();
    if (!compiledDatabasePath.empty())
    {
        argc++;
        compiledDatabasePath = "\"" + compiledDatabasePath + "\"";
        compiledDatabasePath = "-p=" + compiledDatabasePath;
        argv[2] = sourceCodeFilePath.c_str();
    }
    std::vector<std::string> tmpvector = InputParamSaver::instance()->getSaver();
    argc = tmpvector.size();
    for (int i = 0; i < argc; i++)
    {
        argv[i] = tmpvector[i].c_str();
    }
    clang::tooling::CommonOptionsParser op(argc, argv, ToolingSampleCategory);
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());
    auto* tmpFrontendAction = new FuncDeclAnalysisFrontendAction(sourceCodeErrorMessage);
    MyFrontendActionFactory tmp;
    tmp.setPtr(tmpFrontendAction);
    Tool.run(&tmp);

    return true;
}

SourceCodeErrorMessageList FunctionDeclAnalysis::GetErrorMessage() { return sourceCodeErrorMessage; }

SourceCodeFunctionMessageMap FunctionDeclAnalysis::GetFunctionMessage() { return sourceCodeFunctionMessage; };