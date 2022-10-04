#pragma once

#include <map>
#include <string>
#include <vector>

namespace MyFunction
{
/**
 * @brief
 * 记录源码分检测出的错误信息，信息为三个维度分别是【错误等级、错误信息、错误位置】
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
    SourceCodeErrorMessage(
        int errorlevel, std::string errormessage, std::string errorpos)
            : m_errorLevel(errorlevel)
            , m_errorMessage(errormessage)
            , m_errorPos(errorpos)
    {
    }

    /**
     * @brief Get the Error Level object
     *
     * @return int
     */
    int getErrorLevel() const noexcept { return m_errorLevel; }

    /**
     * @brief Get the Error Message object
     *
     * @return std::string
     */
    std::string getErrorMessage() const noexcept { return m_errorMessage; }

    /**
     * @brief Get the Error Pos object
     *
     * @return std::string
     */
    std::string getErrorPos() const noexcept { return m_errorPos; }

private:
    int m_errorLevel;
    std::string m_errorMessage;
    std::string m_errorPos;
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
// TODO:
// 函数信息设计存在扩展性缺陷，如果功能需要更多的函数信息，则必须修改所有解析方法来进行支持，可以通过结构体或
// JSON 字段扩展
class SourceCodeFunctionMessage
{
public:
    enum FUNCTYPE
    {
        CXXMEMBER = 0,   // c++ 类成员
        CTYPE,           // C 函数
        TOTAL
    };
    /**
     * @brief Construct a new Source Code Function Message object
     *
     * @param Functionname
     * @param Functionparam
     * @param HasFuncBody
     */
    SourceCodeFunctionMessage(std::string functionname,
        std::vector<std::string> functionparam, bool hasFuncBody,
        FUNCTYPE funcType)
            : m_functionName(functionname)
            , m_functionParam(functionparam)
            , m_hasBody(hasFuncBody)
            , m_funcType(funcType)
    {
    }
    /**
     * @brief 添加一个调用的函数信息
     *
     * @param callexprname
     */
    void addFunctionWhichCallExpr(std::string callexprname)
    {
        m_functionCallExpr.push_back(callexprname);
    }

    /**
     * @brief Get the Function Name object
     *
     * @return const std::string
     */
    const std::string getFunctionName() const noexcept
    {
        return m_functionName;
    }

    /**
     * @brief Get the Function Param object
     *
     * @return const FunctionParamList&
     */
    const FunctionParamList& getFunctionParam() const noexcept
    {
        return m_functionParam;
    }

    /**
     * @brief Get the Function Which Call Expr object
     *
     * @return const FunctionCallExprList&
     */
    const FunctionCallExprList& getFunctionWhichCallExpr() const noexcept
    {
        return m_functionCallExpr;
    }

    /**
     * @brief Get the Function Has Body Info object
     *
     * @return true Has Function Body
     * @return false  None Function Body
     */
    const bool getFunctionHasBodyInfo() const noexcept { return m_hasBody; }

    /**
     * @brief Get the Function Type
     *
     * @return FUNCTYPE CType or CxxMember
     */
    const FUNCTYPE getFunctionType() const noexcept { return m_funcType; };

private:
    std::string m_functionName;
    FunctionParamList m_functionParam;
    FunctionCallExprList m_functionCallExpr;
    bool m_hasBody;
    FUNCTYPE m_funcType;
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
using SourceCodeFunctionMessageMap =
    std::map<std::string, SourceCodeFunctionMessage>;
}   // namespace MyFunction
