# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.


# This module is shared by multiple languages; use include blocker.
include_guard()


include(Platform/Linker/Linux-GNU)


macro(__linux_linker_lld lang)
  __linux_linker_gnu(${lang})

  set(CMAKE_${lang}_LINK_LIBRARIES_PROCESSING ORDER=REVERSE DEDUPLICATION=ALL)
endmacro()
