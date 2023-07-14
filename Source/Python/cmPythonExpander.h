/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <vector>
#include <string>

class cmMakefile;

// expand vars for python use. 
std::string ExpandVar(
        const cmMakefile& makefile,
        const std::string& in,
        bool strict);

