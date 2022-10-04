#pragma once
#include "SourceCodeMatcherBase.h"
#include "function/AnalysisMessage.h"

namespace MyFunction
{
class FunctionDefLister : public DeclarationMatcherCallBackBase
{
public:
    // clang-format off
    virtual clang::ast_matchers::DeclarationMatcher matcher() override
    {
        using namespace clang::ast_matchers;
        return functionDecl(
                    isDefinition(),
                    unless(isExpansionInSystemHeader()),
                    forEachDescendant(callExpr().bind("callExprInfo"))
                )
                .bind("FunctionDecl");
    }
    // clang-format on

    FunctionDefLister(SourceCodeFunctionMessageMap& ref)
            : m_functionMessageRef(ref)
    {
    }

    virtual void run(
        const clang::ast_matchers::MatchFinder::MatchResult& result) override
    {
        if (auto const* functionDecl =
                result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl"))
        {
            auto iter = m_functionMessageRef.find(
                functionDecl->getQualifiedNameAsString());
            if (iter == m_functionMessageRef.end())
            {
                std::string functionname;
                std::vector<std::string> functionparms;
                functionname = functionDecl->getQualifiedNameAsString();
                functionparms.push_back(
                    functionDecl->getReturnType().getAsString(m_policy));
                getParams(functionparms, functionDecl);
                iter = insertFuncToMapRef(functionDecl);
            }
            if (auto const* callexprdec =
                    result.Nodes.getNodeAs<clang::CallExpr>("callExprInfo"))
            {
                if (const auto* func = callexprdec->getDirectCallee())
                {
                    auto callexprIter = m_functionMessageRef.find(
                        func->getQualifiedNameAsString());
                    if (callexprIter == m_functionMessageRef.end())
                    {
                        insertFuncToMapRef(func);
                    }
                    iter->second.addFunctionWhichCallExpr(
                        func->getQualifiedNameAsString());
                }
            }
        }
    }
    bool config(const ConfigInfo& /*Config*/) override { return true; }

private:
    SourceCodeFunctionMessageMap::iterator insertFuncToMapRef(
        const clang::FunctionDecl* func)
    {
        std::string functionname;
        std::vector<std::string> functionparms;
        functionname = func->getQualifiedNameAsString();
        functionparms.push_back(func->getReturnType().getAsString(m_policy));
        getParams(functionparms, func);
        return m_functionMessageRef
            .insert(SourceCodeFunctionMessageMap::value_type(functionname,
                SourceCodeFunctionMessage(functionname, functionparms,
                    func->hasBody(),
                    func->isCXXClassMember()
                        ? SourceCodeFunctionMessage::FUNCTYPE::CXXMEMBER
                        : SourceCodeFunctionMessage::FUNCTYPE::CTYPE)))
            .first;
    }


private:
    SourceCodeFunctionMessageMap& m_functionMessageRef;
};

}   // namespace MyFunction
