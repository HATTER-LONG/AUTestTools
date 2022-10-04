#pragma once
#include "SourceCodeMatcherBase.h"
#include "function/AnalysisMessage.h"

namespace MyFunction
{
class ClassMemberDefLister : public DeclarationMatcherCallBackBase
{
public:
    // clang-format off
    virtual clang::ast_matchers::DeclarationMatcher matcher() override
    {
        using namespace clang::ast_matchers;
        return cxxMethodDecl(
                    unless(isExpansionInSystemHeader()),
                    matchesName(m_strClassName))
                .bind("ClassMethodDef");
    }
    // clang-format on

    ClassMemberDefLister(SourceCodeFunctionMessageMap& ref)
            : m_functionMessageRef(ref)
    {
    }

    virtual void run(
        const clang::ast_matchers::MatchFinder::MatchResult& result) override
    {
        if (auto const* functionDecl =
                result.Nodes.getNodeAs<clang::FunctionDecl>("ClassMethodDef"))
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
        }
    }

    bool config(const ConfigInfo& config) override
    {
        try
        {
            config.at("ClassName").get_to(m_strClassName);
        }
        catch (nlohmann::json::exception& a)
        {
            spdlog::error("error config info  {}", a.what());
            return false;
        }
        return true;
    }

private:
    SourceCodeFunctionMessageMap::iterator insertFuncToMapRef(
        const clang::FunctionDecl* func)
    {
        std::string functionname;
        std::vector<std::string> functionparms;
        functionname = func->getQualifiedNameAsString();
        functionparms.push_back(func->getReturnType().getAsString(m_policy));
        getParams(functionparms, func);
        bool const hasbody = func->isExplicitlyDefaulted() || func->hasBody()
                             || func->isDefaulted();
        return m_functionMessageRef
            .insert(SourceCodeFunctionMessageMap::value_type(functionname,
                SourceCodeFunctionMessage(functionname, functionparms, hasbody,
                    SourceCodeFunctionMessage::FUNCTYPE::CXXMEMBER)))
            .first;
    }

private:
    SourceCodeFunctionMessageMap& m_functionMessageRef;
    std::string m_strClassName;
};

}   // namespace MyFunction
