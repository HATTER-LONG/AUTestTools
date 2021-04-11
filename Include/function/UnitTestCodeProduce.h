#pragma once
#include "AnalysisMessage.h"

#include <string>
#include <vector>

namespace MyFunction
{

class UnitTestSectionInfo
{
public:
    std::string m_sectionName;
    std::string m_checkInfo;
};

class UnitTestInfo
{
public:
    std::string m_testName;
    std::vector<std::string> m_testTags;
    std::vector<UnitTestSectionInfo> m_testSection;
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
    void getParamList(std::string& FunctionParam, const MyFunction::FunctionParamList& Info);
    void getFuncReturnValue(std::string& ReturnValue, const std::string& ReturnType);
    std::string getSectionCode(const MyFunction::SourceCodeFunctionMessage& Func, const UnitTestSectionInfo&);
};
}   // namespace MyFunction