/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonCommands.h"

// include ALL the things!
#include "cmAddCompileDefinitionsCommand.h"
#include "cmAddCustomCommandCommand.h"
#include "cmAddCustomTargetCommand.h"
#include "cmAddDefinitionsCommand.h"
#include "cmAddDependenciesCommand.h"
#include "cmAddExecutableCommand.h"
#include "cmAddLibraryCommand.h"
#include "cmAddSubDirectoryCommand.h"
#include "cmAddTestCommand.h"
#include "cmBuildCommand.h"
#include "cmCMakeLanguageCommand.h"
#include "cmCMakeMinimumRequired.h"
#include "cmCMakePathCommand.h"
#include "cmCMakePolicyCommand.h"
#include "cmConfigureFileCommand.h"
#include "cmCreateTestSourceList.h"
#include "cmDefinePropertyCommand.h"
#include "cmEnableLanguageCommand.h"
#include "cmEnableTestingCommand.h"
#include "cmExecProgramCommand.h"
#include "cmExecuteProcessCommand.h"
#include "cmFileCommand.h"
#include "cmFindFileCommand.h"
#include "cmFindLibraryCommand.h"
#include "cmFindPackageCommand.h"
#include "cmFindPathCommand.h"
#include "cmFindProgramCommand.h"
#include "cmGetCMakePropertyCommand.h"
#include "cmGetDirectoryPropertyCommand.h"
#include "cmGetFilenameComponentCommand.h"
#include "cmGetPropertyCommand.h"
#include "cmGetSourceFilePropertyCommand.h"
#include "cmGetTargetPropertyCommand.h"
#include "cmGetTestPropertyCommand.h"
#include "cmIncludeDirectoryCommand.h"
#include "cmIncludeGuardCommand.h"
#include "cmIncludeRegularExpressionCommand.h"
#include "cmInstallCommand.h"
#include "cmInstallFilesCommand.h"
#include "cmInstallTargetsCommand.h"
#include "cmLinkDirectoriesCommand.h"
#include "cmListCommand.h"
#include "cmMacroCommand.h"
#include "cmMakeDirectoryCommand.h"
#include "cmMarkAsAdvancedCommand.h"
#include "cmMathCommand.h"
#include "cmMessageCommand.h"
#include "cmOptionCommand.h"
#include "cmParseArgumentsCommand.h"
#include "cmPolicies.h"
#include "cmProjectCommand.h"
#include "cmReturnCommand.h"
#include "cmSeparateArgumentsCommand.h"
#include "cmSetCommand.h"
#include "cmSetDirectoryPropertiesCommand.h"
#include "cmSetPropertyCommand.h"
#include "cmSetSourceFilesPropertiesCommand.h"
#include "cmSetTargetPropertiesCommand.h"
#include "cmSetTestsPropertiesCommand.h"
#include "cmSiteNameCommand.h"
#include "cmStringCommand.h"
#include "cmSubdirCommand.h"
#include "cmTargetCompileDefinitionsCommand.h"
#include "cmTargetCompileFeaturesCommand.h"
#include "cmTargetCompileOptionsCommand.h"
#include "cmTargetIncludeDirectoriesCommand.h"
#include "cmTargetLinkLibrariesCommand.h"
#include "cmTargetLinkOptionsCommand.h"
#include "cmTargetPrecompileHeadersCommand.h"
#include "cmTargetSourcesCommand.h"
#include "cmTryCompileCommand.h"
#include "cmTryRunCommand.h"
#include "cmUnsetCommand.h"

#include "cmAddCompileOptionsCommand.h"
#include "cmAddLinkOptionsCommand.h"
#include "cmCMakeHostSystemInformationCommand.h"
#include "cmExportCommand.h"
#include "cmFLTKWrapUICommand.h"
#include "cmIncludeExternalMSProjectCommand.h"
#include "cmInstallProgramsCommand.h"
#include "cmLinkLibrariesCommand.h"
#include "cmLoadCacheCommand.h"
#include "cmQTWrapCPPCommand.h"
#include "cmQTWrapUICommand.h"
#include "cmRemoveCommand.h"
#include "cmRemoveDefinitionsCommand.h"
#include "cmSourceGroupCommand.h"
#include "cmTargetLinkDirectoriesCommand.h"
#include "cmVariableWatchCommand.h"
#include "cmWriteFileCommand.h"

#include "cmIncludeCommand.h"

void buildPythonCommands(const OtherCmdFunction& build)
{
  // yes! one command has a different signature. and there was much rejoicing
  build("cmake_language", cmCMakeLanguageCommand);
}

void buildPythonCommands(const BuiltinCmdFunction& build)
{
  build("include", cmIncludeCommand);
 
  build("add_compile_definitions", cmAddCompileDefinitionsCommand);
  build("add_compile_options", cmAddCompileOptionsCommand);
  build("add_custom_command", cmAddCustomCommandCommand);
  build("add_custom_target", cmAddCustomTargetCommand);
  build("add_definitions", cmAddDefinitionsCommand);
  build("add_dependencies", cmAddDependenciesCommand);
  build("add_executable", cmAddExecutableCommand);
  build("add_library", cmAddLibraryCommand);
  build("add_link_options", cmAddLinkOptionsCommand);
  build("add_subdirectory", cmAddSubDirectoryCommand);
  build("add_test", cmAddTestCommand);
  build("build_command", cmBuildCommand);
  build("cmake_host_system_information", cmCMakeHostSystemInformationCommand);
  build("cmake_minimum_required", cmCMakeMinimumRequired);
  build("cmake_parse_arguments", cmParseArgumentsCommand);
  build("cmake_path", cmCMakePathCommand);
  build("cmake_policy", cmCMakePolicyCommand);
  build("configure_file", cmConfigureFileCommand);
  build("create_test_sourcelist", cmCreateTestSourceList);
  build("define_property", cmDefinePropertyCommand);
  build("enable_language", cmEnableLanguageCommand);
  build("enable_testing", cmEnableTestingCommand);
  build("exec_program", cmExecProgramCommand);
  build("execute_process", cmExecuteProcessCommand);
  build("export", cmExportCommand);
  build("file", cmFileCommand);
  build("find_file", cmFindFile);
  build("find_library", cmFindLibrary);
  build("find_package", cmFindPackage);
  build("find_path", cmFindPath);
  build("find_program", cmFindProgram);
  build("fltk_wrap_ui", cmFLTKWrapUICommand);
  build("get_cmake_property", cmGetCMakePropertyCommand);
  build("get_directory_property", cmGetDirectoryPropertyCommand);
  build("get_filename_component", cmGetFilenameComponentCommand);
  build("get_property", cmGetPropertyCommand);
  build("get_source_file_property", cmGetSourceFilePropertyCommand);
  build("get_target_property", cmGetTargetPropertyCommand);
  build("get_test_property", cmGetTestPropertyCommand);
  build("include_directories", cmIncludeDirectoryCommand);
  build("include_external_msproject", cmIncludeExternalMSProjectCommand);
  build("include_regular_expression", cmIncludeRegularExpressionCommand);
  build("install", cmInstallCommand);
  build("install_files", cmInstallFilesCommand);
  build("install_programs", cmInstallProgramsCommand);
  build("install_targets", cmInstallTargetsCommand);
  build("link_directories", cmLinkDirectoriesCommand);
  build("link_libraries", cmLinkLibrariesCommand);
  build("list", cmListCommand);
  build("load_cache", cmLoadCacheCommand);
  build("make_directory", cmMakeDirectoryCommand);
  build("mark_as_advanced", cmMarkAsAdvancedCommand);
  build("math", cmMathCommand);
  build("message", cmMessageCommand);
  build("option", cmOptionCommand);
  build("project", cmProjectCommand);
  build("qt_wrap_cpp", cmQTWrapCPPCommand);
  build("qt_wrap_ui", cmQTWrapUICommand);
  build("remove", cmRemoveCommand);
  build("remove_definitions", cmRemoveDefinitionsCommand);
  build("separate_arguments", cmSeparateArgumentsCommand);
  build("set", cmSetCommand);
  build("set_directory_properties", cmSetDirectoryPropertiesCommand);
  build("set_property", cmSetPropertyCommand);
  build("set_source_files_properties", cmSetSourceFilesPropertiesCommand);
  build("set_target_properties",cmSetTargetPropertiesCommand);
  build("set_tests_properties", cmSetTestsPropertiesCommand);
  build("site_name", cmSiteNameCommand);
  build("source_group", cmSourceGroupCommand);
  build("string", cmStringCommand);
  build("subdirs", cmSubdirCommand);
  build("target_compile_definitions", cmTargetCompileDefinitionsCommand);
  build("target_compile_features", cmTargetCompileFeaturesCommand);
  build("target_compile_options", cmTargetCompileOptionsCommand);
  build("target_include_directories",cmTargetIncludeDirectoriesCommand);
  build("target_link_directories", cmTargetLinkDirectoriesCommand);
  build("target_link_libraries", cmTargetLinkLibrariesCommand);
  build("target_link_options", cmTargetLinkOptionsCommand);
  build("target_precompile_headers", cmTargetPrecompileHeadersCommand);
  build("target_sources", cmTargetSourcesCommand);
  build("try_compile", cmTryCompileCommand);
  build("try_run", cmTryRunCommand);
  build("unset", cmUnsetCommand);
  build("variable_watch", cmVariableWatchCommand);
  build("write_file", cmWriteFileCommand);
}
