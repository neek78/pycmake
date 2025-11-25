/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonEnums.h"
#include "cmFileSet.h"
#include "cmStateTypes.h"
#include "cmScriptType.h"

#include <pybind11/native_enum.h>

namespace py = pybind11;

void BindEnums(pybind11::module_& m)
{
    py::native_enum<cmScriptType>(m, "ScriptType", "enum.Enum")
        .value("Native", cmScriptType::Native)
        .value("Python", cmScriptType::Python)
        .export_values()
        .finalize();

    py::native_enum<cmStateEnums::ScriptedCommandType>(m, "ScriptedCommandType", "enum.Enum")
        .value("Macro", cmStateEnums::ScriptedCommandType::Macro)
        .value("Function", cmStateEnums::ScriptedCommandType::Function)
        .value("Python", cmStateEnums::ScriptedCommandType::Python)
        .value("Command", cmStateEnums::ScriptedCommandType::Command)
        .export_values()
        .finalize();

    py::native_enum<cmFileSetVisibility>(m, "FileSetVisibility", "enum.Enum")
        .value("PRIVATE", cmFileSetVisibility::Private)
        .value("PUBLIC", cmFileSetVisibility::Public)
        .value("INTERFACE", cmFileSetVisibility::Interface)
        .export_values()
        .finalize();

    py::native_enum<cmStateEnums::CacheEntryType> (m, "CacheEntryType", "enum.Enum")
        .value("BOOL", cmStateEnums::CacheEntryType::BOOL)
        .value("PATH", cmStateEnums::CacheEntryType::PATH)
        .value("FILEPATH", cmStateEnums::CacheEntryType::FILEPATH)
        .value("STRING", cmStateEnums::CacheEntryType::STRING)
        // not 100% sure we should expose these next three
        .value("INTERNAL", cmStateEnums::CacheEntryType::INTERNAL)
        .value("STATIC", cmStateEnums::CacheEntryType::STATIC)
        .value("UNINITIALIZED", cmStateEnums::CacheEntryType::UNINITIALIZED)
        .export_values()
        .finalize();

    py::native_enum<cmStateEnums::TargetType>(m, "TargetType", "enum.Enum")
        .value("EXECUTABLE", cmStateEnums::TargetType::EXECUTABLE)
        .value("STATIC_LIBRARY", cmStateEnums::TargetType::STATIC_LIBRARY)
        .value("SHARED_LIBRARY", cmStateEnums::TargetType::SHARED_LIBRARY)
        .value("MODULE_LIBRARY", cmStateEnums::TargetType::MODULE_LIBRARY)
        .value("OBJECT_LIBRARY", cmStateEnums::TargetType::OBJECT_LIBRARY)
        .value("UTILITY", cmStateEnums::TargetType::UTILITY)
        .value("GLOBAL_TARGET", cmStateEnums::TargetType::GLOBAL_TARGET)
        .value("INTERFACE_LIBRARY", cmStateEnums::TargetType::INTERFACE_LIBRARY)
        .value("UNKNOWN_LIBRARY", cmStateEnums::TargetType::UNKNOWN_LIBRARY)
        .export_values()
        .finalize();

}
