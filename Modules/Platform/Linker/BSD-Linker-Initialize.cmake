# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

block(SCOPE_FOR POLICIES)
cmake_policy(SET CMP0054 NEW)

if(NOT _CMAKE_SYSTEM_LINKER_TYPE)
  block(SCOPE_FOR VARIABLES)
    execute_process(COMMAND "${CMAKE_LINKER}" --version
                    RESULT_VARIABLE result
                    OUTPUT_VARIABLE output
                    ERROR_VARIABLE output)
    if(result OR NOT output MATCHES "LLD")
      # assume GNU as default linker
      set(_CMAKE_SYSTEM_LINKER_TYPE GNU CACHE INTERNAL "System linker type")
    else()
      set(_CMAKE_SYSTEM_LINKER_TYPE LLD CACHE INTERNAL "System linker type")
    endif()
  endblock()
endif()

endblock()
