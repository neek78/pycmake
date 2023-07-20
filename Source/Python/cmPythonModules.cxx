/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#include "cmPythonModules.h"
#include "cmPythonConstants.h"

namespace py = pybind11;

py::module_ cmPythonModules::GetModuleSys()
{
    return py::module_::import("sys");
}

py::module_ cmPythonModules::GetModulePathLib()
{
    return py::module_::import("pathlib");
}

py::module_ cmPythonModules::GetModuleInspect()
{
    return py::module_::import("inspect");
}

py::type cmPythonModules::GetTypePath()
{
    py::object path = GetModulePathLib().attr("Path")();
    return py::type::of(path);
}

py::module_ cmPythonModules::GetModuleImportLibUtil()
{
    return py::module_::import("importlib.util");
}

pybind11::type cmPythonModules::GetTypeReturnParam()
{
    //FIXME: can we save the type::of call by just getting ReturnParam (without the () operator)?
    py::object returnParam = GetRootModule().attr("ReturnParam")();
    return py::type::of(returnParam);
}

pybind11::type cmPythonModules::GetTypeKwArg()
{
    py::object kwArg = GetRootModule().attr("KwArg")();
    return py::type::of(kwArg);
}

pybind11::module_ cmPythonModules::GetRootModule()
{
    return py::module_::import(PYTHON_CMAKE_MOD.c_str());
}

pybind11::module_ cmPythonModules::GetBuiltinModule()
{
    static const std::string full = PYTHON_CMAKE_MOD + "." + PYTHON_BUILTIN_MOD;
    return py::module_::import(full.c_str());
}

pybind11::module_ cmPythonModules::GetClientModule()
{
    static const std::string full = PYTHON_CMAKE_MOD + "." + PYTHON_CLIENT_MOD;
    return py::module_::import(full.c_str());
}

