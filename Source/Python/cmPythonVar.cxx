#include "cmPythonVar.h"
#include "cmPythonViewIter.h"
#include "cmState.h"
#include "cmMakefile.h"
#include "cmPythonTypeConversion.h"
#include "cmPythonUtils.h"

#include <iostream>

namespace py = pybind11;

void cmPythonVar::bind(pybind11::module_& m)
{
    auto cls = py::class_<cmPythonVar, cmPythonDictInterface>(m, "Var");

    cls.def("__setitem__", &cmPythonVar::setitem);
    cls.def("__delitem__", &cmPythonVar::delitem);

    cls.def("set", &cmPythonVar::Set,
            py::arg("name"),
            py::arg("value"));

    cls.def("set_parent_scope", &cmPythonVar::SetParentScope,
            py::arg("name"),
            py::arg("value"));

    cls.def("unset", &cmPythonVar::Unset,
            py::arg("name"));

    cls.def("unset_parent_scope", &cmPythonVar::UnsetParentScope,
            py::arg("name"));

    cls.def("push_to_parent_scope", &cmPythonVar::PushToParentScope,
            py::arg("name"));

    cls.def_property_readonly("has_parent_scope", &cmPythonVar::HasParentScope);

    // cls.def("parent_scope", &cmPythonVar::GetParentScope);
}

cmPythonVar::cmPythonVar(cmMakefile& makefile)
:   cmPythonModBase(makefile)
{
}

void cmPythonVar::delitem(const py::str& key)
{
    UnsetInternal(key, true);
}

void cmPythonVar::setitem(const py::str& key, const pybind11::object& val)
{
    Set(key, val);
}

void cmPythonVar::Set(
        const py::str& var,
        const py::object& val)
{
    setCommonChecks(var, val);
    GetMakefile().AddDefinition(var, ConvertArgPythonToCMake(val));
}

void cmPythonVar::SetParentScope(
        const py::str& var,
        const py::object& val)
{
    setCommonChecks(var, val);
    GetMakefile().RaiseScope(var, ConvertArgPythonToCMake(val).c_str());
}

void cmPythonVar::Unset(const py::str& var)
{
    UnsetInternal(var, false);
}

void cmPythonVar::UnsetInternal(const py::str& var, bool throwIfDoesntExist)
{
    commonChecks(var);

    if (!contains(var)) {
        if (throwIfDoesntExist) {
            const py::str err = "cannot delete "_pys + var + 
                " - entry does not exist"_pys;
            throw py::key_error(err);
        }
        return;
    }

    GetMakefile().RemoveDefinition(var);
}

void cmPythonVar::UnsetParentScope(
        const py::str& var)
{
    if (!HasParentScope()) {
        py::str err  = "cannot delete '"_pys + var + 
                "' from parent scope - current scope has no parent"_pys;
        // converts to py::runtime_error
        throw std::runtime_error(err);
    }

    GetMakefile().RaiseScope(std::string(var), nullptr);
}

bool cmPythonVar::HasParentScope() const
{
    return !GetMakefile().IsRootMakefile();
}

void cmPythonVar::PushToParentScope(const py::str& var)
{
    using namespace literals;

    commonChecks(var);

    // RaiseScope does check for this, but it'll just raise a textual error and 
    // keep going - we want to hit the python code with an exception
    if (!HasParentScope()) {
        py::str err = "cannot push '"_pys + var + 
                "' to parent scope - current scope has no parent"_pys;
        throw std::runtime_error(err);
    }

    const auto& val = GetDefinition(var);
    // check key exists
    if (!val) {
        py::str err = "no cache item for key "_pys + var + " found"_pys;
        throw py::key_error(err);
    }

    GetMakefile().RaiseScope(var, val);
}

cmValue cmPythonVar::GetDefinition(const std::string& key) const
{
    return GetMakefile().GetStateSnapshot().GetDefinition(key);
}

std::vector<std::string> cmPythonVar::GetDefinitions() const
{
    return GetMakefile().GetStateSnapshot().ClosureKeys();
}

py::object cmPythonVar::getitem(const py::str& key)
{
    using namespace literals;

    const auto& val = GetDefinition(key);
    if (!val) {
        const py::str err = "no value for "_pys  + key;
        throw pybind11::key_error(err);
    }
    return ConvertArgCMakeToPython(*val);
}

bool cmPythonVar::contains(const py::str& key)
{
    const auto& val = GetDefinition(key);
    return !!val;
}

#if 0
pybind11::object cmPythonVar::GetParentScope() 
{
    if (!HasParentScope()) {
        throw py::value_error("current scope has no parent");
    }

    return py::cast(cmPythonVar(GetMakefile()));
}
#endif
