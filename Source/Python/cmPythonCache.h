/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonDictInterface.h"
#include "cmPythonVarChecks.h"
#include "cmPythonModBase.h"

#include "cmStateTypes.h"

#include <pybind11/embed.h>

#include <string>

class cmMakefile;

// access to the global cmake cache from python
class cmPythonCache : 
    public cmPythonModBase,
    public cmPythonDictInterface,
    public cmPythonVarChecks
{
public:
    static void bind(pybind11::module_& m);

    cmPythonCache(cmMakefile& makefile); 
    cmPythonCache(const cmPythonCache&) = delete; 
    cmPythonCache& operator=(const cmPythonCache &) = delete;

    static cmStateEnums::CacheEntryType DetermineType(
        std::optional<cmStateEnums::CacheEntryType> type,         
        const pybind11::object& val);

    pybind11::object getitem(const pybind11::str& key) override;
    bool contains(const pybind11::str& key) override;

protected:
    // dict interface
    void setitem(const pybind11::str& key, const pybind11::object& val);
    void delitem(const pybind11::str& var);

    void set(
        const pybind11::str& var,
        const pybind11::object& val,
        bool force,
        std::optional<cmStateEnums::CacheEntryType> type,
        const pybind11::str& docString,
        bool advanced);

    void unset(
        const pybind11::str& var);

    std::vector<std::string> GetDefinitions() const override;

};

