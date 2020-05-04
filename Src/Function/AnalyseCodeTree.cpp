#include "Function/AnalyseCodeTree.h"
#include "spdlog/spdlog.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include <clang/AST/DeclObjC.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/CommentOptions.h>
#include <clang/Frontend/FrontendActions.h>
#include <string>

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace spdlog;
static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");
class Func_Call : public MatchFinder::MatchCallback
{
public:
    void run(const MatchFinder::MatchResult& Result) override
    {
        if (auto const* functionDecl = Result.Nodes.getNodeAs<FunctionDecl>("FunctiondFeclWithCall"))
        {

            info("[{}] function name = {}\b", __FUNCTION__, functionDecl->getNumParams());
        }
        /*//输出函数名
        if (methodDecl->isInstanceMethod())
        {
            std::string methodName = (methodDecl->getSelector()).getAsString();
            info("[{}] - {}\b", __FUNCTION__, methodName.c_str());
        }
        else if (methodDecl->isClassMethod())
        {
            std::string methodName = (methodDecl->getSelector()).getAsString();
            info("[{}] + {}\b", __FUNCTION__, methodName.c_str());
        }

        //输出文件路径
        info("[{}] Path: {}\b", __FUNCTION__, rootPath.c_str());

        // 输出被调用函数
        const auto* funcCaller = Result.Nodes.getNodeAs<ObjCMessageExpr>("funcCaller");
        std::string selector = (funcCaller->getSelector()).getAsString();
        std::string className;
        if (funcCaller->isInstanceMessage())
        {
            className = funcCaller->getInstanceReceiver()->getType().getAsString();
        }
        else if (funcCaller->isClassMessage())
        {
            className = funcCaller->getClassReceiver().getAsString();
        }
        info("[{}] + [{}] + [{}]\b", __FUNCTION__, className.c_str(), selector.c_str());*/
    }
};

int FunctionToAnalyseCodeTree(int argc, const char** argv)
{
    auto FuncDeclMatcher =
        functionDecl(isExpansionInMainFile(), forEachDescendant(callExpr())).bind("FunctiondFeclWithCall");

    CommonOptionsParser OptionsParser(argc, argv, ToolingSampleCategory);
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    Func_Call FuncCall;
    MatchFinder Finder;
    Finder.addMatcher(FuncDeclMatcher, &FuncCall);
    return Tool.run(newFrontendActionFactory(&Finder).get());
}