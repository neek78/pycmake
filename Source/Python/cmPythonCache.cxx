/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonCache.h"
#include "cmPythonCacheEntry.h"
#include "cmPythonUtils.h"
#include "cmPythonTypeConversion.h"
#include "cmPythonModules.h"

#include "cmState.h"
#include "cmMakefile.h"

#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

void cmPythonCache::bind(pybind11::module_& m)
{
    auto cls = py::class_<cmPythonCache, cmPythonDictInterface>(m, "Cache");

    cls.def("__setitem__", &cmPythonCache::setitem);
    cls.def("__delitem__", &cmPythonCache::delitem);

    cls.def("set", &cmPythonCache::set,
            py::arg("name"),
            py::arg("value"),
            py::kw_only(),
            py::arg_v("force", false, ""),
            py::arg_v("type", cmStateEnums::CacheEntryType::STRING, ""),
            py::arg("doc_string"),
            py::arg_v("advanced", false, ""));

    cls.def("unset", &cmPythonCache::unset);
}

cmPythonCache::cmPythonCache(cmMakefile& makefile)
:   cmPythonModBase(makefile)
{
}

py::object cmPythonCache::getitem(const py::str& key)
{
    using namespace literals; 

    // check key exists
    if (!contains(key)) {
        py::str msg = "no cache item for key"_pys + key + " found"_pys;
        throw py::key_error(msg);
    }

    return py::cast(cmPythonCacheEntry(GetMakefile(), key));
}

void cmPythonCache::setitem(const py::str& key, const py::object& value)
{
    set(key, value, false, std::nullopt, "", false);
}

void cmPythonCache::delitem(const py::str& var)
{
    using namespace literals;

    commonChecks(var);

    // RemoveCacheDefinition does nothing if the var does not exist, so we
    // have to check it explicitly. 
    if(!contains(var)) {
        const py::str err = "no cache item "_pys + var + " set"_pys ;
        throw pybind11::key_error(err);
    }

    GetMakefile().RemoveCacheDefinition(var);
}

cmStateEnums::CacheEntryType cmPythonCache::DetermineType(
        std::optional<cmStateEnums::CacheEntryType> type,         
        const py::object& val)
{
    if (type) {
        return *type;
    }

    // if the type wasn't specified, take a best guess ourselves
    cmStateEnums::CacheEntryType t = cmStateEnums::CacheEntryType::STRING;

    if(py::isinstance<py::bool_>(val)) {
        t = cmStateEnums::CacheEntryType::BOOL;
    } else if(py::isinstance(val, cmPythonModules::GetTypePath())) {
        t = cmStateEnums::CacheEntryType::FILEPATH;
    } 

    return t;
}

void cmPythonCache::set(
        const py::str& var,
        const py::object& val,
        bool force,
        std::optional<cmStateEnums::CacheEntryType> type,
        const pybind11::str& docString,
        bool advanced)
{
    // std::cout << "set " << force << " type " << (bool)type  << " docString " << (bool)docString << "\n";
    setCommonChecks(var, val);

    std::string varName = var;
    const std::string value = ConvertArgPythonToCMake(val);

    cmStateEnums::CacheEntryType t = DetermineType(type, val);

    //  SET (VAR CACHE TYPE "doc String" [FORCE])
    //  SET (VAR value CACHE TYPE "doc string" [FORCE])
    GetMakefile().AddCacheDefinition(var, value, docString, t, force);

    if (advanced) {
        GetMakefile().GetState()->SetCacheEntryProperty(varName, "ADVANCED", "1");
    }
}

void cmPythonCache::unset(
        const py::str& var)
{
    // note that this method does not throw on non-existent value (unlike __delitem__)
    commonChecks(var);
    GetMakefile().RemoveCacheDefinition(var);
}

std::vector<std::string> cmPythonCache::GetDefinitions() const
{
    return GetMakefile().GetState()->GetCacheEntryKeys();
}

bool cmPythonCache::contains(const py::str& key)
{
    return !!GetMakefile().GetState()->GetCacheEntryValue(key);
}

