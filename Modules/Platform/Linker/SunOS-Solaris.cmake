# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.


# This module is shared by multiple languages; use include blocker.
include_guard()

block(SCOPE_FOR POLICIES)
cmake_policy(SET CMP0054 NEW)

macro(__sunos_linker_solaris lang)
  # Features for LINK_LIBRARY generator expression
  ## WHOLE_ARCHIVE: Force loading all members of an archive
  if (CMAKE_SYSTEM_VERSION VERSION_GREATER "5.10")
    set(CMAKE_${lang}_LINK_LIBRARY_USING_WHOLE_ARCHIVE "LINKER:--whole-archive"
                                                       "<LINK_ITEM>"
                                                       "LINKER:--no-whole-archive")
  else()
    set(CMAKE_${lang}_LINK_LIBRARY_USING_WHOLE_ARCHIVE "LINKER:-z,allextract"
                                                       "<LINK_ITEM>"
                                                       "LINKER:-z,defaultextract")
  endif()
  set(CMAKE_${lang}_LINK_LIBRARY_USING_WHOLE_ARCHIVE_SUPPORTED TRUE)
  set(CMAKE_${lang}_LINK_LIBRARY_WHOLE_ARCHIVE_ATTRIBUTES LIBRARY_TYPE=STATIC DEDUPLICATION=YES OVERRIDE=DEFAULT)
endmacro()

endblock()
