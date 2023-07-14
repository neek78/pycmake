
cmake_minimum_required(VERSION 3.26)

project(pyFunctionTest)

function(FAILED testname)
  message(SEND_ERROR "${testname} failed ${ARGN}")
endfunction()

function(PASS testname)
  message("${testname} passed ${ARGN}")
endfunction()

# import functions implemented in python
add_subdirectory(py_dir)

# =======================================================
some_function("hello python")
# if we got here, I guess it didn't fail
PASS("simple_call")

# =======================================================
old_fashioned_return(4 RET1)

if(RET1 EQUAL 8)
  PASS("old_fashone_return")
else()
  FAILED("old_fashone_return RET1 Got: ${RET1}")
endif()

# =======================================================
single_return_value(RET1)

if(RET1 EQUAL 57)
  PASS("single_return_value")
else()
  FAILED("single_return_value RET1 Got: ${RET1}")
endif()

# =======================================================

complicated_mathematics(15 RET2)

if(RET2 EQUAL "16")
  PASS("complicated_mathematics")
else()
  FAILED("RET2 Got: ${RET2}")
endif()

# =======================================================

two_return_values(RET3 RET4)

if(RET3 STREQUAL "cats" AND RET4 STREQUAL "dogs")
  PASS("two_return_values")
else()
  FAILED("two_return_values Got: ${RET3} ${RET4}")
endif()


# =======================================================
# test registering another function whilst in a registered function
register_another_function()
final_destination(RET5)

if(RET5 STREQUAL "secret_message")
  PASS("register_another_function")
else()
  FAILED("register_another_function Got: ${RET3} ${RET4}")
endif()

#=======================================================
# call cmake -> python -> cmake
function(CALL_ME RET)
  message(STATUS "IN CALL_ME")
  set(${RET} "anytime" PARENT_SCOPE)
endfunction()

call_me_back(CALL_ME RET6)

if(RET6 STREQUAL "anytime")
  PASS("call_me_back")
else()
  FAILED("call_me_back Got: |${RET6}|")
endif()

#=======================================================
#raise_exception()

#bad_annotations_function("AHOY"  RET5)
# TODO: 
# - catch bad annotations
# test optinoal return values
# check mismatching return values / annotations
# how do you check something failed sucessfully?
