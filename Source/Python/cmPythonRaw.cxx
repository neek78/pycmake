/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
                              
#include "cmPythonRaw.h"
#include "cmPythonCommands.h"

#include "cmCMakeMinimumRequired.h"
#include "cmProjectCommand.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include <pybind11/stl.h>

namespace py = pybind11;

void cmPythonRaw::bind(pybind11::module_& m)
{
    auto cls = pybind11::class_<cmPythonRaw>(m, "Raw");
    auto *c = &cls;

    // register all (most) cmake commands as a callback to invokeFunction()
    buildPythonCommands([c](const std::string& fnName, const cmState::BuiltinCommand& function) {
        build(c, fnName, function);
    });

    // .. and the 1 other command with a different signature.
    buildPythonCommands([c](const std::string& fnName, const cmState::Command& function) {
        build(c, fnName, function);
    });
}

template<typename FunctionType>
void cmPythonRaw::build(ClassType* cls, const std::string& fnName, const FunctionType& function) 
{
    assert(cls);
    // std::cout << "add builtin: " << fnName<< "\n";

    // avoid duplicate function registration
    assert(!pybind11::hasattr(*cls, fnName.c_str()));

    // here is the actual initial entrypoint from python calls 
    cls->def(fnName.c_str(), [fnName, function] (cmPythonRaw& self, const py::args& args) {
        // std::cout << "invoke lambda builtin: " << fnName<< " argc " << args.size() << "\n";
        return self.invokeFunction(function, fnName, args);
        });
}

cmPythonRaw::cmPythonRaw(cmMakefile& makefile)
:   cmPythonDispatcher(makefile)
{
}

void cmPythonRaw::checkCommands(const cmState::BuiltinCommand& f)
{
    // track calls to project/min version required so we can error if they're not called 
    // from the root script
    if(f == cmProjectCommand) {
        SeenProjectCommand = true;
    } else if (f == cmCMakeMinimumRequired) {
        SeenMinVersionRequiredCommand = true;
    }
}

// there are no commands of this type to check, but we need this to keep the template 
// monster happy
void cmPythonRaw::checkCommands(const cmState::Command&)
{
}

template<typename FunctionType>
py::object cmPythonRaw::invokeFunction(const FunctionType& function, 
        const std::string_view& fnName, const pybind11::args& args)
{
    checkCommands(function);
    return cmPythonDispatcher::invokeFunction(function, fnName, args);
}

