#!/usr/bin/env python3

import cmake
import unittest

from . import mock

class TestCommandAddExecutable(mock.CommonDir):
    def test_simple(self):
        self.d.add_executable("foo", "src1", "src2")
        self.d.raw.add_executable.assert_called_once_with("foo", "src1", "src2")

    def test_simple_splat(self):
        # check a list gets splatted correctly
        self.d.add_executable("foo", ["src1", "src2", "src3.cpp", "src4.h"])
        self.d.raw.add_executable.assert_called_once_with(
                "foo", "src1", "src2", "src3.cpp", "src4.h")

    def test_simple_params_and_list(self):
        # mix of loose params and a list
        self.d.add_executable("foo", "src1", "src2", ["src3.cpp", "src4.h"])
        self.d.raw.add_executable.assert_called_once_with(
                "foo", "src1", "src2", "src3.cpp", "src4.h")

    def test_simple_params_tuple(self):
        #... tuple
        self.d.add_executable("foo", "src1", ("src2", "src3.cpp", "src4.h"))
        self.d.raw.add_executable.assert_called_once_with(
                "foo", "src1", "src2", "src3.cpp", "src4.h")

    def test_all_params(self):
        self.d.add_executable("foo", "src1", "src2", win32 = True, macosx_bundle = True, 
                       exclude_from_all = True)

        self.d.raw.add_executable.assert_called_once_with("foo", "WIN32", "MACOSX_BUNDLE", 
                                                          "EXCLUDE_FROM_ALL", "src1", "src2") 

    def test_all_params_win32(self):
        self.d.add_executable("foo", "foo.cpp", win32=True)
        self.d.raw.add_executable.assert_called_once_with("foo", "WIN32", "foo.cpp") 

    def test_all_params_macos_bindle(self):
        self.d.add_executable("foo2", "foo2.cpp", macosx_bundle=True)
        self.d.raw.add_executable.assert_called_once_with("foo2", "MACOSX_BUNDLE", "foo2.cpp") 

    def test_imported(self):
        self.d.add_executable_imported("target3")
        self.d.raw.add_executable.assert_called_once_with("target3", "IMPORTED") 

    def test_imported_global(self):
        self.d.add_executable_imported("target4", _global=True)
        self.d.raw.add_executable.assert_called_once_with("target4", "IMPORTED", "GLOBAL") 

    def test_alias(self):
        self.d.add_executable_alias("target5", "other_target")
        self.d.raw.add_executable.assert_called_once_with("target5", "ALIAS", "other_target") 

class TestCommandAddCustomTarget(mock.CommonDir):
    def test_simple(self):
        self.d.add_custom_target("t1")
        self.d.raw.add_custom_target.assert_called_once_with("t1")

    def test_all(self):
        self.d.add_custom_target("t1",
            commands = [cmake.Command("ext_command", "arg1"),
                        cmake.Command("ext_command2", "arg2", "arg3")],
            _all = True,
            depends=["t10", "t11", "t12", "t13"],
            byproducts=["f1", "f2", "f3"],
            working_directory="/some/dir/somewhere",
            comment="important job",
            job_pool="jp1",
            verbatim=True,
            uses_terminal=True,
            command_expand_lists=True,
            sources=['tu1.cpp', 'tu2.cpp'])

        self.d.raw.add_custom_target.assert_called_once_with(
            't1', 
            'ALL', 
            'ext_command', 'arg1', 'COMMAND', 'ext_command2', 'arg2', 'arg3',
            'DEPENDS', 't10', 't11', 't12', 't13', 
            'BYPRODUCTS', 'f1', 'f2', 'f3', 
            'WORKING_DIRECTORY', '/some/dir/somewhere', 
            'COMMENT', 'important job', 
            'JOB_POOL', 'jp1', 
            'VERBATIM', 
            'USES_TERMINAL', 
            'COMMAND_EXPAND_LISTS', 
            'SOURCES', 'tu1.cpp', 'tu2.cpp')

    def test_empty_target(self):
        self.d.add_custom_target("t3")
        self.d.raw.add_custom_target.assert_called_once_with("t3")

class TestCommandAddTest(mock.CommonDir):
    def test_simple(self):
        self.d.add_test("test1",
            command = cmake.Command("test_command", "arg1", "arg2"))

        self.d.raw.add_test.assert_called_once_with(
                'NAME', 'test1', 'COMMAND', 'test_command', 'arg1', 'arg2')

    def test_all(self):
        self.d.add_test("test2",
            command = cmake.Command("test_command2", "arg1", "arg2"),
            configurations = ['conf1', 'conf2', 'conf3'],
            working_directory = '/somewhere/',
            command_expand_lists = True)

        self.d.raw.add_test.assert_called_once_with(
            'NAME', 'test2', 
            'COMMAND', 'test_command2', 'arg1', 'arg2', 
            'CONFIGURATIONS', 'conf1', 'conf2', 'conf3', 
            'WORKING_DIRECTORY', '/somewhere/', 'COMMAND_EXPAND_LISTS')

class TestCommandAddSubDirectory(mock.CommonDir):
    def test_add_subdirectory(self):
        self.d.add_subdirectory("dir1")
        self.d.raw.add_subdirectory.assert_called_once_with("dir1")

    def test_add_subdirectory_bindir(self):
        self.d.add_subdirectory("dir1", bin_dir="bindir")
        self.d.raw.add_subdirectory.assert_called_once_with(
                "dir1", "bindir")

    def test_add_subdirectory_exclude_from_all(self):
        self.d.add_subdirectory("dir1", bin_dir="bindir", exclude_from_all=True)
        self.d.raw.add_subdirectory.assert_called_once_with(
                "dir1", "bindir", "EXCLUDE_FROM_ALL")

    def test_add_subdirectory_exclude_from_all_system(self):
        self.d.add_subdirectory("dir1", bin_dir="bin_dir", exclude_from_all=True, system=True)
        self.d.raw.add_subdirectory.assert_called_once_with(
                "dir1", "bin_dir", "EXCLUDE_FROM_ALL", "SYSTEM")

class TestCommandAddCustomCommandOutput(mock.CommonDir):
    def test_simple(self):
        self.d.add_custom_command_output(
                "output1",
                commands = cmake.Command("ext_command1"))

        self.d.raw.add_custom_command.assert_called_once_with(
            'OUTPUT', 'output1', 'COMMAND', 'ext_command1')

    def test_multi_output(self):
        self.d.add_custom_command_output(
                "output1", "output2", 
                commands = [cmake.Command("ext_command", "arg1")])

        self.d.raw.add_custom_command.assert_called_once_with(
            'OUTPUT', 'output1', 'output2', 'COMMAND', 'ext_command', 'ARGS', 'arg1')


class TestCommandAddCustomCommandBuildEvent(mock.CommonDir):
    def test_simple(self):
        self.d.add_custom_command_build_event(
                "t50", 
                build_when=cmake.BuildWhen.PRE_BUILD,
                commands = [cmake.Command("ext_command")]
                )

        self.d.raw.add_custom_command.assert_called_once_with(
            't50', 'PRE_BUILD', 'COMMAND', 'ext_command')

    def test_extra_params(self):
        self.d.add_custom_command_build_event(
                "t50", 
                build_when = cmake.BuildWhen.PRE_BUILD,
                commands = [cmake.Command("ext_command")],
                working_directory="some_dir")

        self.d.raw.add_custom_command.assert_called_once_with(
            't50', 'PRE_BUILD', 'COMMAND', 'ext_command', 'WORKING_DIRECTORY', 'some_dir')

    def test_multiple_commands(self):
        self.d.add_custom_command_build_event(
                "t51", build_when=cmake.BuildWhen.POST_BUILD,
                commands = [
                    cmake.Command("ext_command", 'arg1'),
                    cmake.Command("ext_command2", 'arg2', 'arg3')], 
                working_directory="some_dir")

        self.d.raw.add_custom_command.assert_called_once_with(
                't51', 'POST_BUILD', 
                    'COMMAND', 'ext_command', 'ARGS', 'arg1', 
                    'COMMAND', 'ext_command2', 'ARGS', 'arg2', 'arg3', 
                    'WORKING_DIRECTORY', 'some_dir')

class TestCommandAddLibrary(mock.CommonDir):
    def test_add_library(self):
        self.d.add_library("lib1", "src1", "src2")
        self.d.raw.add_library.assert_called_once_with("lib1", "src1", "src2") 

    def test_add_library_list(self):
        self.d.add_library("lib1", ["src1", "src2"])
        self.d.raw.add_library.assert_called_once_with("lib1", "src1", "src2") 

    def test_add_library_static(self):
        self.d.add_library("lib1", "src1", "src2", library_type=cmake.LibraryType.STATIC)
        self.d.raw.add_library.assert_called_once_with("lib1", "STATIC", "src1", "src2") 

    def test_add_library_exclude_from_all(self):
        self.d.add_library("lib1", "src1", "src2", exclude_from_all=True)
        self.d.raw.add_library.assert_called_once_with("lib1", "EXCLUDE_FROM_ALL", "src1", "src2") 

    def test_add_library_exclude_from_all_module(self):
        self.d.add_library("lib1", "src1", "src2", library_type=cmake.LibraryType.MODULE, exclude_from_all=True)
        self.d.raw.add_library.assert_called_once_with("lib1", "MODULE", "EXCLUDE_FROM_ALL", "src1", "src2") 

    def test_add_library_object(self):
        self.d.add_library_object("lib1", "src1", "src4")
        self.d.raw.add_library.assert_called_once_with("lib1", "OBJECT", "src1", "src4") 

    def test_add_library_objecti_list(self):
        self.d.add_library_object("lib1", ["src1", "src4"])
        self.d.raw.add_library.assert_called_once_with("lib1", "OBJECT", "src1", "src4") 

    def test_add_library_imported(self):
        self.d.add_library_imported("lib1", cmake.ImportedLibraryType.UNKNOWN)
        self.d.raw.add_library.assert_called_once_with("lib1", "UNKNOWN", "IMPORTED") 

    def test_add_library_imported_global(self):
        self.d.add_library_imported("lib2", cmake.ImportedLibraryType.MODULE, _global=True)
        self.d.raw.add_library.assert_called_once_with("lib2", "MODULE", "IMPORTED", "GLOBAL") 

    def test_add_library_alias(self):
        self.d.add_library_alias("lib4", "target7")
        self.d.raw.add_library.assert_called_once_with("lib4", "ALIAS", "target7") 

class TestCommandAddCompileDefinitions(mock.CommonDir):
    def test_add_compile_definitions(self):
        self.d.add_compile_definitions("-DSTUFF")
        self.d.raw.add_compile_definitions.assert_called_once_with("-DSTUFF")

    def test_add_compile_definitions_multi(self):
        self.d.add_compile_definitions("-DTHINGS", "-DOTHER_THINGS")
        self.d.raw.add_compile_definitions.assert_called_once_with("-DTHINGS", "-DOTHER_THINGS")

class TestCommandTargetSources(mock.CommonDir):
    def test_source_set(self):
        self.d.target_sources("t1", 
                cmake.TargetEntitySet(cmake.Scope.PUBLIC, 
                                ("foo.cpp", "bar.cpp", "baz.cpp")))

        self.d.raw.target_sources.assert_called_once_with(
                "t1", "PUBLIC", "foo.cpp", "bar.cpp", "baz.cpp") 

    def test_file_set(self):
        self.d.target_sources("t2", 
            cmake.FileSet(cmake.Scope.PUBLIC, 
                           "new_files", 
                           source_type=cmake.SourceType.CXX_MODULES, 
                           files=("foo.cpp", "bar.cpp", "baz.cpp")))

        self.d.raw.target_sources.assert_called_once_with(
            't2', 'PUBLIC', 
            'FILE_SET', 'new_files', 
            'TYPE', 'CXX_MODULES', 
            'FILES', 'foo.cpp', 'bar.cpp', 'baz.cpp')

    def test_file_both(self):
        es1 = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("foo.cpp", "bar.cpp", "baz.cpp"))

        es2 = cmake.TargetEntitySet(cmake.Scope.PRIVATE, ("other1.cpp", "other2.cpp", "other3.cpp"))

        fs1 = cmake.FileSet(cmake.Scope.PUBLIC, 
                           "new_files", 
                           source_type=cmake.SourceType.CXX_MODULES, 
                           files=("m1.cpp", "m2.cpp", "m3.cpp"))

        fs2 = cmake.FileSet(cmake.Scope.INTERFACE, 
                           "new_files2", 
                           source_type=cmake.SourceType.HEADERS, 
                           files=("m1.hpp", "m2.hpp", "m3.hpp"),
                           base_dirs=("d1", "d2"))

        self.d.target_sources("t3", es1, fs1, es2, fs2)

        self.d.raw.target_sources.assert_called_once_with(
                't3', 
                'PUBLIC', 'foo.cpp', 'bar.cpp', 'baz.cpp', 
                'PUBLIC', 'FILE_SET', 'new_files', 'TYPE', 'CXX_MODULES', 
                    'FILES', 'm1.cpp', 'm2.cpp', 'm3.cpp', 
                'PRIVATE', 'other1.cpp', 'other2.cpp', 'other3.cpp', 
                'INTERFACE', 'FILE_SET', 'new_files2', 'TYPE', 'HEADERS', 
                    'BASE_DIRS', 'd1', 'd2', 
                    'FILES', 'm1.hpp', 'm2.hpp', 'm3.hpp')

class TestCommandTargetCompileDefinitions(mock.CommonDir):
    def test_compile_definitions(self):
        self.d.target_compile_definitions("t10", 
                cmake.TargetEntitySet(cmake.Scope.PUBLIC, 
                                ("-DAWESOME_MODE", "-DBAD_MODE")))

        self.d.raw.target_compile_definitions.assert_called_once_with(
                "t10", "PUBLIC", "-DAWESOME_MODE", "-DBAD_MODE") 

    def test_compile_definitions_multiple(self):
        s1 = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("-DCOOL_MODE", "-DUNCOOL_MODE"))
        s2 = cmake.TargetEntitySet(cmake.Scope.INTERFACE, ("-DYEP", "-DNOPE"))

        self.d.target_compile_definitions("t12", s1, s2) 

        self.d.raw.target_compile_definitions.assert_called_once_with(
                't12', 
                'PUBLIC', '-DCOOL_MODE', '-DUNCOOL_MODE', 
                'INTERFACE', '-DYEP', '-DNOPE')

class TestCommandTargetCompileFeatures(mock.CommonDir):
    def test_target_compile_features(self):
        s1 = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("cxx_delegating_constructors", 
                                   "cxx_deleted_functions", "cxx_enum_forward_declarations"))

        self.d.target_compile_features("t20", s1)

        self.d.raw.target_compile_features.assert_called_once_with(
                't20', 'PUBLIC', 
                'cxx_delegating_constructors', 'cxx_deleted_functions', 'cxx_enum_forward_declarations')

class TestCommandTargetCompileOptions(mock.CommonDir):
    def test_target_compile_options(self):
        s1 = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("-m", "-n", "-zzzz")) 

        self.d.target_compile_options("t21", s1)

        self.d.raw.target_compile_options.assert_called_once_with(
                't21', 'PUBLIC', '-m', '-n', '-zzzz')

    def test_target_compile_options_before(self):
        s1 = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("-m", "-n", "-zzzz")) 

        self.d.target_compile_options("t22", s1, end=cmake.End.BEFORE)

        self.d.raw.target_compile_options.assert_called_once_with(
                't22', 'BEFORE', 'PUBLIC', '-m', '-n', '-zzzz')

class TestCommandTargetLinkLibraries(mock.CommonDir):
    def test_target_link_libraries_simple(self):
        self.d.target_link_libraries("t21", "myLib")

        self.d.raw.target_link_libraries.assert_called_once_with(
                't21', 'myLib')

    def test_target_link_libraries_multiple(self):
        self.d.target_link_libraries("t21", "myLib", "myOtherLib")

        self.d.raw.target_link_libraries.assert_called_once_with(
                't21', 'myLib', 'myOtherLib')

    def test_target_link_libraries_list(self):
        self.d.target_link_libraries("t21", "myLib", "myOtherLib")

        self.d.raw.target_link_libraries.assert_called_once_with(
                't21', 'myLib', 'myOtherLib')

    def test_target_link_libraries_entity(self):
        e = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("c", "parser", "eater")) 

        self.d.target_link_libraries("t21", e)

        self.d.raw.target_link_libraries.assert_called_once_with(
                't21', 'PUBLIC', 'c', 'parser', 'eater')

class TestCommandTargetLinkOptions(mock.CommonDir):
    def test_target_link_options_entity(self):
        e = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("-big-thing", "--other-thing")) 

        self.d.target_link_options("t32", e)

        self.d.raw.target_link_options.assert_called_once_with(
            't32', 'PUBLIC', '-big-thing', '--other-thing')

    def test_target_link_options_linker_options_set(self):
        e = cmake.LinkerOptionsSet(cmake.Scope.PUBLIC, ("-big-thing", "--other-thing")) 
        e.add_linker_option("good_mode")
        e.add_linker_option("better_mode")

        self.d.target_link_options("t33", e)

        self.d.raw.target_link_options.assert_called_once_with(
            't33', 'PUBLIC', '-big-thing', '--other-thing', 'LINKER:good_mode', 'LINKER:better_mode')

class TestCommandTargetPrecompileHeaders(mock.CommonDir):
    def test_target_precompile_headers_entity(self):
        e = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("[[foo.h]]", "<unordered_map>")) 

        self.d.target_precompile_headers("t31", e)

        self.d.raw.target_precompile_headers.assert_called_once_with(
                't31', 'PUBLIC', '[[foo.h]]', '<unordered_map>')

class TestCommandTargetIncludeDirectories(mock.CommonDir):
    def test_simple(self):
        #e = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("[[foo.h]]", "<unordered_map>")) 

        self.d.target_include_directories("t31",
            items = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("foo.h", "bar.h"))) 

        self.d.raw.target_include_directories.assert_called_once_with(
                't31', 'PUBLIC', 'foo.h', 'bar.h')

    def test_multi(self):
        e1 = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("foo.hpp", "bar.hpp")) 
        e2 = cmake.TargetEntitySet(cmake.Scope.PRIVATE, ("h1.hpp", "h2.hpp")) 

        self.d.target_include_directories("t32", system=True, end=cmake.End.BEFORE,
            items = (e1, e2))

        self.d.raw.target_include_directories.assert_called_once_with(
            't32', 'SYSTEM', 'BEFORE', 
                'PUBLIC', 'foo.hpp', 'bar.hpp', 'PRIVATE', 'h1.hpp', 'h2.hpp')

class TestCommandTargetLinkDirectories(mock.CommonDir):
    def test_simple(self):
        self.d.target_link_directories("t31",
            items = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("dir1", "dir2")))

        self.d.raw.target_link_directories.assert_called_once_with(
                't31', 'PUBLIC', 'dir1', 'dir2')

    def test_simple(self):
        self.d.target_link_directories("t31",
            items = [cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("dir1", "dir2")),
                     cmake.TargetEntitySet(cmake.Scope.PRIVATE, ("dir3", "dir4"))])

        self.d.raw.target_link_directories.assert_called_once_with(
                't31', 'PUBLIC', 'dir1', 'dir2', 'PRIVATE', 'dir3', 'dir4')

    def test_before(self):
        self.d.target_link_directories("t31", end=cmake.End.BEFORE,
            items = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("dir1", "dir2")))

        self.d.raw.target_link_directories.assert_called_once_with(
                't31', 'BEFORE', 'PUBLIC', 'dir1', 'dir2')

    def test_after(self):
        self.d.target_link_directories("t31", end=cmake.End.AFTER,
            items = cmake.TargetEntitySet(cmake.Scope.PUBLIC, ("dir1", "dir2")))

        self.d.raw.target_link_directories.assert_called_once_with(
                't31', 'PUBLIC', 'dir1', 'dir2')

class TestCommandProject(mock.CommonDir):
    def test_simple(self):
        self.d.project("sample_project")
        self.d.raw.project.assert_called_once_with("sample_project") 

    def test_combined(self):
        self.d.project("targetTest", version="1.2.3", homepage_url="https://myprojectr.com", 
                       languages="CXX", description="important project")

        self.d.raw.project.assert_called_once_with(
                "targetTest", "VERSION", "1.2.3", "DESCRIPTION", "important project",
                "HOMEPAGE_URL", "https://myprojectr.com", "LANGUAGES", "CXX")

    def test_versions1(self):
        self.d.project("sample_project2", version="4.6.8")
        self.d.raw.project.assert_called_once_with("sample_project2", "VERSION", "4.6.8") 

    def test_versions2(self):
        self.d.project("sample_project3", version=8)
        self.d.raw.project.assert_called_once_with("sample_project3", "VERSION", "8") 

    def test_versions3(self):
        self.d.project("sample_project4", version=cmake.ProjectVersion(9))
        self.d.raw.project.assert_called_once_with("sample_project4", "VERSION", "9") 

    def test_versions4(self):
        self.d.project("sample_project5", version=cmake.ProjectVersion(9,1))
        self.d.raw.project.assert_called_once_with("sample_project5", "VERSION", "9.1") 

    def test_versions5(self):
        self.d.project("sample_project6", version=cmake.ProjectVersion(9,1,2))
        self.d.raw.project.assert_called_once_with("sample_project6", "VERSION", "9.1.2") 

    def test_versions6(self):
        self.d.project("sample_project7", version=cmake.ProjectVersion(9,1,2,3))
        self.d.raw.project.assert_called_once_with("sample_project7", "VERSION", "9.1.2.3") 

    def test_versions7(self):
        self.d.project("sample_project8", version=cmake.ProjectVersion(
            major=4, minor=8, patch=7, tweak=6))
        self.d.raw.project.assert_called_once_with("sample_project8", "VERSION", "4.8.7.6") 

    def test_langauages_CXX(self):
        self.d.project("sample_project", languages=[cmake.Languages.CXX])
        self.d.raw.project.assert_called_once_with("sample_project", "LANGUAGES", "CXX")

    def test_langauages_CXX_C(self):
        self.d.project("sample_project", languages=[cmake.Languages.CXX, cmake.Languages.C])
        self.d.raw.project.assert_called_once_with("sample_project", "LANGUAGES", "CXX", "C")

    def test_langauages_CXX_flat(self):
        self.d.project("sample_project", languages=cmake.Languages.CXX)
        self.d.raw.project.assert_called_once_with("sample_project", "LANGUAGES", "CXX")

    def test_langauages_CXX_str(self):
        self.d.project("sample_project", languages="CXX")
        self.d.raw.project.assert_called_once_with("sample_project", "LANGUAGES", "CXX")

class TestCommandFindModule(mock.CommonDir):
    def test_find_module(self):
        self.d.find_package("some_package")
        self.d.raw.find_package.assert_called_once_with("some_package")

    def test_find_module_full_test_all_the_things(self):
        self.d.find_package("some_package2", 
                    version='1.2.3', 
                    exact=True, 
                    quiet=True, 
                    required=True,
                    components=["partA", "partB"], 
                    optional_components=["PartC", "PartD"], 
                    mode=cmake.FindPackageMode.CONFIG, 
                    _global=True, 
                    no_policy_scope=True,
                    bypass_provider=True, 
                    names=['Name1', 'Name2'], 
                    configs=['Config1', 'Config2'],
                    hints=['hint1', 'hint2', 'hint3'], 
                    paths=None, 
                    registry_view=cmake.RegistryView._64_32,
                    path_suffixes=['suffix1', 'suffix2', 'suffix3'], 
                    no_default_path=True,
                    no_package_root_path=True, 
                    no_cmake_path=True,
                    no_cmake_environment_path=True,
                    no_system_environment_path=True,
                    no_cmake_package_registry=True, 
                    no_cmake_system_path=True,
                    no_cmake_install_prefix=True,
                    no_cmake_system_package_registry=True,
                    find_root_path_mode=cmake.FindRootPathMode.ONLY_CMAKE_FIND_ROOT_PATH)

        self.d.raw.find_package.assert_called_once_with(
                'some_package2', '1.2.3', 'EXACT', 'QUIET', 'REQUIRED', 
                'COMPONENTS', 'partA', 'partB', 'OPTIONAL_COMPONENTS', 'PartC', 
                'PartD', 'CONFIG', 'GLOBAL', 'NO_POLICY_SCOPE', 'BYPASS_PROVIDER', 
                'NAMES', 'Name1', 'Name2', 'CONFIGS', 'Config1', 'Config2', 'HINTS', 
                'hint1', 'hint2', 'hint3', 'REGISTRY_VIEW', '64_32', 'PATH_SUFFIXES', 
                'suffix1', 'suffix2', 'suffix3', 'NO_DEFAULT_PATH', 'NO_PACKAGE_ROOT_PATH', 
                'NO_CMAKE_PATH', 'NO_CMAKE_ENVIRONMENT_PATH', 'NO_SYSTEM_ENVIRONMENT_PATH', 
                'NO_CMAKE_PACKAGE_REGISTRY', 'NO_CMAKE_SYSTEM_PATH', 'NO_CMAKE_INSTALL_PREFIX', 
                'NO_CMAKE_SYSTEM_PACKAGE_REGISTRY', 
                'ONLY_CMAKE_FIND_ROOT_PATH')

class TestCommandInclude(mock.CommonDir):
    def test_include(self):
        self.d.include("some_file")
        self.d.raw.include.assert_called_once_with(
                "some_file", "RESULT_VARIABLE", mock.MockReturnParam())

    def test_include_all(self):
        self.d.include("some_file2", optional=True, no_policy_scope=True)
        self.d.raw.include.assert_called_once_with(
                "some_file2", "OPTIONAL", "NO_POLICY_SCOPE", "RESULT_VARIABLE", 
                mock.MockReturnParam())

class TestCommandConfigureFile(mock.CommonDir):
    def test_simple(self):
        self.d.configure_file("input_file", "output_file")
        self.d.raw.configure_file.assert_called_once_with("input_file", "output_file")

    def test_all_vars(self):
        self.d.configure_file("input_file", "output_file", 
                copy_only=True, escape_quotes=True, at_only=True, 
                newline_style = cmake.NewlineStyle.WIN32)

        self.d.raw.configure_file.assert_called_once_with(
                'input_file', 'output_file', 
                'COPY_ONLY', 'ESCAPE_QUOTES', 'AT_ONLY', 'NEWLINE_STYLE', 'WIN32')

class TestCommandCMakePolicy(mock.CommonDir):
    def test_set(self):
        self.d.cmake_policy_set("CMP0123", mode=cmake.PolicyMode.NEW)
        self.d.raw.cmake_policy.assert_called_once_with('SET', 'CMP0123', 'NEW')

    def test_set(self):
        self.d.cmake_policy_set(451, mode=cmake.PolicyMode.OLD)
        self.d.raw.cmake_policy.assert_called_once_with('SET', 'CMP0451', 'OLD')

    def test_version(self):
        self.d.cmake_policy_version(cmake.CMakeVersion(5, 30, 5))
        self.d.raw.cmake_policy.assert_called_once_with("VERSION", "5.30.5")

    def test_version_policy_max(self):
        vers_min = cmake.CMakeVersion(3, 1, 1)
        vers_max = cmake.CMakeVersion(4, 5, 2)

        self.d.cmake_policy_version(vers_min, vers_max)

        self.d.raw.cmake_policy.assert_called_once_with("VERSION", "3.1.1...4.5.2")

if __name__ == "__main__":
    unittest.main()

