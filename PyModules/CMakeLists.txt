# just add our unit tests to CTest

add_test (NAME python-tests
  COMMAND ${PYTHON_EXECUTABLE} -m unittest 
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

