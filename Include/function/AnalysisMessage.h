#pragma once

#include <map>
#include <string>
#include <vector>

namespace MyFunction
{
/**
 * @brief 记录源码分检测出的错误信息，信息为三个维度分别是 【错误等级、错误信息、错误位置】
 *
 */
class SourceCodeErrorMessage
{
public:
    /**
     * @brief Construct a new Source Code Error Message object
     *
     * @param errorlevel
     * @param errormessage
     * @param errorpos
     */
    SourceCodeErrorMessage(int errorlevel, std::string errormessage, std::string errorpos)
            : errorLevel(errorlevel), errorMessage(errormessage), errorPos(errorpos)
    {
    }

    /**
     * @brief Get the Error Level object
     *
     * @return int
     */
    int GetErrorLevel() const noexcept { return errorLevel; }

    /**
     * @brief Get the Error Message object
     *
     * @return std::string
     */
    std::string GetErrorMessage() const noexcept { return errorMessage; }

    /**
     * @brief Get the Error Pos object
     *
     * @return std::string
     */
    std::string GetErrorPos() const noexcept { return errorPos; }

private:
    int errorLevel;
    std::string errorMessage;
    std::string errorPos;
};
/**
 * @brief 记录函数方法参数信息列表
 *
 */
using FunctionParamList = std::vector<std::string>;

/**
 * @brief 记录函数方法调用的其他方法列表
 *
 */
using FunctionCallExprList = std::vector<std::string>;

/**
 * @brief 函数方法信息类
 *
 */
class SourceCodeFunctionMessage
{
public:
    /**
     * @brief Construct a new Source Code Function Message object
     *
     * @param functionname
     * @param functionparam
     */
    SourceCodeFunctionMessage(std::string functionname, std::vector<std::string> functionparam)
            : functionName(functionname), functionParam(functionparam)
    {
    }
    /**
     * @brief 添加一个调用的函数信息
     *
     * @param callexprname
     */
    void AddFunctionWhichCallExpr(std::string callexprname) { functionCallExpr.push_back(callexprname); }

    /**
     * @brief Get the Function Name object
     *
     * @return const std::string
     */
    const std::string GetFunctionName() const noexcept { return functionName; }

    /**
     * @brief Get the Function Param object
     *
     * @return const FunctionParamList&
     */
    const FunctionParamList& GetFunctionParam() const noexcept { return functionParam; }

    /**
     * @brief Get the Function Which Call Expr object
     *
     * @return const FunctionCallExprList&
     */
    const FunctionCallExprList& GetFunctionWhichCallExpr() const noexcept { return functionCallExpr; }

private:
    std::string functionName;
    FunctionParamList functionParam;
    FunctionCallExprList functionCallExpr;
};

/**
 * @brief 源码中分析出的所有错误信息
 *
 */
using SourceCodeErrorMessageList = std::vector<SourceCodeErrorMessage>;
/**
 * @brief 源码中所有目标函数信息列表
 *
 */
using SourceCodeFunctionMessageMap = std::map<std::string, SourceCodeFunctionMessage>;
} // namespace MyFunction