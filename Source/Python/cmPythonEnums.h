/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

void BindEnums(pybind11::module_& m);

enum class CMakeScriptType 
{
    CMakeScript,
    Python
};

