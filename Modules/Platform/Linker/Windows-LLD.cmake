# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.


# This module is shared by multiple languages; use include blocker.
include_guard()

block(SCOPE_FOR POLICIES)
cmake_policy(SET CMP0054 NEW)

macro(__windows_linker_lld lang)
  # Features for LINK_LIBRARY generator expression
  if(CMAKE_${lang}_COMPILER_LINKER_FRONTEND_VARIANT STREQUAL "GNU")
    include(Platform/Linker/Windows-GNU)
    __windows_linker_gnu(${lang})

    set(CMAKE_${lang}_LINK_LIBRARIES_PROCESSING ORDER=FORWARD DEDUPLICATION=ALL)
  else()
    include(Platform/Linker/Windows-MSVC)
    __windows_linker_msvc(${lang})
  endif()
endmacro()

endblock()
