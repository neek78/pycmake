
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
    MessageType mtype = makefile.ExpandVariablesInString(errStr, out);
    std::cerr << "err |" << errStr << "| out |" << out << "| mytype |" 
        << (int)mtype << "|\n"; 

    if (mtype != MessageType::LOG) {
        py::str err = "error during variable expansion - "_pys + py::str(errStr);
        throw py::value_error(err);
    }

    return out;
}

