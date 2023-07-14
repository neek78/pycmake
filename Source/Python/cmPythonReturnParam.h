
/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

#include <string>
#include <optional>

// this is a convenience wrapper around a cmake.ReturnParam object. this is generally
// short lived and stores no state of its own.
class cmPythonReturnParam
{
public:
    cmPythonReturnParam(const pybind11::handle& ReturnParam);

    std::optional<std::string> GetDefaultValue() const;
    std::string GetName() const;
    bool GetAutoCleanup() const;
    bool GetRaiseOnOverwrite() const;
    bool ShouldRaiseOnUnset() const;

    void SetArgPosition(int pos);
    std::optional<int> GetArgPosition () const;

private:
    // the python object we wrap
    pybind11::object PyReturnParam;
};

