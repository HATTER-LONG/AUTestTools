#include "SourceCodeMessageAnalysis.h"
#include "spdlog/spdlog.h"

#include <algorithm>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

static llvm::cl::OptionCategory fltCat("func-decl-list-am");
namespace MyFunction
{

void SourceCodeMessageAnalysis::paramInitialize(
    std::vector<std::string>& ParamList)
{
    ParamList.emplace_back(std::string("__READY_TO_ANALYSIS__"));
    ParamList.emplace_back(m_sourceCodeFilePath);
    if (!m_compiledDatabasePath.empty())
    {
        std::string tmpstr = "-p=" + m_compiledDatabasePath;
        ParamList.emplace_back(tmpstr);
    }
}

SourceCodeMessageAnalysis::CommonOptionsParserExpected
    SourceCodeMessageAnalysis::getCommonOptionsParser()
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
    return clang::tooling::CommonOptionsParser::create(argc, argv, fltCat);
}

bool SourceCodeMessageAnalysis::startToAnalysisSourceCode(
    SourceCodeFunctionMessageMap& Functionmessage,
    SourceCodeErrorMessageList& Errormessage)
{
    return startToAnalysisSourceCode(Errormessage);
}

bool SourceCodeMessageAnalysis::startToAnalysisSourceCode(
    SourceCodeErrorMessageList& ErrorMessage)
{
    std::vector<std::string> sourceCodePathList;
    sourceCodePathList.emplace_back(m_sourceCodeFilePath);

    auto commonOptionsParserVar = getCommonOptionsParser();
    if (!commonOptionsParserVar)
    {
        llvm::outs() << llvm::toString(commonOptionsParserVar.takeError());
        return false;
    }
    clang::tooling::ClangTool tool(
        commonOptionsParserVar->getCompilations(), sourceCodePathList);

    clang::ast_matchers::MatchFinder finder;
    if (m_matcherWithCallBack != nullptr
        && m_matcherWithCallBack->config(m_jsonConfigToMatcher))
    {
        finder.addMatcher(
            m_matcherWithCallBack->matcher(), m_matcherWithCallBack.get());
    }
    else
    {
        spdlog::error("No matcher has set to analysis source code or config "
                      "failed, please check it !!!");
        spdlog::error(
            "Config Info is \n{}", m_jsonConfigToMatcher.dump(1, '\t'));
        return false;
    }

    SourceCodeErrorAnalysis diagnosticConsumer(ErrorMessage);
    tool.setDiagnosticConsumer(&diagnosticConsumer);
    int ret = tool.run(clang::tooling::newFrontendActionFactory(&finder).get());
    if (ret != 0)
    {
        spdlog::error("[{}] error for analysis file[{}] errnocode[{}]",
            __FUNCTION__, m_sourceCodeFilePath, ret);
        return false;
    }
    return true;
}
}   // namespace MyFunction
