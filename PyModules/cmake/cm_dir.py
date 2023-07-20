# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.  */

"""
Important docs...
"""
from . import utils
from . import return_param 
from . import enums 
from . import shared 
from . import cm_dir_legacy

class LibrarySet:
    def __init__(self, scope, *libraries):
        self.scope = scope
        self.libraries = libraries

    def render():
        args = [scope.name()]
        args += utils.compact_to_list(lib)
        return args

class TargetEntitySet():
    "used as a parameter to target_ commands when specifying a list of entities and their scope"
    def __init__(self, scope, entities):
        self._scope = scope
        self._entities = entities

    def add_entity(self, entity):
        self._entities.append(entity)
        
    def render(self):
        args = [self._scope.name]
        for e in self._entities:
            args.append(e)
        return args

class LinkerOptionsSet(TargetEntitySet):
    "optionally used as a parameter to the target_link_options command"
    def __init__(self, scope, options=[]):
        super(LinkerOptionsSet, self).__init__(scope, entities=list(options))

    def add_linker_option(self, option):
        "adds a linker option prefixed with LINKER: which will cmake will pass to the linker"
        self.add_entity("LINKER:" + str(option))

class FileSet():
    def __init__(self, scope, file_set, *, source_type=None, base_dirs=None, files=None):
        self._scope = scope
        self._file_set = file_set 
        self._source_type = source_type
        self._base_dirs = base_dirs
        self._files = files

    def app(self, kw, seq, args):
        if seq is None:
            return
        args.append(kw)
        for s in seq:
            args.append(s)

    def render(self):
        args = [self._scope.name]
        args.append("FILE_SET")
        args.append(self._file_set)
        if self._source_type is not None:
            args.append("TYPE")
            args.append(self._source_type.name)
        self.app("BASE_DIRS", self._base_dirs, args)
        self.app("FILES", self._files, args)
        return args

class ProjectVersion:
    def __init__(self, major, minor=None, patch=None, tweak=None):
        self.elems = [tweak, patch, minor, major]

    def __str__(self):
        elems = self.elems
        major = elems.pop()
        if major is None:
            raise ValueError("major version must not be none")
        res = str(major)
        while len(elems) > 0:
            e = elems.pop()
            if e is None:
                break
            res += "." + str(e)

        # FIXME: we'll quietly ignore 'holes' in the list here (eg if 
        # minor=None but patch is defined
        return res    

class Command():
    def __init__(self, command, *args):
        self._command = command
        self._args = args

    def render(self, with_command_kw=True, with_args_kw=True):
        res = []
        if with_command_kw:
            res.append("COMMAND")
        res.append(self._command)
        if len(self._args) > 0:
            if with_args_kw:
                res.append("ARGS")
            for a in self._args:
                res.append(a)
        return res


class CMakeDir():
    def __init__(self):
         pass

    def _inject_factory(self, factory):
        "internal method used by c++ code to inject itself"
        self._factory = factory
        self.var = self._factory.var
        self.combined_var = self._factory.combined_var
        self.raw = self._factory.raw
        self.dir_attr = self._factory.dir_attr
        self.functions = self._factory.function_launcher
        self.py_functions = self._factory.py_functions
        self.legacy = cm_dir_legacy.CMakeDirLegacy(self.raw)

    ## shorthands
    def is_set(self, varname):
        "a shorthand to check if a var is set in combined var"
        return varname in self.combined_var

    ###############################3
    ## add commands
    def add_compile_definitions(self, *defs):
        d = utils.compact_to_list(defs)
        self.raw.add_compile_definitions(*d)

    def add_custom_command_output(self, output, *outputs,
                    commands,
                    main_dependency = None,
                    byproducts = None,
                    implicit_depends = None,
                    working_directory = None,
                    comment = None,
                    depfile = None,
                    job_pool = None,
                    verbatim = False,
                    append = False,
                    uses_terminal = False,
                    command_expand_lists = False,
                    depends_expicit_only = False):
        p = utils.scan_params()
        flags = utils.build_params(p, default_key_names=True, 
                                   default_enum_names=False, default_list_names=False)
        args = ['OUTPUT', output, *outputs, *flags]
        self.raw.add_custom_command(*args)

    def add_custom_command_build_event(self, target,
                    commands,
                    build_when : enums.BuildWhen,
                    byproducts = None,
                    working_directory = None,
                    comment = None,
                    verbatim = False,
                    command_expand_lists = False):
        """
        The 'second signature' form of add_custom_command - adds a custom command 
        to a target such as a library or executable
        """
        p = utils.scan_params()
        flags = utils.build_params(p, default_key_names=True, 
                                   default_enum_names=False, default_list_names=False)
        args = [target, *flags]
        self.raw.add_custom_command(*args)

    def add_executable(self, target, *srcfiles, win32 : bool = False, 
                       macosx_bundle : bool = False, exclude_from_all : bool = False):
        p = utils.scan_params()
        flags = utils.build_params(p)
        s = utils.compact_to_list(srcfiles)
        args = [target, *flags, *s]
        
        self.raw.add_executable(*args)

    def add_executable_imported(self, name, _global : bool = False):
        args = [name, "IMPORTED"]
        if _global:
            args.append("GLOBAL")

        self.raw.add_executable(*args)

    def add_executable_alias(self, name, target):
        args = [name, "ALIAS", target]
        self.raw.add_executable(*args)

    def add_subdirectory(self, path,bin_dir = None, 
                         exclude_from_all : bool = False, system : bool = False):
        p = utils.scan_params()
        flags = utils.build_params(p, default_key_names=False)
        args = [path, *flags]
        self.raw.add_subdirectory(*args)

    #add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL] [<source>...])
    def add_library(self, name, *sources, 
                    library_type : enums.LibraryType = None, 
                    exclude_from_all = False):
        p = utils.scan_params()
        flags = utils.build_params(p, default_enum_names=False, default_key_names=False)
        src = utils.compact_to_list(sources)
        self.raw.add_library(name, *flags, *src)

    #add_library(<name> OBJECT [<source>...])
    def add_library_object(self, name, *sources):
        src = utils.compact_to_list(sources)
        self.raw.add_library(name, "OBJECT", *src)

    #add_library(<name> <type> IMPORTED [GLOBAL])
    def add_library_imported(self, name, 
                             library_type : enums.ImportedLibraryType, 
                             _global : bool = False):
        global_ = ["GLOBAL"] if _global else [] 
        self.raw.add_library(name, library_type.name, "IMPORTED", *global_)

    #add_library(<name> ALIAS <target>)
    def add_library_alias(self, name, target):
        self.raw.add_library(name, "ALIAS", target)
    
    def add_compile_options(self, option, *options):
        args = [option, *options]
        self.raw.add_compile_options(*args)

    def add_link_options(self, option, *options):
        args = [option, *options]
        self.raw.add_link_options(*args)

    def add_dependencies(self, target, dependency, *dependencies):
        args = [target, dependency, *dependencies]
        self.raw.add_dependencies(*args)

#add_custom_target(Name [ALL] [command1 [args1...]]
#                  [COMMAND command2 [args2...] ...]
#                  [DEPENDS depend depend depend ... ]
#                  [BYPRODUCTS [files...]]
#                  [WORKING_DIRECTORY dir]
#                  [COMMENT comment]
#                  [JOB_POOL job_pool]
#                  [VERBATIM] [USES_TERMINAL]
#                  [COMMAND_EXPAND_LISTS]
#                  [SOURCES src1 [src2...]])
    def add_custom_target(self, name,
                    _all = False,
                    commands = None,
                    depends = None,
                    byproducts = None,
                    working_directory = None,
                    comment = None,
                    job_pool = None,
                    verbatim = False,
                    uses_terminal = False,
                    command_expand_lists = False,
                    sources = None):
        p = utils.scan_params()

        # sigh.. this statement has inconsistent command layout, so have
        # to special-case it..
        del p['commands']
        del p['_all']
        flags = utils.build_params(p, default_list_names = True)

        args = [name]
        if _all:
            args.append('ALL')
        if commands is not None:
            command_kw = False
            for c in commands:
                cmd = c.render(with_command_kw=command_kw, with_args_kw=False)
                args += cmd
                command_kw = True

        args += flags
        self.raw.add_custom_target(*args)
#add_test(NAME <name> COMMAND <command> [<arg>...]
#         [CONFIGURATIONS <config>...]
#         [WORKING_DIRECTORY <dir>]
#         [COMMAND_EXPAND_LISTS])
    def add_test(self, name,
                    command : Command, 
                    configurations = None,
                    working_directory = None,
                    command_expand_lists = False):
        p = utils.scan_params()

        del p['command']
        flags = utils.build_params(p, default_list_names = True)

        args = ['NAME', name]
        cmd = command.render(with_command_kw=True, with_args_kw=False)
        args += cmd

        args += flags
        self.raw.add_test(*args)

    ###############################3
    ## cmake_policy commands
    def cmake_policy_version(self, version, policy_max = None):
        "set policies by cmake version"
        version_str = shared.build_version_str(version, policy_max)
        args = ["VERSION", version_str]
        self.raw.cmake_policy(*args)

    def cmake_policy_set(self, policy, mode: enums.PolicyMode):
        "set individual policies explicitly"
        p = utils.scan_params()
        flags = utils.build_params(p, default_enum_names=False)
        if type(policy) == int:
            pol = "CMP" + f'{policy:04}'
        else:
            pol = policy
        args = ("SET", pol, *flags)
        self.raw.cmake_policy(*args)

    ###############################3
    ## find commands
    def find_package(self, package_name,
                    version=None, 
                    exact=False, 
                    quiet=False, 
                    required=False, 
                    components=None, 
                    optional_components=None, 
                    mode : enums.FindPackageMode = None, 
                    _global=False, 
                    no_policy_scope=False,
                    bypass_provider=False, 
                    names=None, 
                    configs=None,
                    hints=None, 
                    paths=None, 
                    registry_view : enums.RegistryView = None,
                    path_suffixes=None, 
                    no_default_path=False,
                    no_package_root_path=False,
                    no_cmake_path=False,
                    no_cmake_environment_path=False, 
                    no_system_environment_path=False,
                    no_cmake_package_registry=False,
                    no_cmake_system_path=False,
                    no_cmake_install_prefix=False, 
                    no_cmake_system_package_registry=False,
                    find_root_path_mode : enums.FindRootPathMode = None):
        p = utils.scan_params()
        flags = utils.build_params(p, 
                        default_enum_names=False, enum_name_exclusions={"registry_view"},
                        default_key_names=True, key_name_exclusions={'version'})
        args = [package_name, *flags]

        #print("ARGS", args)
        return utils.exec_and_return_entity_diff(self, self.raw.find_package, args)

#  find_file
#  find_library
#  find_path
#  find_program

    ###############################3
    ## Target commands
    def target_compile_definitions(self, target, *defs):
        args = [target]
        for d in defs:
            args += d.render()
        self.raw.target_compile_definitions(*args)

    def target_compile_features(self, target, *features):
        args = [target]
        for f in features:
            args += f.render()
        self.raw.target_compile_features(*args)

    def target_compile_options(self, target, *options, end : enums.End = None):
        args = [target]
        # special case handling for end - there's no "after" parameter
        if(end == enums.End.BEFORE):
            args += ["BEFORE"]
        for o in options:
            args += o.render()
        self.raw.target_compile_options(*args)

    def target_link_libraries(self, target, *libraries):
        args = [target]
        for lib in libraries:
            if type(lib) is TargetEntitySet:
                args += lib.render()
            else:
                args += utils.compact_to_list([lib])
        self.raw.target_link_libraries(*args)

    def target_link_options(self, target, *options):
        args = [target]
        for o in options:
            args += o.render()
        self.raw.target_link_options(*args)
    
    def target_precompile_headers(self, target, *headers):
        args = [target]
        for h in headers:
            args += h.render()
        self.raw.target_precompile_headers(*args)

    def target_sources(self, target, *sets):
        args = [target]
        for s in sets:
            if type(s) not in (FileSet, TargetEntitySet):
                raise TypeError("unexpected type " + str(type(s)) +' given to target_sources')
            for a in s.render():
                args.append(a)
        
        self.raw.target_sources(*args)

#target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
#  <INTERFACE|PUBLIC|PRIVATE> [items1...]
#  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])

    def target_include_directories(self, target,
                                   items,
                                   system = False, 
                                   end : enums.End = None):
        p = utils.scan_params()
        flags = utils.build_params(p, default_enum_names = False, 
                                   default_list_names = False)
        args = [target, *flags]
        self.raw.target_include_directories(*args)

#target_link_directories(<target> [BEFORE]
#  <INTERFACE|PUBLIC|PRIVATE> [items1...]
#  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
    def target_link_directories(self, target, items
                                end : enums.End = None):
        p = utils.scan_params()
        # special case handling for end - there's no "after" parameter
        del p['end']
        flags = utils.build_params(p, default_enum_names = False, 
                                   default_list_names = False)
        args = [target]
        if(end == enums.End.BEFORE):
            args.append("BEFORE")
        args += flags
        self.raw.target_link_directories(*args)

    def include(self, f, optional=False, no_policy_scope=False):
        p = utils.scan_params()
        flags = utils.build_params(p)
        # capture the result var (ie location of the file found).
        rv = return_param.ReturnParam()
        args = [f, *flags, "RESULT_VARIABLE", rv]
        res = self.raw.include(*args)
        return res

    def project(self, project_name, version=None, description=None, 
                homepage_url=None, languages=None):
        args = [project_name]
        # FIXME: this could probably use utils.build_params()
        if version is not None:
            args += ["VERSION", str(version)]
        if description is not None:
            args += ["DESCRIPTION", str(description)]
        if homepage_url is not None:
            args += ["HOMEPAGE_URL", str(homepage_url)]
        if languages is not None:
            args += ["LANGUAGES"]
            args += utils.compact_enums_to_list(languages)
        self.raw.project(*args)
    
#configure_file(<input> <output>
#               [NO_SOURCE_PERMISSIONS | USE_SOURCE_PERMISSIONS |
#                FILE_PERMISSIONS <permissions>...]
#               [COPYONLY] [ESCAPE_QUOTES] [@ONLY]
#               [NEWLINE_STYLE [UNIX|DOS|WIN32|LF|CRLF] ])

#FIXME: implement permissions stuff
    def configure_file(self, 
                       input_path, output_path,
                       copy_only = False,
                       escape_quotes = False,
                       at_only = False,
                       newline_style : enums.NewlineStyle = None):
        p = utils.scan_params()
        flags = utils.build_params(p, default_key_names=True)
        args = [input_path, output_path, *flags]
        self.raw.configure_file(*args)

#source_group(<name> [FILES <src>...] [REGULAR_EXPRESSION <regex>])
#source_group(TREE <root> [PREFIX <prefix>] [FILES <src>...])
#site_name(variable)

#.export(TARGETS <target>... [...])
#export(EXPORT <export-name> [...])
#export(PACKAGE <PackageName>)
