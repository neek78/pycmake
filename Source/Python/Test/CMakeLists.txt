
message(STATUS "Unit tests enabled")

enable_testing()

add_executable(
  unitTestExec 
  pyTest.cxx
  cacheTests.cxx
  cacheEntryTests.cxx
  typeConversionTests.cxx
  )

target_link_libraries(
  unitTestExec 
  CMakeLib
  GTest::gtest 
  GTest::gtest_main
  )

gtest_discover_tests(unitTestExec)

# add a target to run just the tests
add_custom_target(unit-test-all 
  COMMAND unitTestExec 
  DEPENDS unitTestExec
  USES_TERMINAL # enable fancy-pants coloured output
  )

