
/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

#include <string>
#include <optional>

// this is a convenience wrapper around a cmake.kwarg object. this is generally
// short lived and stores no state of its own.
class cmPythonKwArg
{
public:
    cmPythonKwArg(const pybind11::handle& KwArg);
    bool GetEnabled() const;

private:
    // the python object we wrap
    pybind11::object PyKwArg;
};

