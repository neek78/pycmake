# just add our unit tests to CTest

message(STATUS "Python_EXECUTABLE is ${Python_EXECUTABLE}")

add_test (NAME python-tests
  COMMAND ${Python_EXECUTABLE} -m unittest 
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

