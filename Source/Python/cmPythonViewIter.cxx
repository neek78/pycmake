#include "cmPythonViewIter.h"

namespace py = pybind11;

void cmPythonViewIter::bind(pybind11::module_& m)
{
    py::class_<cmPythonViewIter>(m, "ViewIter")
        .def("__next__", &cmPythonViewIter::next)
        .def("__iter__", &cmPythonViewIter::iter, py::keep_alive<0, 1>());
}

cmPythonViewIter::cmPythonViewIter(cmPythonViewIterSrc& view)
:   View(view)
{
}

pybind11::object cmPythonViewIter::next()
{
    if(Current >= View.size()) {
        throw pybind11::stop_iteration();
    }
    return View.get(Current++);
}

cmPythonViewIter cmPythonViewIter::iter()
{
    return *this;
}

