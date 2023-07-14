/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonFunctionLauncher.h"
#include "cmPythonReturnParam.h"
#include "cmPythonUtils.h"
#include "cmPythonEnums.h"

#include "cmState.h"
#include "cmMakefile.h"

#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <iostream>

namespace py = pybind11;

namespace {

const char* USER_FUNCTIONS_LIST = "__userfunctions";

}

void cmPythonFunctionLauncher::bind(py::module_& m)
{
    // all methods in this class are added dynamically,
    auto cls = ClassType(m, "FunctionLauncher");

    cls.attr(USER_FUNCTIONS_LIST) = py::dict();
}

cmPythonFunctionLauncher::cmPythonFunctionLauncher(cmMakefile& makefile)
:   cmPythonDispatcher(makefile)
{

}

cmPythonFunctionLauncher::ClassType cmPythonFunctionLauncher::GetClassObject()
{
    // this will kersplode if the python scoped_interpreter does not exist yet 
    // and the cmake_builtin module is not imported - both are done if cmPythonCore
    // has been created/init'd
    return py::type::of<cmPythonFunctionLauncher>();
}

pybind11::dict cmPythonFunctionLauncher::GetUserFunctions()
{
    return GetClassObject().attr(USER_FUNCTIONS_LIST);
}

void cmPythonFunctionLauncher::AddScriptedCommand(const std::string& name, 
        cmState::Command& cmd, cmStateEnums::ScriptedCommandType type)
{
    ClassType cls = GetClassObject();

    // Bind a new method to the class
    cls.def(name.c_str(), 
        [name, cmd](cmPythonFunctionLauncher& self, 
            const py::args& args, const py::kwargs& kwargs) {
            return self.invokeFunction(cmd, name, args, kwargs); 
        });

    auto fn = cls.attr(name.c_str());
    
    // keep track of this registration for our own purposes
    GetUserFunctions()[name.c_str()] = py::cast(std::make_pair(type, fn));
}

void cmPythonFunctionLauncher::RemoveAllScriptedCommands()
{
    ClassType cls = GetClassObject();
    auto userFunctions = GetUserFunctions();
    
    for(const auto& e : userFunctions) {
        py::delattr(cls, e.first);
    }
    userFunctions.clear();
    assert(userFunctions.empty());
}

py::object cmPythonFunctionLauncher::getitem(const py::str& name)
{
    // cmake lower-cases function names
    std::string sName = cmSystemTools::LowerCase(name);
    return GetUserFunctions()[sName.c_str()];
}

bool cmPythonFunctionLauncher::contains(const py::str& key)
{
    auto userFunctions = GetUserFunctions();
    // cmake lower-cases function names
    std::string sName = cmSystemTools::LowerCase(key);
    return userFunctions.contains(sName);
}

std::vector<std::string> cmPythonFunctionLauncher::GetDefinitions() const 
{
    std::vector<std::string> out;
    for(const auto& e : GetUserFunctions()) {
        out.emplace_back(py::str(e.first));
    }
    return out;
}
