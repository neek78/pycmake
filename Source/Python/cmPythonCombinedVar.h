/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonDictInterface.h"
#include "cmPythonVarChecks.h"
#include "cmPythonModBase.h"
#include "cmPythonUtils.h"

#include "cmStateTypes.h"

#include <pybind11/embed.h>

class cmMakefile;

class cmPythonCombinedVar : 
    public cmPythonModBase,
    public cmPythonDictInterface,
    public cmPythonVarChecks
{
public:
    static void bind(pybind11::module_& m);

    cmPythonCombinedVar(cmMakefile& makefile); 
    cmPythonCombinedVar(const cmPythonCombinedVar&) = delete; 
    cmPythonCombinedVar& operator=(const cmPythonCombinedVar&) = delete;

    pybind11::object getitem(const pybind11::str& key) override;
    bool contains(const pybind11::str& key) override;

protected:
    std::vector<std::string> GetDefinitions() const override;
    std::string Expand(const std::string& in);
    std::string ExpandRelaxed(const std::string& in);
    pybind11::object ConvertFromCache(const std::string& key);
};

