# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include_guard()


block(SCOPE_FOR POLICIES)
cmake_policy(SET CMP0054 NEW)

macro(__linker_lld lang)
  if(CMAKE_${lang}_COMPILER_LINKER_FRONTEND_VARIANT STREQUAL "MSVC")
    include(Linker/MSVC)

    __linker_msvc(${lang})
  else()
    include(Linker/GNU)

    __linker_gnu(${lang})
  endif()
endmacro()

endblock()
