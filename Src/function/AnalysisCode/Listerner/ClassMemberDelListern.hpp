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

    ClassMemberDefLister(SourceCodeFunctionMessageMap& Ref, std::string ClassName)
            : m_functionMessageRef(Ref)
            , m_strClassName(ClassName)
    {
    }

    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override
    {
        clang::LangOptions langOpts;
        langOpts.CPlusPlus = true;
        clang::PrintingPolicy policy(langOpts);
        if (auto const* functionDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("ClassMethodDef"))
        {
            auto iter = m_functionMessageRef.find(functionDecl->getQualifiedNameAsString());
            if (iter == m_functionMessageRef.end())
            {
                std::string functionname;
                std::vector<std::string> functionparms;
                functionname = functionDecl->getQualifiedNameAsString();
                functionparms.push_back(functionDecl->getReturnType().getAsString());
                getParams(functionparms, functionDecl);
                iter = insertFuncToMapRef(functionDecl);
            }
        }
    }

    SourceCodeFunctionMessageMap::iterator insertFuncToMapRef(const clang::FunctionDecl* Func)
    {
        std::string functionname;
        std::vector<std::string> functionparms;
        functionname = Func->getQualifiedNameAsString();
        functionparms.push_back(Func->getReturnType().getAsString());
        getParams(functionparms, Func);
        return m_functionMessageRef
            .insert(SourceCodeFunctionMessageMap::value_type(
                functionname, SourceCodeFunctionMessage(
                                  functionname, functionparms, Func->hasBody(), SourceCodeFunctionMessage::FUNCTYPE::CXXMEMBER)))
            .first;
    }

    void getParams(FunctionParamList& Functionparms, const clang::FunctionDecl* Func)
    {
        clang::LangOptions langOpts;
        langOpts.CPlusPlus = true;
        clang::PrintingPolicy policy(langOpts);
        for (unsigned int i = 0; i < Func->getNumParams(); i++)
        {
            std::string paramwithname;
            const auto* param = Func->getParamDecl(i);
            paramwithname += clang::QualType::getAsString(param->getType().split(), policy);
            // paramwithname += "  ";
            // paramwithname += func->getParamDecl(i)->getNameAsString();
            Functionparms.push_back(paramwithname);
        }
    }

private:
    SourceCodeFunctionMessageMap& m_functionMessageRef;
    std::string m_strClassName;
};

}   // namespace MyFunction