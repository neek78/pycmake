/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

#include <filesystem>
#include <iostream>

class cmPythonCore
{
public:
  static void AppendPythonPath(const std::filesystem::path& path);

  // drop into an ipython interactive session for debugging
  static void ipython();

  // construct and init. Need to do this in two phases due to exception melarky
  cmPythonCore() = default;

  // ... 2nd phase of init - load and build the python code from cmake.py
  // and bind the c++ code
  // this most certainly can throw, so has to be done after the scoped_interpreter is up
  bool init();

  bool PrintPythonInfo(std::wostream& os);

  static std::filesystem::path GetModulePath();
  static std::string GetModuleName();

  void DisableBytecodeCache();

private:
  void buildRootModule();

  pybind11::scoped_interpreter Guard;

  // we hold the next two here to keep them alive - they're more conveniently
  // accessed through the cmPythonModules interface
  // The "root" module of python-implemented code (ie cmake.py)
  pybind11::module_ RootModule;

  // the module containing all our c++ implemented code visible to python.
  // we keep this alive to avoid re-binding things, and also allowing access
  // to class objects (etc)
  pybind11::module_ BuiltinModule;
};
