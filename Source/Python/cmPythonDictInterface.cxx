#include "cmPythonDictInterface.h"
#include "cmStringAlgorithms.h"
#include "cmPythonTypeConversion.h"
#include "cmPythonDictView.h"

#include <iostream>

namespace py = pybind11;

void cmPythonDictInterface::bind(pybind11::module_& m)
{
    auto cls = py::class_<cmPythonDictInterface>(m, "DictInterface");

    cls.def("keys", &cmPythonDictInterface::keys, py::keep_alive<0, 1>());
    cls.def("values", &cmPythonDictInterface::values, py::keep_alive<0, 1>());
    cls.def("items", &cmPythonDictInterface::items, py::keep_alive<0, 1>());
    cls.def("__len__", &cmPythonDictInterface::len);
    cls.def("__getitem__", &cmPythonDictInterface::getitem);
    cls.def("__contains__", &cmPythonDictInterface::contains);
    cls.def("__iter__", &cmPythonDictInterface::iter, pybind11::keep_alive<0, 1>());
}

py::object cmPythonDictInterface::iter()
{
    // we could call keys().iter() directly, but then the keys
    // object will destruct and segfaults shortly after. 
    // Calling via python will keep the intermediate object alive
    // due to keep_alive in its binding
    py::object k = keys(); 
    return k.attr("__iter__")();
}

std::size_t cmPythonDictInterface::len() const
{
    return GetDefinitions().size();
}

py::object cmPythonDictInterface::keys()
{
    return py::cast(cmPythonDictViewKeys(*this));
}

py::object cmPythonDictInterface::values()
{
    return py::cast(cmPythonDictViewValues(*this));
}

py::object cmPythonDictInterface::items()
{
    return py::cast(cmPythonDictViewItems(*this));
}
