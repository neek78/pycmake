/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonModBase.h"
#include "cmPythonRaw.h"
#include "cmPythonVar.h"
#include "cmPythonCache.h"
#include "cmPythonDirAttr.h"
#include "cmPythonSourceDirs.h"
#include "cmPythonFunctionLauncher.h"
#include "cmPythonPyFunctions.h"
#include "cmPythonGlobalAttr.h"
#include "cmPythonCombinedVar.h"

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include <pybind11/pybind11.h>

class cmPythonFactory : 
    public std::enable_shared_from_this<cmPythonFactory>
{
public:
    static void bind(pybind11::module_& m);

    static std::shared_ptr<cmPythonFactory> Create(cmMakefile& mf);

    cmPythonFactory(const cmPythonFactory&) = delete; 
    cmPythonFactory& operator=(const cmPythonFactory&) = delete;

    bool InvokePythonEntryPoint(const pybind11::module_& clientMod);

    cmPythonRaw& raw();

    // objects passed to calls to python entrypoints
    pybind11::kwargs BuildStdKwArgs();

private:
    // private constructor to force use of Create()
    cmPythonFactory(cmMakefile& makefile);

    // find cmake_main() in user module
    pybind11::function GetUserEntryPoint(const pybind11::module_& mod);
    void CheckReturnValue(const pybind11::object& ret);

    cmPythonVar& var();
    cmPythonCombinedVar& combined_var();
    cmPythonDirAttr& dir_attr();
    pybind11::object root_source_dir();
    cmPythonFunctionLauncher& function_launcher();
    cmPythonPyFunctions& pyFunctions();
    std::filesystem::path ConvertToRelativeSourcePath(const std::filesystem::path& path) const;

    cmPythonGlobalAttr& global_attr();
    cmPythonCache& cache();
    cmPythonSourceDirs& source_dirs();
    pybind11::dict cmake_version();

    cmPythonGlobalAttr GlobalAttr;
    cmPythonRaw Raw;
    cmPythonVar Var;
    cmPythonCombinedVar CombinedVar;
    cmPythonCache Cache;
    cmPythonSourceDirs SourceDirs;
    cmPythonDirAttr DirAttr;
    cmPythonFunctionLauncher FunctionLauncher;
    cmPythonPyFunctions PyFunctions;

    cmMakefile& Makefile;

    // the c++ cmake module
    pybind11::module_ BuiltinModule;

    // invoke_cmake_main() function from our builtin module
    pybind11::function EntryPoint;
};

