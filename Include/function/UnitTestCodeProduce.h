#pragma once
#include "FunctionDeclAnalysis.h"
#include <string>
#include <vector>

namespace MyFunction
{

class UnitTestCodeProduceFunc
{
public:
    UnitTestCodeProduceFunc() = default;
    ~UnitTestCodeProduceFunc() = default;

    std::string createMockSourceCode(const MFunction::SourceCodeFunctionMessage&);
    std::string createMockSourceCode(const MFunction::SourceCodeFunctionMessageMap&);

    std::string createUnitTestCode(std::string testName, std::string Tag);
    std::string createUnitTestSectionCode(std::string sectionName, std::string checkInfo);

private:
    void getParamList(std::string& functionParam, const MFunction::FunctionParamList& info);
    void getFuncReturnValue(std::string& returnValue, const std::string& returnType);
};
} // namespace MyFunction