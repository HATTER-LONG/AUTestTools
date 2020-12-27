#pragma once

#include "AnalysisMessage.h"

namespace MyFunction
{

/**
 * @brief Get the Has Body Function Map object
 *
 * @param FunctionInfoMap
 * @return SourceCodeFunctionMessageMap
 */
inline auto getHasBodyOfFunctionMap(const SourceCodeFunctionMessageMap& FunctionInfoMap)
{
    SourceCodeFunctionMessageMap functionMap;
    for (const auto& iter : FunctionInfoMap)
    {
        if (iter.second.getFunctionHasBodyInfo()) { functionMap.insert(iter); }
    }
    return functionMap;
}

/**
 * @brief Get the No Has Body Of Functionmap object
 *
 * @param FunctionInfoMap
 * @return auto
 */
inline auto getNoHasBodyOfFunctionmap(const SourceCodeFunctionMessageMap& FunctionInfoMap)
{
    SourceCodeFunctionMessageMap functionMap;
    for (const auto& iter : FunctionInfoMap)
    {
        if (!iter.second.getFunctionHasBodyInfo()) { functionMap.insert(iter); }
    }
    return functionMap;
}
}   // namespace MyFunction