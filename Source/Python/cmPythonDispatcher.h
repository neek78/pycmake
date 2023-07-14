/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonModBase.h"
#include "cmPythonTypes.h"
#include "cmState.h"

#include <pybind11/embed.h>

#include <string>
#include <optional>

class cmMakefile;
class cmPythonReturnParam;
struct cmListFileArgument;
class ArgTracker;

class cmPythonDispatcher : public cmPythonModBase 
{
public:
    cmPythonDispatcher(cmMakefile& makefile); 
    cmPythonDispatcher(const cmPythonDispatcher&) = delete; 
    cmPythonDispatcher& operator=(const cmPythonDispatcher&) = delete;

protected:
    using StrArgs = std::vector<std::string>;
    using LfArgs = std::vector<cmListFileArgument>;

    pybind11::object invokeFunction(const cmState::BuiltinCommand& function, 
            const std::string_view& fnName, 
            const pybind11::args& args, 
            const pybind11::kwargs& kwargs = pybind11::kwargs());

    pybind11::object invokeFunction(const cmState::Command& function, 
            const std::string_view& fnName,
        const pybind11::args& args, 
        const pybind11::kwargs& kwargs = pybind11::kwargs());

private:
    template<typename FnType, typename ArgsType>
    pybind11::object coreInvoke(
        const FnType& function, 
        const pybind11::args& originalArgs,
        const ArgsType& processedArgs,
        const ArgTracker& tracker);

    void ExpandArguments(const std::vector<std::string>& in,
                         std::vector<std::string>& outArgs) const;

    template<typename OutArgsType>
    void convertArgs(const pybind11::args& args, ArgTracker& tracker, OutArgsType& out);

    template<typename OutArgsType>
    void convertKwArgs(const pybind11::kwargs& kwargs, ArgTracker& tracker, OutArgsType& out);

    std::string convertArg(const pybind11::handle& arg, ArgTracker& tracker);
    std::string convertSimple(const pybind11::handle& arg, ArgTracker& tracker);

    bool TestForKwArg(const pybind11::handle& arg);

    void AddToOutArgs(const std::string& param, StrArgs& out);
    void AddToOutArgs(const std::string& param, LfArgs& out);

    template<typename T>
    std::string flattenCollection(const T& arg);

    pybind11::object BuildReturnValueTuple(
        const ArgTracker& tracker,
        const pybind11::args& originalArgs);

    pybind11::object BuildReturnValue(const cmPythonReturnParam& arg, const ArgTracker& tracker);

};
