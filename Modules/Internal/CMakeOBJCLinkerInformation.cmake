# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.


# This file sets the basic flags for the linker used by the Objective-C compiler in CMake.
# It also loads the available platform file for the system-linker
# if it exists.
# It also loads a system - linker - processor (or target hardware)
# specific file, which is mainly useful for crosscompiling and embedded systems.

include(Internal/CMakeCommonLinkerInformation)

set(_INCLUDED_FILE 0)

# Load linker-specific information.
if(CMAKE_OBJC_COMPILER_LINKER_ID)
  include(Linker/${CMAKE_OBJC_COMPILER_LINKER_ID}-OBJC OPTIONAL)
endif()

# load a hardware specific file, mostly useful for embedded compilers
if(CMAKE_SYSTEM_PROCESSOR AND CMAKE_OBJC_COMPILER_LINKER_ID)
    include(Platform/${CMAKE_EFFECTIVE_SYSTEM_NAME}-${CMAKE_OBJC_COMPILER_LINKER_ID}-OBJC-${CMAKE_SYSTEM_PROCESSOR} OPTIONAL RESULT_VARIABLE _INCLUDED_FILE)
endif()


# load the system- and linker specific files
if(CMAKE_OBJC_COMPILER_LINKER_ID)
  include(Platform/Linker/${CMAKE_EFFECTIVE_SYSTEM_NAME}-${CMAKE_OBJC_COMPILER_LINKER_ID}-OBJC
    OPTIONAL RESULT_VARIABLE _INCLUDED_FILE)
endif()

# We specify the platform linker information in the system file.
if (NOT _INCLUDED_FILE)
  include(Platform/Linker/${CMAKE_SYSTEM_NAME}-OBJC OPTIONAL)
endif ()

_cmake_common_linker_platform_flags(OBJC)

set(CMAKE_OBJC_LINKER_INFORMATION_LOADED 1)
