/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonModBase.h"
#include "cmFileSet.h"

#include <pybind11/embed.h>

class cmFileSet;

class cmPythonFileSet
{
public:
    static void bind(pybind11::module_& m);
    cmPythonFileSet(const cmFileSet& fileSet);

private:
    pybind11::list GetFileEntries() const;
    pybind11::list GetDirEntries() const;

    std::string str() const;
    std::string GetName() const;
    std::string GetType() const;
    cmFileSetVisibility GetVisibility() const;

    const cmFileSet& FileSet;
};

