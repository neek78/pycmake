/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonCacheEntry.h"
#include "cmPythonUtils.h"
#include "cmPythonTypeConversion.h"
#include "cmState.h"
#include "cmMakefile.h"

#include <filesystem>

#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

void cmPythonCacheEntry::bind(pybind11::module_& m)
{
    auto cls = py::class_<cmPythonCacheEntry>(m, "CacheEntry");

    cls.def("__repr__", &cmPythonCacheEntry::repr);

    cls.def_property_readonly("name", &cmPythonCacheEntry::name);
    cls.def_property_readonly("value", &cmPythonCacheEntry::value);
    cls.def_property_readonly("cache_type", &cmPythonCacheEntry::cachetype);
    cls.def_property_readonly("properties", &cmPythonCacheEntry::properties);
    cls.def_property("advanced", &cmPythonCacheEntry::GetAdvanced, &cmPythonCacheEntry::SetAdvanced);
}

cmPythonCacheEntry::cmPythonCacheEntry(cmMakefile& makefile, std::string entryName)
:   cmPythonModBase(makefile),
    EntryName(std::move(entryName))
{
}

const std::string& cmPythonCacheEntry::name() const
{
    return EntryName;
}

void cmPythonCacheEntry::ThrowKeyError() const
{
    throw py::key_error("the cache entry " + name() + " has been deleted out from under us");
}

void cmPythonCacheEntry::CheckStillAlive() const
{
    auto val = GetMakefile().GetState()->GetCacheEntryValue(EntryName);
    if(!val) {
        ThrowKeyError();
    }
}

cmStateEnums::CacheEntryType cmPythonCacheEntry::cachetype() const
{
    CheckStillAlive();
    auto t =  GetMakefile().GetState()->GetCacheEntryType(EntryName);
    return t;
}

py::dict cmPythonCacheEntry::properties() const
{
    CheckStillAlive();
    py::dict out;
    for(const auto& item : GetMakefile().GetState()->GetCacheEntryPropertyList(name())) {
        out[py::str(item)] = ConvertArgCMakeToPython(
                GetMakefile().GetState()->GetCacheEntryProperty(name(), item));
    }
    return out;
}

py::object cmPythonCacheEntry::value() const
{
    CheckStillAlive();
    auto val = GetMakefile().GetState()->GetCacheEntryValue(EntryName);
    return convertToPython(val, cachetype(), name());
}

pybind11::str cmPythonCacheEntry::repr() const
{
    using namespace py::literals;

    py::dict out(
        "name"_a = name(),
        "value"_a = value(),
        "cache_type"_a = cachetype(),
        "properties"_a = properties(),
        "advanced"_a = GetAdvanced());

    return py::repr(out);
}

bool cmPythonCacheEntry::GetAdvanced() const
{
    return GetMakefile().GetState()->GetCacheEntryPropertyAsBool(name(), "ADVANCED");
}

void cmPythonCacheEntry::SetAdvanced(bool advanced)
{
    return GetMakefile().GetState()->SetCacheEntryProperty(name(), "ADVANCED", advanced ? "1" : "0");
}

void SetAdvanced(bool advanced);
py::object cmPythonCacheEntry::convertToPython(const cmValue& val, 
            cmStateEnums::CacheEntryType type, const std::string& name)
{
    // special case conversion code, as we have a bit more extra info in the form
    // of the Type param, generally not available in most places
    
    // check first for "null" values 
    // this also handles the NOTFOUND case
    if(!val.IsSet()) {
        return py::none();
    } 

    // if it's a bool, should be able to use built-in conversion
    if(type == cmStateEnums::CacheEntryType::BOOL) {
        if(val.IsOn()) {
            return py::bool_(true);
        }

        // FIXME: this will also return if it's a NOTFOUND value. Should we explicitly pick these
        // up first and return something else?
        if(val.IsOff()) {
            return py::bool_(false);
        }
        
        // should probably also check 
        // cmValue::IsInternallyOn() though I'm not quite sure what to do with that case.

        // not sure if this can happen, but when everthing's stored as a string...
        const auto msg = std::string("could not convert value ") + *val+ " to bool " 
            + " for cache entry " + name;
        throw py::type_error(msg);
    }
    
    // are there lists of filepaths?
    if(type == cmStateEnums::CacheEntryType::FILEPATH ||
       type == cmStateEnums::CacheEntryType::PATH) {
        // if we convert to a std::filesystem::path, pybind11 will deploy its
        // magic to convert to the appropriate python path type
        const auto path = std::filesystem::path(*val);
        return py::cast(path);
    }

    // OK, generic conversion for other types.
    // Of course, "INTERNAL" is not really a type, but what can we do...
    return ConvertArgCMakeToPython(*val);
}

