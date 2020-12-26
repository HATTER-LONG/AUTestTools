#pragma once
#include "AnalysisMessage.h"

#include <string>
#include <vector>

namespace MyFunction
{

class UnitTestSectionInfo
{
public:
    std::string SectionName;
    std::string CheckInfo;
};

class UnitTestInfo
{
public:
    std::string TestName;
    std::vector<std::string> TestTags;
    std::vector<UnitTestSectionInfo> TestSection;
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