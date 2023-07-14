#include "cmPythonKwArg.h"
#include "cmPythonModules.h"
#include "cmPythonUtils.h"

#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

cmPythonKwArg::cmPythonKwArg(const pybind11::handle& pyKwArg)
:   PyKwArg(pyKwArg.cast<py::object>())
{
    assert(py::isinstance(PyKwArg, cmPythonModules::GetTypeKwArg()));
}

bool cmPythonKwArg::GetEnabled() const
{
    return SinglePropExtract<bool>(PyKwArg, "enabled");
}

