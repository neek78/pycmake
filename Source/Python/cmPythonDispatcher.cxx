/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonDispatcher.h"
#include "cmPythonModules.h"
#include "cmPythonReturnParam.h"
#include "cmPythonKwArg.h"
#include "cmPythonTypeConversion.h"
#include "cmPythonUtils.h"
#include "cmPythonExpander.h"
#include "cmArgTracker.h"

#include "cmMakefile.h"
#include "cmListFileCache.h"
#include "cmExecutionStatus.h"
#include "cmake.h"

#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include <pybind11/stl.h>

namespace py = pybind11;

cmPythonDispatcher::cmPythonDispatcher(cmMakefile& makefile)
: cmPythonModBase(makefile)
{
}

py::object cmPythonDispatcher::invokeFunction(
        const cmState::BuiltinCommand& function, 
        const std::string_view& fnName, 
        int lineNum,
        int endLineNum,
        const py::args& args, 
        const py::kwargs& kwargs)
{
    return invokeFunctionInternal<StrArgs>(function, fnName, lineNum, endLineNum, args, kwargs);
}

py::object cmPythonDispatcher::invokeFunction(
        const cmState::Command& function, 
        const std::string_view& fnName, 
        int lineNum,
        int endLineNum,
        const py::args& args, 
        const py::kwargs& kwargs)
{
    return invokeFunctionInternal<LfArgs>(function, fnName, lineNum, endLineNum, args, kwargs);
}

template<typename ArgsType, typename FnType>
py::object cmPythonDispatcher::invokeFunctionInternal(
        const FnType& function, 
        const std::string_view& fnName, 
        int lineNum,
        int endLineNum,
        const py::args& args, 
        const py::kwargs& kwargs)
{
    ArgTracker tracker(fnName);
    LfArgs unexpandedArgs;
    ArgsType expandedArgs;

    convertArgs(args, tracker, unexpandedArgs, expandedArgs);
    convertKwArgs(kwargs, tracker, unexpandedArgs, expandedArgs);

    return coreInvoke(function, fnName, lineNum, endLineNum, args, unexpandedArgs, expandedArgs, tracker);
}

template<typename OutArgsType>
void cmPythonDispatcher::convertArgs(
        const py::args& args, 
        ArgTracker& tracker, 
        LfArgs& unexpandedArgs,
        OutArgsType& expandedArgs)
{
    // Convert and expand each arg from python to a native representation
    // Either of these operations might throw... 
    for(const auto& a : args) {
        auto converted = convertArg(a, tracker);
        ExpandAndAddToOutArgs(converted, unexpandedArgs, expandedArgs, true);
    }
}

template<typename OutArgsType>
void cmPythonDispatcher::convertKwArgs(const pybind11::kwargs& kwargs, 
        ArgTracker& tracker, 
        LfArgs& unexpandedArgs,
        OutArgsType& expandedArgs)
{
    using namespace literals;

    for(const auto& a: kwargs) {
        // cm_dir and cm_global are reserved for our own objects.. 
        py::str name = py::str(a.first);
        if (name.equal("cm_dir"_pys) || name.equal("cm_global"_pys)) {
            py::str err = "kwarg "_pys + name + " is a reserved name"_pys;
            throw py::value_error(err); 
        }

        bool convertValue = true;

        if (py::isinstance(a.second, cmPythonModules::GetTypeKwArg())) {
            bool include = TestForKwArg(a.second);
            if (!include) {
                continue;
            }
            convertValue = false;
        }

        // increase arg count for keyname
        tracker.IncArgNum();
        //AddToOutArgs(name, expandedArgs);
        ExpandAndAddToOutArgs(name, unexpandedArgs, expandedArgs, false);

        if (!convertValue) {
            continue;
        }

        auto convertedArg = convertArg(a.second, tracker);
        ExpandAndAddToOutArgs(convertedArg, unexpandedArgs, expandedArgs, true);
        // std::cout << "KWARG " << py::str(name) << " value " << py::str(value) << "\n";
    }
}

void cmPythonDispatcher::ExpandAndAddToOutArgs(const std::string& param, 
        LfArgs& unexpandedArgs, StrArgs& expandedArgs, bool expand)
{
    unexpandedArgs.emplace_back(param, cmListFileArgument::Delimiter::Unquoted, 0);
    if (expand) {
        expandedArgs.emplace_back(ExpandVar(GetMakefile(), param, false));
    } else {
        expandedArgs.emplace_back(param);
    }
}

void cmPythonDispatcher::ExpandAndAddToOutArgs(const std::string& param, 
        LfArgs& unexpandedArgs, LfArgs& expandedArgs, bool expand)
{
    unexpandedArgs.emplace_back(param, cmListFileArgument::Delimiter::Unquoted, 0);

    if (expand) {
        expandedArgs.emplace_back(ExpandVar(GetMakefile(), param, false), 
                cmListFileArgument::Delimiter::Unquoted, 0);
    } else {
        expandedArgs.emplace_back(param, cmListFileArgument::Delimiter::Unquoted, 0);
    }
}

bool cmPythonDispatcher::TestForKwArg(const py::handle& arg)
{
    assert(py::isinstance(arg, cmPythonModules::GetTypeKwArg()));

    // build a c++ wrapper around the python KwArg object
    cmPythonKwArg kwArg(arg);

    return kwArg.GetEnabled();
}

std::string cmPythonDispatcher::convertArg(const py::handle& arg, ArgTracker& tracker)
{
    if(py::isinstance(arg, cmPythonModules::GetTypeKwArg())) {
        throw py::type_error("KwArg cannot be used as a positional argument");
    }

    tracker.IncArgNum();

    // firstly, check if we've got a ReturnParam object
    if(py::isinstance(arg, cmPythonModules::GetTypeReturnParam())) {
        // ok, we do.
        // Do checks and substitute its variable name into the argument list

        // build a c++ wrapper around the python ReturnParam object - this is short lived
        // just to provide a more convenient interface
        cmPythonReturnParam param(arg);

        param.SetArgPosition(tracker.GetArgCount());

        // catch stupidity - check the value actually set, and we're getting the same object back
        assert(param.GetArgPosition());

        tracker.IncRetParamCount();
        
        // should we refuse to overwrite an existing value?
        if(param.GetRaiseOnOverwrite()) {
            const auto& val = GetMakefile().GetDefinition(param.GetName());
            if(!!val) {
                // variable is already set... blow up
                std::stringstream ss;
                ss << "variable " << param.GetName() <<
                    " is already set (containing value " << *val
                    << ") and may be overwritten by call to function "
                    << tracker.GetFnName();
                throw py::value_error(ss.str());
            }
        }
        return param.GetName();
    }

    // ok, just do regular conversion
    return convertSimple(arg, tracker);
}

std::string cmPythonDispatcher::convertSimple(const py::handle& arg, ArgTracker& tracker)
{
    try {
        return ConvertArgPythonToCMake(arg);
    }
    catch(const std::exception& e) {
        // give a nicer error than the default exception
        std::stringstream ss;
        ss << "could not convert argument " << tracker.GetArgCount() <<
            " from " << arg.get_type() << 
            " to string in call to cmake function '" << tracker.GetFnName()
            << "' - " << e.what();
        throw py::type_error(ss.str());
    }
}

void cmPythonDispatcher::DoTrace(
        const std::string_view& fnName,
        int lineNum,
        int endLineNum,
        const LfArgs& unexpandedArgs)
{
    // trace enabled? 
    if (!GetMakefile().GetCMakeInstance()->GetTrace()) {
        return;
    }

    GetMakefile().PrintCommandTrace(FindCallSite(unexpandedArgs), 
            GetMakefile().GetBacktrace());
}

template<typename FnType, typename ArgsType>
py::object cmPythonDispatcher::coreInvoke(
        const FnType& function, 
        const std::string_view& fnName,
        int lineNum,
        int endLineNum,
        const py::args& originalArgs,
        const LfArgs& unexpandedArgs,
        const ArgsType& processedArgs,
        const ArgTracker& tracker)
{
    assert(function);

    cmExecutionStatus status(GetMakefile()); 

    // sometimes fatal error's already been set before we're invoked - 
    // don't blow up again because of it
    bool alreadyFatal = cmSystemTools::GetFatalErrorOccurred();

    DoTrace(fnName, lineNum, endLineNum, unexpandedArgs);
    
    // fire off the command itself
    bool invokeSucceeded = function(processedArgs, status);

    bool fatal = cmSystemTools::GetFatalErrorOccurred();
    bool nestedErr = status.GetNestedError();

    if (fatal && !alreadyFatal) {
        const std::string error = "Fatal error during command execution - " + 
            std::string(tracker.GetFnName()) + " - " + status.GetError();
        throw std::runtime_error(error);
    }

    if (!invokeSucceeded || nestedErr) {
        // FIXME: msg could be better
        const std::string error = "Error during command execution - " + 
            std::string(tracker.GetFnName()) + " - " + status.GetError();
        throw std::runtime_error(error);
    }

    return BuildReturnValueTuple(tracker, originalArgs);
}

py::object cmPythonDispatcher::BuildReturnValueTuple(
        const ArgTracker& tracker,
        const py::args& originalArgs)
{
    // caller did't add any ReturnParam object, we're done here - no return values
    if (tracker.GetRetParamCount() == 0) {
        return py::none();
    }
        
    // extract return values and possibly cleanup
    py::tuple ret = py::tuple(tracker.GetRetParamCount());
    std::size_t pNum = 0;

    py::type retParamType = cmPythonModules::GetTypeReturnParam();
    for(const auto& arg : originalArgs) {
        if(!py::isinstance(arg, retParamType)) {
            // not a return value, nothing to do
            continue;
        }

        assert(pNum < ret.size());
        cmPythonReturnParam param(arg);

        // arg pos should be set during call setup (convertArg())
        assert(param.GetArgPosition());

        ret[pNum] = BuildReturnValue(param, tracker);

        pNum++;
    }

    // check we found all expected ReturnParam objects
    assert(pNum == tracker.GetRetParamCount());

    // should not have gotten here if no return vars
    assert(!ret.empty());

    // don't return a tuple for a single value
    if (ret.size() == 1) {
        return ret[0];
    }

    return ret;
}

py::object cmPythonDispatcher::BuildReturnValue(const cmPythonReturnParam& arg, 
        const ArgTracker& tracker)
{
    const std::string& key = arg.GetName();
    const auto& val = GetMakefile().GetDefinition(key);

    py::object ret;

    // is the value set in cmake vars?
    if (!!val) {
        ret = ConvertArgCMakeToPython(val);

        // remove value from cmake vars?
        if(arg.GetAutoCleanup()) {
            GetMakefile().RemoveDefinition(key);
        }
    } else {
        // value not set.. how to handle this?
        if (arg.ShouldRaiseOnUnset()) {
            // arg pos should be set during call setup (convertArg())
            assert(arg.GetArgPosition());

            std::stringstream ss;
            ss << "return value for parameter " << arg.GetName() << 
                " (position " << *arg.GetArgPosition() <<
                ") was not set by function " << tracker.GetFnName();
            
            throw std::invalid_argument(ss.str());
        }

        // Return a default value?
        if (arg.GetDefaultValue()) {
            ret = py::str(*arg.GetDefaultValue());
        } else {
            // ... nope
            ret = py::none();
        }
    }

    return ret;
}

#if 0
const cmPythonDispatcher::LfArgs& cmPythonDispatcher::ConvertToLfArgs(const LfArgs& args)
{
    return args;
}

cmPythonDispatcher::LfArgs cmPythonDispatcher::ConvertToLfArgs(const StrArgs& args)
{
    LfArgs a;
    for(const auto& i : args) {
        AddToOutArgs(i, a);
    }
    return a;
}
#endif

cmListFileFunction cmPythonDispatcher::FindCallSite(const LfArgs& unprocessedArgs)
{
    py::module_ inspect = cmPythonModules::GetModuleInspect();
    py::object frame = inspect.attr("currentframe")();
    int i =0;
    while(!frame.is_none()) {
        py::object code = frame.attr("f_code");
        int lineNo = frame.attr("f_lineno").cast<int>();
        py::str name = code.attr("co_qualname");
        py::str filename = code.attr("co_filename");
        std::cout << "PYFRAME " << name << " line " << lineNo << " " << filename << "\n";
        if (i==1) {
            return cmListFileFunction(std::string(name), lineNo, lineNo, unprocessedArgs);
        }

        i++;
        frame = frame.attr("f_back");
    }
    return cmListFileFunction("unknown", 0, 0, LfArgs());
}
