/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file LICENSE.rst or https://cmake.org/licensing for details.  */

#include "cmScriptType.h"
#include "cmStringAlgorithms.h"
#include "cmSystemTools.h"
#include "Python/cmPythonConstants.h"

std::string GetScriptName(cmScriptType t)
{
  switch(t) {
    case cmScriptType::Native:  return "CMakeLists.txt";
    case cmScriptType::Python:  return PYTHON_SCRIPT_NAME;
  };
}

std::optional<cmScriptType> ScriptTypeFromString(const std::string& str)
{
  if (str == "Native") {
      return cmScriptType::Native;
  } 
  if (str == "Python") {
      return cmScriptType::Python;
  }
  return {};
}

std::string ScriptTypeToString(cmScriptType t)
{
  switch(t) {
    case cmScriptType::Native:  return "Native";
    case cmScriptType::Python:  return "Python";
  };
}
std::string GetFullScriptPath(const std::string& dir, cmScriptType t)
{
  return cmStrCat(dir, "/", GetScriptName(t));
}

std::optional<cmScriptType> DetectScriptType(const std::string& dir) 
{
  if (cmSystemTools::FileExists(GetFullScriptPath(dir, cmScriptType::Native))) {
    return cmScriptType::Native;
  }

  if (cmSystemTools::FileExists(GetFullScriptPath(dir, cmScriptType::Python))) {
    return cmScriptType::Python;
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

