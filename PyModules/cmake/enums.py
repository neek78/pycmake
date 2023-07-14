
import enum

Scope = enum.Enum('Scope', ["INTERFACE", "PUBLIC", "PRIVATE"])

# target_sources types 

SourceType = enum.Enum('SourceType', ["HEADERS", "CXX_MODULES"])

# add_library types  
LibraryType = enum.Enum('LibraryType', ['STATIC', 'SHARED', 'MODULE', 'INTERFACE'])

# project types
ImportedLibraryType = enum.Enum('ImportedLibraryType', 
                                ['STATIC', 'SHARED', 'MODULE', 'UNKNOWN'])

# project enums
Languages = enum.Enum("Languages", ['C', 'CXX', 'CSharp', 'CUDA', 'OBJC', 
    'OBJCXX', 'Fortran', 'HIP', 'ISPC', 'Swift', 'ASM', 'ASM_NASM', 
    'ASM_MARMASM', 'ASM_MASM', 'ASM-ATT'])

# find_package enums
RegistryView = enum.Enum('RegistryView', 
                         ["_64", "_32", "_64_32", "_32_64", "HOST", "TARGET", "BOTH"])

FindPackageMode = enum.Enum("FindPackageMode", ['CONFIG', 'MODULE'])

FindRootPathMode = enum.Enum('FindRootPathMode ', 
                    ["CMAKE_FIND_ROOT_PATH_BOTH", "ONLY_CMAKE_FIND_ROOT_PATH", 
                     "NO_CMAKE_FIND_ROOT_PATH"])

# configure_file types
NewlineStyle = enum.Enum('NewlineStyle', ['UNIX','DOS','WIN32','LF','CRLF'])

# include_directories typesV
End = enum.Enum('End', ["AFTER", "BEFORE"])

# message enums
MsgLevel = enum.Enum('MsgLevel', ["SEND_ERROR", "FATAL_ERROR", "WARNING", 
    "AUTHOR_WARNING", "CHECK_START", "CHECK_PASS", "CHECK_FAIL",
    "CONFIGURE_LOG", "STATUS", "VERBOSE", "DEBUG", "TRACE", "DEPRECATION",
    "NOTICE"])

#add_custom_command enums
BuildWhen = enum.Enum('BuildWhen', [ "PRE_BUILD", "PRE_LINK", "POST_BUILD"])

# cmake_policy 
PolicyMode = enum.Enum('PolicyMode', ["OLD", "NEW"])
