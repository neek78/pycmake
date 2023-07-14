/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonPyFunctions.h"
#include "cmPythonModules.h"

#include "cmMakefile.h"
#include "cmExecutionStatus.h"
#include "cmPythonFactory.h"
#include "cmPythonCore.h"
#include "cmPythonTypeConversion.h"

#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

namespace {

const char* USER_FUNCTIONS_LIST = "__userfunctions";

}

void cmPythonPyFunctions::bind(py::module_& m)
{
    auto cls = pybind11::class_<cmPythonPyFunctions, cmPythonDictInterface>(m, "PyFunctions")
        .def("add", py::overload_cast<const py::function&>
               (&cmPythonPyFunctions::Add), "add python function")
        .def("add", py::overload_cast<const std::string &, const py::function&>
                (&cmPythonPyFunctions::Add), "add python function with a specified name");
        //.def("delete", &cmPythonPyFunctions::Delete)

    cls.attr(USER_FUNCTIONS_LIST) = py::dict();
}

cmPythonPyFunctions::cmPythonPyFunctions(cmMakefile& makefile)
:   cmPythonModBase(makefile)
{
}

py::type cmPythonPyFunctions::GetClassObject()
{
    return py::type::of<cmPythonPyFunctions>();
}

py::type cmPythonPyFunctions::GetClassObject() const
{
    return py::type::of<cmPythonPyFunctions>();
}

pybind11::dict cmPythonPyFunctions::GetUserFunctions()
{
    return GetClassObject().attr(USER_FUNCTIONS_LIST);
}

pybind11::dict cmPythonPyFunctions::GetUserFunctions() const
{
    return GetClassObject().attr(USER_FUNCTIONS_LIST);
}

bool cmPythonPyFunctions::Basecamp(
        const py::function& fn, 
        const cmPolicies::PolicyMap& policies,
        const std::vector<cmListFileArgument>& args, 
        cmExecutionStatus& status)
{
    std::vector<std::string> expanded;

    // ExpandArguments should set & report error state (i think).
    if (!status.GetMakefile().ExpandArguments(args, expanded)) {
        return false;
    }

    // another copy - blergh
    py::tuple out(expanded.size());
    for(std::size_t i = 0; i < expanded.size(); i++) {
        out[i] = ConvertArgCMakeToPython(expanded[i]);
        // std::cout << "ARG " << py::type::of(out[i]) << "\n";
    }

    //FIXME: get filepath
    cmMakefile::FunctionPushPop functionScope(&status.GetMakefile(), "FILEPATH", policies);
    //std::cout << "py function exec " << name <<  " arg count " <<  expanded.size() <<  "\n";

    try {
        // create factory and all py interfaces. Note: the makefile provided here comes from 
        // status and thus is the makefile/dir of the caller, not where the function was created
        auto factory = cmPythonFactory::Create(status.GetMakefile());

        py::function entryPoint = cmPythonModules::GetRootModule().attr("invoke_py_user_fn");
        py::kwargs kwargs = factory->BuildStdKwArgs();

        // call into python via our provided entrypoint
        py::object ret = entryPoint(fn, out, kwargs);

        // it's the job of invoke_py_user_fn() to grab any return values and do the appropriate
        // thing with them - it's an error to send them back here
        assert(ret.is_none());

        return true;
    } catch (const py::error_already_set& e) {
        status.SetError(e.what());
        return false;
    }
}

cmState::Command cmPythonPyFunctions::BuildLambda(const py::function& fn,
    const cmPolicies::PolicyMap& policies)
{
    // build the lambda for our function. Note that this might be called after this
    // cmPythonPyFunctions object destructs, so careful what gets captured here
    // (the reason this method is static is to prevent capturing this)
    cmState::Command c =
        [fn, policies] 
        (const std::vector<cmListFileArgument>& args, cmExecutionStatus& stat) {
            return Basecamp(fn, policies, args, stat);
        };

    return c;
}

void cmPythonPyFunctions::CheckFunction(const py::function& fn)
{
    py::function annotationScan = cmPythonModules::GetRootModule().attr("scan_for_annotations");
    annotationScan(fn);
}

void cmPythonPyFunctions::Add(const std::string& name, const py::function& fn)
{
    if (name == "add") {
        throw py::value_error("oh no you don't");
    }

    // run checks on the function now, avoid delaying any blowups until fn is called
    // any errors will be thrown straight back our to calling python code
    CheckFunction(fn);
  
    cmPolicies::PolicyMap policies;
    GetMakefile().RecordPolicies(policies);

    cmState::Command c = BuildLambda(fn, policies);

    // get file / line number
    py::object code = fn.attr("__code__");
    py::str fileName = code.attr("co_filename");
    int lineNum = code.attr("co_firstlineno").cast<int>();

    // FIXME: is this the context of where the function is being added, or whence
    // it runs?
    cmListFileContext context(name, fileName, lineNum);
    GetMakefile().GetBacktrace().Push(context);

    bool ret = GetMakefile().GetState()->AddScriptedCommand(
        name,
        BT<cmState::Command>(std::move(c), GetMakefile().GetBacktrace().Push(context)),
        GetMakefile(),
        cmStateEnums::ScriptedCommandType::Python,
        false); // allowOverwrite

    if(!ret) {
        throw std::runtime_error("failed to add Python functioyyn");
    }

    // keep track of this function for our own purposes
    GetUserFunctions()[py::str(name)] = fn;
}

void cmPythonPyFunctions::Add(const py::function& fn)
{
    Add(py::str(fn.attr("__name__")), fn);
}

std::vector<std::string> cmPythonPyFunctions::GetDefinitions() const
{
    std::vector<std::string> out;
    for(const auto& key: GetUserFunctions()) {
        out.emplace_back(py::str(key.first));
    }
    return out;
}

py::object cmPythonPyFunctions::getitem(const py::str& key)
{
    // throws on errof
    py::function fn = GetUserFunctions()[py::str(key)];
    return fn;
}

bool cmPythonPyFunctions::contains(const py::str& key)
{
    return GetUserFunctions().contains(key);
}

