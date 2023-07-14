#include "cmPythonReturnParam.h"
#include "cmPythonModules.h"
#include "cmPythonUtils.h"

#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

cmPythonReturnParam::cmPythonReturnParam(const pybind11::handle& pyReturnParam)
:   PyReturnParam(pyReturnParam.cast<py::object>())
{
    assert(py::isinstance(PyReturnParam, cmPythonModules::GetTypeReturnParam()));
}

bool cmPythonReturnParam::GetAutoCleanup() const
{
    return SinglePropExtract<bool>(PyReturnParam, "auto_cleanup");
}

void cmPythonReturnParam::SetArgPosition(int pos)
{
    PyReturnParam.attr("_arg_pos") = pos;
}

std::optional<int> cmPythonReturnParam::GetArgPosition() const
{
    return SinglePropExtract<std::optional<int>>(PyReturnParam, "_arg_pos");
}

std::optional<std::string> cmPythonReturnParam::GetDefaultValue() const
{
    return SinglePropExtract<std::optional<std::string>>(PyReturnParam, "default_value");
}

bool cmPythonReturnParam::GetRaiseOnOverwrite() const
{
    return SinglePropExtract<bool>(PyReturnParam, "raise_on_overwrite");
}

std::string cmPythonReturnParam::GetName() const
{
    return SinglePropExtract<std::string>(PyReturnParam, "name");
}

bool cmPythonReturnParam::ShouldRaiseOnUnset() const
{
    return SinglePropExtract<bool>(PyReturnParam, "should_raise_on_unset");
}


