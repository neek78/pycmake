/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonCombinedVar.h"
#include "cmPythonViewIter.h"
#include "cmPythonCacheEntry.h"
#include "cmPythonTypeConversion.h"
#include "cmPythonExpander.h"

#include "cmState.h"
#include "cmMakefile.h"

#include <iostream>

namespace py = pybind11;

void cmPythonCombinedVar::bind(pybind11::module_& m)
{
    auto cls = py::class_<cmPythonCombinedVar, cmPythonDictInterface>(m, "CombinedVar");

    cls.def("expand", &cmPythonCombinedVar::Expand);
    cls.def("expand_relaxed", &cmPythonCombinedVar::ExpandRelaxed);
}

cmPythonCombinedVar::cmPythonCombinedVar(cmMakefile& makefile)
:   cmPythonModBase(makefile)
{
}

std::vector<std::string> cmPythonCombinedVar::GetDefinitions() const
{
    return GetMakefile().GetDefinitions();
}

bool cmPythonCombinedVar::contains(const py::str& key)
{
    return !!GetMakefile().GetDefinition(key);
}

std::string cmPythonCombinedVar::Expand(const std::string& in)
{
    return ExpandVar(GetMakefile(), in, true);
}

std::string cmPythonCombinedVar::ExpandRelaxed(const std::string& in)
{
    return ExpandVar(GetMakefile(), in, false);
}

py::object cmPythonCombinedVar::getitem(const py::str& key)
{
    using namespace literals;

    const auto& val = GetMakefile().GetDefinition(key);
    if (!val) {
        const py::str err = "no value for "_pys + key;
        throw pybind11::key_error(err);
    }

    // Ugly: need to work out if this came from the cache or a var.
    // if it's a cache entry, apply our slightly better type conversion
    // So many lookups...
    bool isFromCache = !GetMakefile().IsNormalDefinitionSet(key);

    return isFromCache ? ConvertFromCache(key) : ConvertArgCMakeToPython(*val);
}

py::object cmPythonCombinedVar::ConvertFromCache(const std::string& key)
{
    // temp object is a bit much - though piles of lookups are required anyway
    cmPythonCacheEntry e(GetMakefile(), key);
    return e.value();
}
