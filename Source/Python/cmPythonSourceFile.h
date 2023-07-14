/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <string>

#include <pybind11/embed.h>

class cmSourceFile;
class cmPythonSourceFile

{
public:
    static void bind(pybind11::module_& m);
    cmPythonSourceFile(const cmSourceFile& sourceFile);

private:
    pybind11::list GetFileEntries() const;
    std::string str() const;
    std::string GetName() const;

    pybind11::dict GetProperties();
    pybind11::list GetCompileOptions();
    pybind11::list GetCompileDefinitions();
    pybind11::list GetIncludeDirectories();

    const cmSourceFile& SourceFile;
};

