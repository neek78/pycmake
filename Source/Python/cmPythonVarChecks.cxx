/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonVarChecks.h"

#include "cmStringAlgorithms.h"

namespace py = pybind11;

void cmPythonVarChecks::commonChecks(
        const py::str& var)
{
    if (py::len(var) == 0) {
        throw py::value_error("variable name cannot be blank");
    }

    // we don't support setting environment variables here
    // FIXME: can we do this without the conversion to str?
    if (cmHasLiteralPrefix(std::string(var), "ENV{")) {
        throw py::value_error("setting environment variables is not supported through this interface");
    }
}

void cmPythonVarChecks::setCommonChecks(
        const py::str& var,
        const py::str& val)
{
    commonChecks(var);

    // nor do we support unsetting variables here. 
    if (val.is_none() || py::len(val) == 0) {
        throw py::value_error("variables cannot be set to None or a blank string. Use unset() instead");
    }
}

