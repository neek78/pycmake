
#include "cmPythonSourceFile.h"
#include "cmPythonTypeConversion.h"
#include "cmSourceFile.h"

namespace py = pybind11;

void cmPythonSourceFile::bind(pybind11::module_& m)
{
    auto cls = pybind11::class_<cmPythonSourceFile>(m, "SourceFile")
        .def("__str__", &cmPythonSourceFile::str)
        .def_property_readonly("properties", &cmPythonSourceFile::GetProperties)
        .def_property_readonly("compile_options", &cmPythonSourceFile::GetCompileOptions)
        .def_property_readonly("compile_definitions", &cmPythonSourceFile::GetCompileDefinitions)
        .def_property_readonly("include_directories", &cmPythonSourceFile::GetIncludeDirectories)
        ;
}

cmPythonSourceFile::cmPythonSourceFile(const cmSourceFile& sourceFile)
:   SourceFile(sourceFile)
{
}

std::string cmPythonSourceFile::str() const
{
    return "source file!";
}

py::dict cmPythonSourceFile::GetProperties() 
{
    return ConvertToDict(SourceFile.GetProperties());
}

py::list cmPythonSourceFile::GetCompileOptions()
{
    return ConvertToList(SourceFile.GetCompileOptions());
}

py::list cmPythonSourceFile::GetCompileDefinitions()
{
    return ConvertToList(SourceFile.GetCompileDefinitions());
}

py::list cmPythonSourceFile::GetIncludeDirectories()
{
    return ConvertToList(SourceFile.GetIncludeDirectories());
}
