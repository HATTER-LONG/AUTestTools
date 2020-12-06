#pragma once
#include "AnalysisMessage.h"
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

    std::string createMockSourceCode(const MyFunction::SourceCodeFunctionMessage&);
    std::string createMockSourceCode(const MyFunction::SourceCodeFunctionMessageMap&);

    std::string createUnitTestCode(const MyFunction::SourceCodeFunctionMessage&, const UnitTestInfo&);

private:
    void getParamList(std::string& functionParam, const MyFunction::FunctionParamList& info);
    void getFuncReturnValue(std::string& returnValue, const std::string& returnType);
    std::string getSectionCode(const MyFunction::SourceCodeFunctionMessage& Func, const UnitTestSectionInfo&);
};
} // namespace MyFunction