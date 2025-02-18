
#include "cmPythonExpander.h"
#include "cmPythonUtils.h"
#include "cmMakefile.h"

#include <pybind11/embed.h>
#include <iostream>

namespace py = pybind11;

std::string ExpandVar(
        const cmMakefile& makefile,
        const std::string& in,
        bool strict)
{
    using namespace literals;
    std::string out = in;
    std::string errStr;

    // this expands in place - ie output is in value
    // FIXME: fill file/line
    // FIXME: reinstate error handling
#if 0
    MessageType mtype = makefile.ExpandVariablesInStringNew(errStr, out, false,
                             false, false, "FILENAME", 0, false, strict);

    if (mtype != MessageType::LOG) {
        py::str err = "error during variable expansion - "_pys + py::str(errStr);
        throw py::value_error(err);
    }
#endif

    makefile.ExpandVariablesInString(out, false, false);
    return out;
}

