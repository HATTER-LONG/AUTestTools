#pragma once
#include "function/AnalysisMessage.h"
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
namespace MyFunction
{
class FunctionDefLister : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    // clang-format off
    auto matcher()
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

    FunctionDefLister(SourceCodeFunctionMessageMap& ref) : functionMessageRef(ref) {}


    void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override
    {
        clang::LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        clang::PrintingPolicy Policy(LangOpts);
        if (auto const* functionDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl"))
        {
            auto iter = functionMessageRef.find(functionDecl->getQualifiedNameAsString());
            if (iter == functionMessageRef.end())
            {
                std::string functionname;
                std::vector<std::string> functionparms;
                functionname = functionDecl->getQualifiedNameAsString();
                functionparms.push_back(functionDecl->getReturnType().getAsString());
                getParams(functionparms, functionDecl);

                iter = functionMessageRef
                           .insert(SourceCodeFunctionMessageMap::value_type(
                               functionname, SourceCodeFunctionMessage(functionname, functionparms)))
                           .first;
            }
            if (auto const* callexprdec = Result.Nodes.getNodeAs<clang::CallExpr>("callExprInfo"))
            {
                auto func = callexprdec->getDirectCallee();
                auto callexprIter = functionMessageRef.find(func->getQualifiedNameAsString());
                if (callexprIter == functionMessageRef.end())
                {
                    std::string functionname;
                    std::vector<std::string> functionparms;
                    functionname = func->getQualifiedNameAsString();
                    functionparms.push_back(func->getReturnType().getAsString());
                    getParams(functionparms, func);
                    functionMessageRef.insert(SourceCodeFunctionMessageMap::value_type(
                        functionname, SourceCodeFunctionMessage(functionname, functionparms)));
                }
                iter->second.AddFunctionWhichCallExpr(func->getQualifiedNameAsString());
            }
        }
    }


    void getParams(FunctionParamList& functionparms, const clang::FunctionDecl* func)
    {
        clang::LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        clang::PrintingPolicy Policy(LangOpts);
        for (unsigned int i = 0; i < func->getNumParams(); i++)
        {
            std::string paramwithname;
            auto param = func->getParamDecl(i);
            paramwithname += clang::QualType::getAsString(param->getType().split(), Policy);
            // paramwithname += "  ";
            // paramwithname += func->getParamDecl(i)->getNameAsString();
            functionparms.push_back(paramwithname);
        }
    }

private:
    SourceCodeFunctionMessageMap& functionMessageRef;
}; // namespace MyFunction;
} // namespace MyFunction