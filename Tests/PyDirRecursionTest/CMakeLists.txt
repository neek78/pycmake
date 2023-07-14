
cmake_minimum_required(VERSION 3.26)

project(pyFunctionTest)

function(FAILED testname)
  message(SEND_ERROR "${testname} failed ${ARGN}")
endfunction()

function(PASS testname)
  message("${testname} passed ${ARGN}")
endfunction()

# import functions implemented in python
add_subdirectory(dir1)

get_target_property(VAR test_target NAME)

if(VAR STREQUAL "test_target")
  PASS("target_create_test")
else()
  FAILED("target_create_test ${VAR}")
endif()

if(BUBBLE_UP STREQUAL "suds")
  PASS("bubble_up_test")
else()
  FAILED("bubble_up_test ${BUBBLE_UP}")
endif()

if(BUBBLE_UP2 STREQUAL "suds2")
  PASS("bubble_up_test2")
else()
  FAILED("bubble_up_test2 ${BUBBLE_UP2}")
endif()
