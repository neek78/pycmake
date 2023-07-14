/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

#include <string>

class cmPythonVarChecks
{
protected:
    // checks for set methods
    void setCommonChecks(
        const pybind11::str& var,
        const pybind11::str& val);

    // common checks for all set/unset calls
    void commonChecks(
        const pybind11::str& var);
};
