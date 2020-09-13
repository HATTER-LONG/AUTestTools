#pragma once
#include "FunctionDeclAnalysis.h"
#include <string>
#include <vector>

namespace MyFunction
{

class UnitTestSectionInfo
{
public:
    std::string sectionName;
    std::string checkInfo;
};

class UnitTestInfo
{
public:
    std::string testName;
    std::vector<std::string> testTags;
    std::vector<UnitTestSectionInfo> testSection;
};

class UnitTestCodeProduceFunc
{
public:
    UnitTestCodeProduceFunc() = default;
    ~UnitTestCodeProduceFunc() = default;

    std::string createMockSourceCode(const MFunction::SourceCodeFunctionMessage&);
    std::string createMockSourceCode(const MFunction::SourceCodeFunctionMessageMap&);

    std::string createUnitTestCode(const MFunction::SourceCodeFunctionMessage&, const UnitTestInfo&);

private:
    void getParamList(std::string& functionParam, const MFunction::FunctionParamList& info);
    void getFuncReturnValue(std::string& returnValue, const std::string& returnType);
    std::string getSectionCode(const MFunction::SourceCodeFunctionMessage& Func, const UnitTestSectionInfo&);
};
} // namespace MyFunction