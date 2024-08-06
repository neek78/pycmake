# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include(Platform/Linker/BSD-Linker-Initialize)

block(SCOPE_FOR POLICIES)
cmake_policy(SET CMP0054 NEW)

if(_CMAKE_SYSTEM_LINKER_TYPE STREQUAL "GNU")
  include(Platform/Linker/OpenBSD-GNU-C)
else()
  include(Platform/Linker/OpenBSD-LLD-C)
endif()

endblock()
