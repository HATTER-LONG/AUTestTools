#ifndef __FUNCTION_DECL_ANALYSIS_H__
#define __FUNCTION_DECL_ANALYSIS_H__
#include <map>
#include <string>
#include <vector>

class SourceCodeErrorMessage
{
public:
    SourceCodeErrorMessage(int errorlevel, std::string errormessage, std::string errorpos)
            : errorLevel(errorlevel), errorMessage(errormessage), errorPos(errorpos)
    {
    }
    int GetErrorLevel() { return errorLevel; }
    std::string GetErrorMessage() { return errorMessage; }
    std::string GetErrorPos() { return errorPos; }

private:
    int errorLevel;
    std::string errorMessage;
    std::string errorPos;
};

class SourceCodeFunctionMessage
{
public:
    SourceCodeFunctionMessage(std::string functionname, std::vector<std::string> functionparam)
            : functionName(functionname), functionParam(functionparam)
    {
    }
    void AddFunctionWhichCallExpr(std::string callexprname) { functionCallExpr.push_back(callexprname); }
    std::string GetFunctionName() { return functionName; }
    std::vector<std::string> GetFunctionParam() { return functionParam; }
    std::vector<std::string> GetFunctionWhichCallExpr() { return functionCallExpr; }

private:
    std::string functionName;
    std::vector<std::string> functionParam;
    std::vector<std::string> functionCallExpr;
};

using SourceCodeErrorMessageList = std::vector<SourceCodeErrorMessage>;
using SourceCodeFunctionMessageMap = std::map<std::string, SourceCodeFunctionMessage>;
class FunctionDeclAnalysis
{
public:
    FunctionDeclAnalysis(std::string filepath, std::string compiledatabase);

    bool StartToAnalysis();
    std::vector<SourceCodeErrorMessage> GetErrorMessage();
    std::map<std::string, SourceCodeFunctionMessage> GetFunctionMessage();

private:
    SourceCodeErrorMessageList sourceCodeErrorMessage;
    SourceCodeFunctionMessageMap sourceCodeFunctionMessage;

    std::string sourceCodeFilePath;
    std::string compiledDatabasePath;
};

#endif //__FUNCTION_DECL_ANALYSIS_H__