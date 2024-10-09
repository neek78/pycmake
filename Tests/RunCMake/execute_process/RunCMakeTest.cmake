include(RunCMake)

set(RunCMake_TEST_OUTPUT_MERGE 1)
run_cmake_command(MergeOutput ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/MergeOutput.cmake)
unset(RunCMake_TEST_OUTPUT_MERGE)

run_cmake_command(MergeOutputFile ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/MergeOutputFile.cmake)
run_cmake_command(MergeOutputVars ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/MergeOutputVars.cmake)

run_cmake(EncodingMissing)
run_cmake(EncodingUnknown)
if(TEST_ENCODING_EXE)
  run_cmake_script(EncodingCMP0176-NEW -DTEST_ENCODING_EXE=${TEST_ENCODING_EXE})
  run_cmake_script(EncodingCMP0176-OLD -DTEST_ENCODING_EXE=${TEST_ENCODING_EXE})
  run_cmake_script(EncodingAUTO -DTEST_ENCODING_EXE=${TEST_ENCODING_EXE})
  run_cmake_script(EncodingUTF-8 -DTEST_ENCODING_EXE=${TEST_ENCODING_EXE})
  run_cmake_script(EncodingUTF8 -DTEST_ENCODING_EXE=${TEST_ENCODING_EXE})
endif()

if(EXIT_CODE_EXE)
  run_cmake_command(ExitValues ${CMAKE_COMMAND} -DEXIT_CODE_EXE=${EXIT_CODE_EXE} -P ${RunCMake_SOURCE_DIR}/ExitValues.cmake)
endif()

run_cmake_command(EchoCommand ${CMAKE_COMMAND} -DCHECK_GLOBAL=TRUE
  -P ${RunCMake_SOURCE_DIR}/EchoCommand.cmake)
run_cmake_command(EchoCommand2 ${CMAKE_COMMAND} -P
  ${RunCMake_SOURCE_DIR}/EchoCommand.cmake)
run_cmake_command(EchoCommand3 ${CMAKE_COMMAND}
  -DCHECK_ERROR_OUTPUT_LOCATION=TRUE -P
  ${RunCMake_SOURCE_DIR}/EchoCommand.cmake)

run_cmake_command(EchoVariable ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/EchoVariable.cmake)

run_cmake_command(CommandError ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/CommandError.cmake)
run_cmake_command(AnyCommandError ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/AnyCommandError.cmake)
run_cmake_command(AnyCommandTimeout ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/AnyCommandTimeout.cmake)
run_cmake_command(AnyCommandGood ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/AnyCommandGood.cmake)
run_cmake_command(LastCommandError ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/LastCommandError.cmake)
run_cmake_command(LastCommandTimeout ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/LastCommandTimeout.cmake)
run_cmake_command(LastCommandGood ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/LastCommandGood.cmake)
run_cmake_command(Stdin ${CMAKE_COMMAND} -DPRINT_STDIN_EXE=${PRINT_STDIN_EXE} -P ${RunCMake_SOURCE_DIR}/Stdin.cmake)
run_cmake_command(StdinNoexist ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/StdinNoexist.cmake)
run_cmake_command(StdoutNoexist ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/StdoutNoexist.cmake)
run_cmake_command(StderrNoexist ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/StderrNoexist.cmake)
run_cmake_command(StdoutStderrNoexist ${CMAKE_COMMAND} -P ${RunCMake_SOURCE_DIR}/StderrNoexist.cmake)
run_cmake_command(InOutErrDirectory ${CMAKE_COMMAND} -DPRINT_STDIN_EXE=${PRINT_STDIN_EXE} -P ${RunCMake_SOURCE_DIR}/InOutErrDirectory.cmake)

if(EXIT_CRASH_EXE)
  run_cmake_command(AnyCommandAbnormalExit ${CMAKE_COMMAND} -DEXIT_CRASH_EXE=${EXIT_CRASH_EXE} -P ${RunCMake_SOURCE_DIR}/AnyCommandAbnormalExit.cmake)
  run_cmake_command(LastCommandAbnormalExit-1 ${CMAKE_COMMAND} -DEXIT_CRASH_EXE=${EXIT_CRASH_EXE} -P ${RunCMake_SOURCE_DIR}/LastCommandAbnormalExit-1.cmake)
  run_cmake_command(LastCommandAbnormalExit-2 ${CMAKE_COMMAND} -DEXIT_CRASH_EXE=${EXIT_CRASH_EXE} -P ${RunCMake_SOURCE_DIR}/LastCommandAbnormalExit-2.cmake)
endif()

if(WIN32 OR CYGWIN)
  set(RunCMake_TEST_BINARY_DIR ${RunCMake_BINARY_DIR}/WindowsNoExtension-build)
  run_cmake(WindowsNoExtension)
  set(RunCMake_TEST_NO_CLEAN 1)
  run_cmake_command(WindowsNoExtension-build ${CMAKE_COMMAND} --build . --config Debug --target RunScript)
endif()

if(TEST_STARTUPINFO_EXE)
  run_cmake_script(StartupInfo -DTEST_STARTUPINFO_EXE=${TEST_STARTUPINFO_EXE})
endif()
