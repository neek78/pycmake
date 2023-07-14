/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/pybind11.h>
#include <cassert>

class cmMakefile;

// not sure whether to keep this around - it does a lot less than originally intended
class cmPythonModBase
{
protected:
    cmPythonModBase(cmMakefile& makefile)
    :   Makefile(makefile)
    {
    }

    cmMakefile& GetMakefile()
    {
        return Makefile;
    }

    const cmMakefile& GetMakefile() const
    {
        return Makefile;
    }

private:
    cmMakefile& Makefile;
};

