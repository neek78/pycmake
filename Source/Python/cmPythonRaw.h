/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonDispatcher.h"
#include "cmPythonTypes.h"
#include "cmState.h"

#include <pybind11/pybind11.h>

class cmExecutionStatus;
struct cmListFileArgument;

class cmPythonRaw : public cmPythonDispatcher
{
public:
  static void bind(pybind11::module_& m);

  cmPythonRaw(cmMakefile& makefile);
  cmPythonRaw(const cmPythonRaw&) = delete; 
  cmPythonRaw& operator=(const cmPythonRaw&) = delete;

  bool GetSeenProjectCommand() const { return SeenProjectCommand; }
  bool GetSeenMinVersionRequiredCommand() const { return SeenMinVersionRequiredCommand; }

private:
  using ClassType = pybind11::class_<cmPythonRaw>;

  template<typename FunctionType>
  pybind11::object invokeFunction(const FunctionType& function, 
        const std::string_view& fnName, const pybind11::args& args);

  template<typename FunctionType>
  static void build(ClassType* cls, const std::string& fnName, const FunctionType& function);

  void checkCommands(const cmState::BuiltinCommand& f);
  void checkCommands(const cmState::Command&);

  bool SeenProjectCommand = false;
  bool SeenMinVersionRequiredCommand = false;
};
