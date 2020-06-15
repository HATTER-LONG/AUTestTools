#include "FuncDeclASTFrontendAction.hpp"
#include "function/FunctionDeclAnalysis.h"
#include <clang/Tooling/Tooling.h>
namespace MFunction
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
    spdlog::info("[{}] filepath = [{}] complieddatabase = [{}]\n", __FUNCTION__, filepath.c_str(),
                 compiledatabase.c_str());
}

int FunctionDeclAnalysis::StartToAnalysis()
{
    int argc = 2;
    const char* argv[3];
    argv[0] = "";
    argv[1] = sourceCodeFilePath.c_str();
    if (!compiledDatabasePath.empty())
    {
        argc++;
        argv[2] = ("-p=" + compiledDatabasePath).c_str();
    }

    for (int i = 0; i < argc; i++)
    {
        spdlog::info("argc = [{}], argv = [{}]\n", i, argv[i]);
    }
    clang::tooling::CommonOptionsParser op(argc, argv, ToolingSampleCategory);
    std::vector<std::string> sourcepath;
    sourcepath.emplace_back(argv[1]);
    clang::tooling::ClangTool Tool(op.getCompilations(), sourcepath);

    // sourceCodeErrorMessage.clear();
    // sourceCodeFunctionMessage.clear();

    auto* tmpFrontendAction = new FuncDeclAnalysisFrontendAction(sourceCodeErrorMessage, sourceCodeFunctionMessage);
    MyFrontendActionFactory tmp;
    tmp.setPtr(tmpFrontendAction);
    return Tool.run(&tmp);
}

SourceCodeErrorMessageList& FunctionDeclAnalysis::GetErrorMessage() { return sourceCodeErrorMessage; }

SourceCodeFunctionMessageMap& FunctionDeclAnalysis::GetFunctionMessage() { return sourceCodeFunctionMessage; };

} // namespace MFunction