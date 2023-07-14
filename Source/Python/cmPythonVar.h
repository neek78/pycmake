/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonDictInterface.h"
#include "cmPythonModBase.h"
#include "cmPythonUtils.h"
#include "cmPythonVarChecks.h"

#include "cmStateTypes.h"

#include <pybind11/embed.h>

class cmMakefile;
class cmValue;

class cmPythonVar : 
    public cmPythonModBase,
    public cmPythonDictInterface,
    public cmPythonVarChecks
{
public:
    static void bind(pybind11::module_& m);

    cmPythonVar(cmMakefile& makefile); 
    cmPythonVar(const cmPythonVar&) = delete; 
    cmPythonVar& operator=(const cmPythonVar&) = delete;

    bool contains(const pybind11::str& key) override;
    pybind11::object getitem(const pybind11::str& key) override;

private:
    // dict interface
    void setitem(const pybind11::str& key, const pybind11::object& val);
    void delitem(const pybind11::str& key);

    void Set(
        const pybind11::str& var,
        const pybind11::object& value);

    void SetParentScope(const pybind11::str& var, const pybind11::object& value);

    void Unset(const pybind11::str& var);

    void UnsetInternal(
        const pybind11::str& var,
        bool throwIfDoesntExist);

    void UnsetParentScope(const pybind11::str& var);

    void PushToParentScope(const pybind11::str& var);

    bool HasParentScope() const;

    //pybind11::object GetParentScope();

    cmValue GetDefinition(const std::string& key) const;
    std::vector<std::string> GetDefinitions() const override;
};

