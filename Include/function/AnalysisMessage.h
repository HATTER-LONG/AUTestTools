#pragma once

#include <map>
#include <string>
#include <vector>

namespace MyFunction
{

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

using FunctionParamList = std::vector<std::string>;
using FunctionCallExprList = std::vector<std::string>;

class SourceCodeFunctionMessage
{
public:
    SourceCodeFunctionMessage(std::string functionname, std::vector<std::string> functionparam)
            : functionName(functionname), functionParam(functionparam)
    {
    }
    void AddFunctionWhichCallExpr(std::string callexprname) { functionCallExpr.push_back(callexprname); }
    const std::string GetFunctionName() const { return functionName; }
    const FunctionParamList& GetFunctionParam() const { return functionParam; }
    const FunctionCallExprList& GetFunctionWhichCallExpr() const { return functionCallExpr; }

private:
    std::string functionName;
    FunctionParamList functionParam;
    FunctionCallExprList functionCallExpr;
};

using SourceCodeErrorMessageList = std::vector<SourceCodeErrorMessage>;
using SourceCodeFunctionMessageMap = std::map<std::string, SourceCodeFunctionMessage>;
} // namespace MyFunction