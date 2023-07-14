/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonDictInterface.h"
#include "cmPythonUtils.h"
#include "cmGlobalGenerator.h"

#include <pybind11/embed.h>

class cmPythonDirAttr;

class cmPythonSourceDirs :
    public cmPythonDictInterface
{
public:
    static void bind(pybind11::module_& m);

    cmPythonSourceDirs(cmGlobalGenerator& globalGenerator);
    cmPythonSourceDirs(const cmPythonSourceDirs&) = delete; 
    cmPythonSourceDirs& operator=(const cmPythonSourceDirs&) = delete;

    using KeyType = std::string;

    // dict interface
    pybind11::object getitem(const pybind11::str& key) override;
    bool contains(const pybind11::str& key) override;

protected:
    std::string collapse(const std::string& path) const;
    const std::vector<std::unique_ptr<cmMakefile>>& GetIdx() const;

    cmMakefile* FindBySrcDir(const KeyType& key) const;
    std::vector<std::string> GetDefinitions() const override;

    cmGlobalGenerator& GlobalGenerator;
};

