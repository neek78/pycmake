/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

// a wrapper to get python types from one place. 
// Just saves on a few typos here and there.
class cmPythonModules
{
public:
    static pybind11::module_ GetModuleSys();
    static pybind11::module_ GetModulePathLib();
    static pybind11::type GetTypePath();
    static pybind11::module_ GetModuleImportLibUtil();
    static pybind11::type GetTypeReturnParam();
    static pybind11::type GetTypeKwArg();

    // return the '_builtin' module - access to entities implemented in c++
    static pybind11::module_ GetBuiltinModule();

    // return the client module - the root into which all client code is loaded
    static pybind11::module_ GetClientModule();

    // return the 'cmake' module - ie the std pycmake library implemented in python
    static pybind11::module_ GetRootModule();
};

