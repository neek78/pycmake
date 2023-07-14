/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonFileSet.h"

#include "cmPythonTypeConversion.h"
#include "cmPythonUtils.h"
#include "cmList.h"

#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

void cmPythonFileSet::bind(pybind11::module_& m)
{
    auto cls = pybind11::class_<cmPythonFileSet>(m, "FileSet")
        .def("__str__", &cmPythonFileSet::str)
        .def_property_readonly("name", &cmPythonFileSet::GetName)
        .def_property_readonly("type", &cmPythonFileSet::GetType)
        .def_property_readonly("visibility", &cmPythonFileSet::GetVisibility)
        .def_property_readonly("file_entries", &cmPythonFileSet::GetFileEntries)
        .def_property_readonly("dir_entries", &cmPythonFileSet::GetDirEntries)
        ;

}

cmPythonFileSet::cmPythonFileSet(const cmFileSet& fileSet)
:   FileSet(fileSet)
{
}

std::string cmPythonFileSet::str() const
{
    return GetName();
}

std::string cmPythonFileSet::GetName() const
{
    return FileSet.GetName();
}

std::string cmPythonFileSet::GetType() const
{
    return FileSet.GetType();
}

cmFileSetVisibility cmPythonFileSet::GetVisibility() const
{
    return FileSet.GetVisibility();
}

py::list cmPythonFileSet::GetFileEntries() const
{
    py::list out;

    // bah! this vector is actually a vector of cmake (semicolon separated) lists
    // there is no key, so.. I guess a list of lists
    for (auto const& e : FileSet.GetFileEntries()) {
        py::list l = ConvertToList(cmList{e.Value});
        out.append(l);
    }
    return out;
}

py::list cmPythonFileSet::GetDirEntries() const
{
    return ConvertToList(FileSet.GetDirectoryEntries());
}
