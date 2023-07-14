/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonModBase.h"
#include "cmPythonUtils.h"
#include "cmPythonEnums.h"

#include <pybind11/embed.h>

#include <string>
#include <filesystem>

class cmStateDirectory;
class cmPythonTarget;
class cmPythonSourceFile;

class cmPythonDirAttr
:   public cmPythonModBase
{
public:
    static void bind(pybind11::module_& m);

    cmPythonDirAttr(cmMakefile& makefile);

private:
    cmMakefile* FindMakefile(const std::string& key) const;

    //std::string str() const;
    pybind11::str repr();
    pybind11::dict Properties() const;

    std::string ProjectName() const;
    CMakeScriptType ScriptType() const;
    std::filesystem::path BuildDir() const;
    std::filesystem::path SourceDir() const;
    std::filesystem::path RelativeSourceDir() const;
    bool IsRootSourceDir() const;

    pybind11::list ListFiles() const;
    pybind11::list OutputFiles() const;
    pybind11::list IncludeDirs() const;
    pybind11::list CompileDefinitions() const;
    pybind11::list CompileOptions() const;
    pybind11::list LinkOptions() const;
    pybind11::list LinkDirectories() const;

    pybind11::dict BuildSystemTargets();
    pybind11::dict ImportedTargets() ;
    pybind11::dict AliasTargets() const;
    cmPythonTarget FindTarget(const std::string& name) ;

    pybind11::object Parent() const;
    pybind11::dict Children() const;

    cmPythonSourceFile GetSourceFile(const pybind11::str& name);

    cmStateDirectory GetDirectory() const;

    bool eq(const cmPythonDirAttr& other) const;

    pybind11::object GetPolicyStatus(const pybind11::object& policy) const;
};

