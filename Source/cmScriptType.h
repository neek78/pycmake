/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file LICENSE.rst or https://cmake.org/licensing for details.  */
#pragma once

#include <string>
#include <optional>

enum class cmScriptType {
  Native,
  Python
};

std::string GetScriptName(cmScriptType t);

std::string ScriptTypeToString(cmScriptType t);

std::optional<cmScriptType> ScriptTypeFromString(const std::string& str);

std::string GetFullScriptPath(const std::string& dir, cmScriptType t);

std::optional<cmScriptType> DetectScriptType(const std::string& dir);

std::optional<cmScriptType> ScriptTypeFromStdFilename(const std::string& fn);

// try and detect a script by name - otherwise return classic CMakeList.txt
std::string TryDetectScriptType(const std::string& dir);
