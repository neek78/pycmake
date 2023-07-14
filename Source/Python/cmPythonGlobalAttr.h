/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

class cmake;

class cmPythonGlobalAttr
{
public:
    static void bind(pybind11::module_& m);
    cmPythonGlobalAttr(cmake& cm);

private:
    pybind11::list GetFileEntries() const;
    std::string GetName() const;

    void test() const;
    //const cmFileSet& FileSet;
    cmake& CMake;
};

