/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmState.h"

#include <functional>
#include <string>

template<typename T>
using CmdFunction = std::function<void (const std::string& fnName, T fn)>;

// FIXME: need better name
using OtherCmdFunction = CmdFunction<cmState::Command>;
using BuiltinCmdFunction = CmdFunction<cmState::BuiltinCommand>;

void buildPythonCommands(const OtherCmdFunction& build);
void buildPythonCommands(const BuiltinCmdFunction& build);

