/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file LICENSE.rst or https://cmake.org/licensing for details.  */

#include "cmScriptType.h"
#include "cmStringAlgorithms.h"
#include "cmSystemTools.h"
#include "Python/cmPythonConstants.h"

std::string GetScriptName(ScriptType t)
{
  switch(t) {
    case ScriptType::Native:  return "CMakeLists.txt";
    case ScriptType::Python:  return PYTHON_SCRIPT_NAME;
  };
}

std::string GetFullScriptPath(const std::string& dir, ScriptType t)
{
  return cmStrCat(dir, "/", GetScriptName(t));
}

std::optional<ScriptType> DetectScriptType(const std::string& dir) 
{
  if (cmSystemTools::FileExists(GetFullScriptPath(dir, ScriptType::Native))) {
    return ScriptType::Native;
  }

  if (cmSystemTools::FileExists(GetFullScriptPath(dir, ScriptType::Python))) {
    return ScriptType::Python;
  }

  return {};
}

// try and detect a script by name - otherwise return classic CMakeList.txt
std::string TryDetectScriptType(const std::string& dir) 
{
    auto t = DetectScriptType(dir);
    if(t) {
        return GetScriptName(*t);
    }
    return "CMakeLists.txt";

}
