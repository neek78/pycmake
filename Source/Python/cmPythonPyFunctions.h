/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonModBase.h"
#include "cmPythonTypes.h"
#include "cmListFileCache.h"
#include "cmPythonDictInterface.h"
#include "cmState.h"

#include <string>
#include <vector>

#include <pybind11/embed.h>

class cmExecutionStatus;
class cmMakefile;

// FIXME: the name of this class sucks
class cmPythonPyFunctions : 
    public cmPythonDictInterface,
    public cmPythonModBase
{
public:
    static void bind(pybind11::module_& m);

    cmPythonPyFunctions(cmMakefile& makefile); 
    cmPythonPyFunctions(const cmPythonPyFunctions&) = delete; 
    cmPythonPyFunctions& operator=(const cmPythonPyFunctions&) = delete;

    pybind11::object getitem(const pybind11::str& key) override;
    bool contains(const pybind11::str& key) override;

protected:
    // add a function implemented in python to be called from cmake script
    void Add(const std::string& name, const pybind11::function& fn);
    void Add(const pybind11::function& fn);

    // and remove it
    //void Delete(const std::string& name);
    std::vector<std::string> GetDefinitions() const override;

    static cmState::Command BuildLambda(const pybind11::function& fn,
        const cmPolicies::PolicyMap& policies);

    // entrypoint from cmake code into our function
    static bool Basecamp(const pybind11::function& fn, 
        const cmPolicies::PolicyMap& policies,
        const std::vector<cmListFileArgument>& args, 
        cmExecutionStatus& status);

    static void CheckFunction(const pybind11::function& fn);

    pybind11::type GetClassObject();
    pybind11::type GetClassObject() const;
    pybind11::dict GetUserFunctions();
    pybind11::dict GetUserFunctions() const;
};

