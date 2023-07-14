/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <string>
#include <filesystem>

// This file is always included, even if python support is disabled.. 
// SO don't put anything in here that actually relies on python

// name of the user python script file - equivalent of CMakeLists.txt
const std::string PYTHON_SCRIPT_NAME = "cmake_build.py";

const std::filesystem::path PYTHON_MOD_DIR = "PyModules";
const std::string PYTHON_CMAKE_MOD = "cmake";

const std::string PYTHON_BUILTIN_MOD = "_builtin";
const std::string PYTHON_BUILTIN_MOD_FULL = PYTHON_CMAKE_MOD + "." + PYTHON_BUILTIN_MOD; 

const std::string PYTHON_CLIENT_MOD = "_client";
const std::string PYTHON_CLIENT_MOD_FULL = PYTHON_CMAKE_MOD + "." + PYTHON_CLIENT_MOD; 
