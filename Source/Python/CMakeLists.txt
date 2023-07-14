
target_sources(
  CMakeLib
    PRIVATE
    cmPythonBuiltinModule.cxx
    cmPythonBuiltinModule.h
    cmPythonCache.cxx
    cmPythonCache.h
    cmPythonCache.h
    cmPythonCacheEntry.cxx
    cmPythonCacheEntry.h
    cmPythonCombinedVar.cxx
    cmPythonCombinedVar.h
    cmPythonCommands.cxx
    cmPythonCommands.h
    cmPythonCore.cxx
    cmPythonCore.h
    cmPythonDictInterface.cxx
    cmPythonDictInterface.h
    cmPythonDictView.cxx
    cmPythonDictView.h
    cmPythonDirAttr.cxx
    cmPythonDirAttr.h
    cmPythonDispatcher.cxx
    cmPythonDispatcher.h
    cmPythonEnums.cxx
    cmPythonEnums.h
    cmPythonExpander.h
    cmPythonExpander.cxx
    cmPythonFactory.cxx
    cmPythonFactory.h
    cmPythonFileSet.cxx
    cmPythonFileSet.h
    cmPythonFunctionLauncher.cxx
    cmPythonFunctionLauncher.h
    cmPythonGlobalAttr.cxx
    cmPythonGlobalAttr.h
    cmPythonKwArg.h
    cmPythonKwArg.cxx
    cmPythonModBase.cxx
    cmPythonModBase.h
    cmPythonModules.cxx
    cmPythonModules.h
    cmPythonPyFunctions.cxx
    cmPythonPyFunctions.h
    cmPythonRaw.cxx
    cmPythonRaw.h
    cmPythonReturnParam.cxx
    cmPythonReturnParam.h
    cmPythonScript.cxx
    cmPythonScript.h
    cmPythonSourceDirs.cxx
    cmPythonSourceDirs.h
    cmPythonSourceFile.cxx
    cmPythonSourceFile.h
    cmPythonTarget.cxx
    cmPythonTarget.h
    cmPythonTypeConversion.cxx
    cmPythonTypeConversion.h
    cmPythonTypes.h
    cmPythonUtils.cxx
    cmPythonUtils.h
    cmPythonVar.cxx
    cmPythonVar.h
    cmPythonVarChecks.cxx
    cmPythonVarChecks.h
    cmPythonViewIter.cxx
    cmPythonViewIter.h
)

target_link_libraries(CMakeLib PUBLIC pybind11::embed)

if(GTest_FOUND)
  add_subdirectory(Test)
endif()

# fix visibility warnings when compiling with gcc
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(CMakeLib PUBLIC -fvisibility=hidden)
endif()
