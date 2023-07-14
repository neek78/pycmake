/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonDictInterface.h"
#include "cmPythonDispatcher.h"
#include "cmPythonModBase.h"
#include "cmPythonTypes.h"
#include "cmState.h"

#include <string>
#include <map>

#include <pybind11/embed.h>

class cmMakefile;

// interface to call CMakeScript functions from python
class cmPythonFunctionLauncher : 
    public cmPythonDictInterface,
    public cmPythonDispatcher 
{
public:
    static void bind(pybind11::module_& m);

    cmPythonFunctionLauncher(cmMakefile& makefile); 
    cmPythonFunctionLauncher(const cmPythonFunctionLauncher&) = delete; 
    cmPythonFunctionLauncher& operator=(const cmPythonFunctionLauncher&) = delete;

    // add a function implemented in cmake script to be called from python
    static void AddScriptedCommand(const std::string& name, cmState::Command& c, 
            cmStateEnums::ScriptedCommandType type);

    static void RemoveAllScriptedCommands();

    pybind11::object getitem(const pybind11::str& key) override;
    bool contains(const pybind11::str& key) override;

private:
    using ClassType = pybind11::class_<cmPythonFunctionLauncher, cmPythonDictInterface>;
    static ClassType GetClassObject();

    static pybind11::dict GetUserFunctions();
    std::vector<std::string> GetDefinitions() const override;
};

