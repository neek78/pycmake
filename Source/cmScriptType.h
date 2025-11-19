/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file LICENSE.rst or https://cmake.org/licensing for details.  */
#pragma once

#include <string>
#include <optional>

enum class ScriptType {
  Native,
  Python
};

std::string GetScriptName(ScriptType t);

std::string GetFullScriptPath(const std::string& dir, ScriptType t);

std::optional<ScriptType> DetectScriptType(const std::string& dir);

// try and detect a script by name - otherwise return classic CMakeList.txt
std::string TryDetectScriptType(const std::string& dir);
