/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#include "cmVersion.h"

#include "cmVersionConfig.h"

unsigned int cmVersion::GetMajorVersion()
{
  return CMake_VERSION_MAJOR;
}
unsigned int cmVersion::GetMinorVersion()
{
  return CMake_VERSION_MINOR;
}
unsigned int cmVersion::GetPatchVersion()
{
  return CMake_VERSION_PATCH;
}
unsigned int cmVersion::GetTweakVersion()
{
  return 0;
}
const char* cmVersion::GetCMakeVersion()
{
  return CMake_VERSION;
}

const char* cmVersion::GetSuffix()
{
#ifdef CMake_VERSION_SUFFIX
  return CMake_VERSION_SUFFIX;
#else
  return 0;
#endif
}

bool cmVersion::GetVersionIsDirty()
{
#ifdef CMake_VERSION_IS_DIRTY 
  return (CMake_VERSION_IS_DIRTY == 1);
#else
  return false;
#endif
}
