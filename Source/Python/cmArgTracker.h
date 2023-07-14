/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

class ArgTracker;

// FIXME: uses param and arg interchangably for no good reason
class ArgTracker
{
public:
    ArgTracker(const std::string_view& fnName)
    :   FnName(fnName)
    {
    }

    // this is the arg number from cmake's POV - ie kwargs add two to this
    // per argument
    void IncArgNum() { ArgNum++; }

    // number of return values we've detected
    void IncRetParamCount() { RetParamCount++; }

    std::size_t GetArgCount() const { return ArgNum; }
    std::size_t GetRetParamCount() const { return RetParamCount; }
    const std::string_view& GetFnName() const { return FnName; }

private:
    std::size_t ArgNum = 0;
    std::size_t RetParamCount = 0;
    const std::string_view FnName;
};
