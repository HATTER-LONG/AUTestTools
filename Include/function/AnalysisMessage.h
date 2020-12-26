#pragma once

#include <map>
#include <string>
#include <vector>

namespace MyFunction
{
/**
 * @brief 记录源码分检测出的错误信息，信息为三个维度分别是【错误等级、错误信息、错误位置】
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
    SourceCodeErrorMessage(int Errorlevel, std::string Errormessage, std::string Errorpos)
            : ErrorLevel(Errorlevel)
            , ErrorMessage(Errormessage)
            , ErrorPos(Errorpos)
    {
    }

    /**
     * @brief Get the Error Level object
     *
     * @return int
     */
    int getErrorLevel() const noexcept { return ErrorLevel; }

    /**
     * @brief Get the Error Message object
     *
     * @return std::string
     */
    std::string getErrorMessage() const noexcept { return ErrorMessage; }

    /**
     * @brief Get the Error Pos object
     *
     * @return std::string
     */
    std::string getErrorPos() const noexcept { return ErrorPos; }

private:
    int ErrorLevel;
    std::string ErrorMessage;
    std::string ErrorPos;
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
    SourceCodeFunctionMessage(std::string Functionname, std::vector<std::string> Functionparam)
            : FunctionName(Functionname)
            , FunctionParam(Functionparam)
    {
    }
    /**
     * @brief 添加一个调用的函数信息
     *
     * @param callexprname
     */
    void addFunctionWhichCallExpr(std::string Callexprname) { FunctionCallExpr.push_back(Callexprname); }

    /**
     * @brief Get the Function Name object
     *
     * @return const std::string
     */
    const std::string getFunctionName() const noexcept { return FunctionName; }

    /**
     * @brief Get the Function Param object
     *
     * @return const FunctionParamList&
     */
    const FunctionParamList& getFunctionParam() const noexcept { return FunctionParam; }

    /**
     * @brief Get the Function Which Call Expr object
     *
     * @return const FunctionCallExprList&
     */
    const FunctionCallExprList& getFunctionWhichCallExpr() const noexcept { return FunctionCallExpr; }

private:
    std::string FunctionName;
    FunctionParamList FunctionParam;
    FunctionCallExprList FunctionCallExpr;
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
}   // namespace MyFunction