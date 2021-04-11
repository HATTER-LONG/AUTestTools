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

    FunctionDefLister(SourceCodeFunctionMessageMap& Ref)
            : m_functionMessageRef(Ref)
    {
    }


    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override
    {
        clang::LangOptions langOpts;
        langOpts.CPlusPlus = true;
        clang::PrintingPolicy policy(langOpts);
        if (auto const* functionDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl"))
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
            if (auto const* callexprdec = Result.Nodes.getNodeAs<clang::CallExpr>("callExprInfo"))
            {
                if (const auto* func = callexprdec->getDirectCallee())
                {
                    auto callexprIter = m_functionMessageRef.find(func->getQualifiedNameAsString());
                    if (callexprIter == m_functionMessageRef.end())
                    {
                        insertFuncToMapRef(func);
                    }
                    iter->second.addFunctionWhichCallExpr(func->getQualifiedNameAsString());
                }
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
                functionname, SourceCodeFunctionMessage(functionname, functionparms, Func->hasBody(),
                                  Func->isCXXClassMember() ? SourceCodeFunctionMessage::FUNCTYPE::CXXMEMBER
                                                           : SourceCodeFunctionMessage::FUNCTYPE::CTYPE)))
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
};

}   // namespace MyFunction