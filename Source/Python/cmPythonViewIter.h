/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

class cmPythonViewIter;

class cmPythonViewIterSrc
{
public:
    virtual ~cmPythonViewIterSrc() = default;
protected:
    virtual pybind11::object get(unsigned int idx) = 0;
    virtual unsigned int size() const = 0;
    friend cmPythonViewIter;
};

class cmPythonViewIter
{
public:
    static void bind(pybind11::module_& m);
    cmPythonViewIter(cmPythonViewIterSrc& view);

protected:
    pybind11::object next();
    cmPythonViewIter iter();

    unsigned int Current = 0;
    cmPythonViewIterSrc& View;
};

