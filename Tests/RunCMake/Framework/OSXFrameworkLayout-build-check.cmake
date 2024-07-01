include("${RunCMake_TEST_BINARY_DIR}/FrameworkName.cmake")
set(framework-resources "${framework-dir}/Resources")
set(framework-resource-file "${framework-resources}/res.txt")
set(framework-flat-resource-file "${framework-resources}/flatresource.txt")
set(framework-deep-resource-file "${framework-resources}/deep/deepresource.txt")
set(framework-library "${framework-dir}/Framework")
set(framework-versions "${framework-dir}/Versions")
set(framework-some-file "${framework-versions}/Current/somedir/some.txt")
set(plist-file "${framework-resources}/Info.plist")
set(framework-header "${framework-dir}/Headers/foo.h")

if(NOT IS_DIRECTORY ${framework-dir})
  set(RunCMake_TEST_FAILED "Framework not found at \n  ${framework-dir}")
  return()
endif()

if(NOT EXISTS ${plist-file})
  set(RunCMake_TEST_FAILED "plist file not found at \n  ${plist-file}")
  return()
endif()

if(NOT EXISTS ${framework-library})
  set(RunCMake_TEST_FAILED "Framework library not found at \n  ${framework-library}")
  return()
endif()

if(NOT EXISTS ${framework-resource-file})
  set(RunCMake_TEST_FAILED "Framework resource file not found at \n  ${framework-resource-file}")
  return()
endif()

if(NOT EXISTS ${framework-flat-resource-file})
  set(RunCMake_TEST_FAILED "Framework flat resource file not found at \n  ${framework-flat-resource-file}")
  return()
endif()

if(NOT EXISTS ${framework-deep-resource-file})
  set(RunCMake_TEST_FAILED "Framework deep resource file not found at \n  ${framework-deep-resource-file}")
  return()
endif()

if(NOT EXISTS ${framework-some-file})
  set(RunCMake_TEST_FAILED "Framework some file not found at \n  ${framework-some-file}")
  return()
endif()

if(NOT EXISTS ${framework-versions})
  set(RunCMake_TEST_FAILED "Framework versions not found at \n  ${framework-versions}")
  return()
endif()

if(NOT EXISTS ${framework-resources})
  set(RunCMake_TEST_FAILED "Framework Resources not found at \n  ${framework-resources}")
  return()
endif()

if(NOT EXISTS ${framework-header})
  set(RunCMake_TEST_FAILED "Framework header file not found at \n  ${framework-header}")
  return()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/FrameworkLayout-check-common.cmake)
