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
inline SourceCodeFunctionMessageMap getHasBodyOfFunctionMap(
    const SourceCodeFunctionMessageMap& functionInfoMap)
{
    SourceCodeFunctionMessageMap functionMap;
    for (const auto& iter : functionInfoMap)
    {
        if (iter.second.getFunctionHasBodyInfo())
        {
            functionMap.insert(iter);
        }
    }
    return functionMap;
}

/**
 * @brief Get the No Has Body Of Functionmap object
 *
 * @param FunctionInfoMap
 * @return SourceCodeFunctionMessageMap
 */
inline SourceCodeFunctionMessageMap getNoHasBodyOfFunctionmap(
    const SourceCodeFunctionMessageMap& functionInfoMap)
{
    SourceCodeFunctionMessageMap functionMap;
    for (const auto& iter : functionInfoMap)
    {
        if (!iter.second.getFunctionHasBodyInfo())
        {
            functionMap.insert(iter);
        }
    }
    return functionMap;
}
}   // namespace MyFunction
