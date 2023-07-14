/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
                              
#include "cmPythonScript.h"
#include "cmPythonFactory.h"
#include "cmPythonCore.h"
#include "cmPythonConstants.h"
#include "cmPythonModules.h"

#include "cmMessenger.h"
#include "cmMakefile.h"
#include "cmSystemTools.h"

#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include <memory>
#include <filesystem>

namespace py = pybind11;

cmPythonScript::cmPythonScript(
      cmMakefile& makefile,
      const std::filesystem::path& path, 
      const std::filesystem::path& filename) 
:     Makefile(makefile),
      FullPath(path/filename),
      MangledModName(Mangle(FullPath))
{
}

std::string cmPythonScript::Mangle(const std::filesystem::path& fullPath) const
{
    std::filesystem::path relativePath = cmSystemTools::RelativePath(
            Makefile.GetHomeDirectory(), fullPath.string());

    std::string out;
    for (auto c : relativePath.string()) {
        if(c == '/' || c == '\\' || c == ':' || c == '.') {
            out += "_";
        } else {
            out += c;
        }
    }
    return out;
}
// https://stackoverflow.com/questions/8718885/import-module-from-string-variable
// https://stackoverflow.com/questions/301134/how-can-i-import-a-module-dynamically-given-its-name-as-string
// https://bugs.python.org/issue21436
void cmPythonScript::LoadModule()
{
    py::module_ clientMod = cmPythonModules::GetClientModule();
    py::module_ importLibUtil = cmPythonModules::GetModuleImportLibUtil();

    py::str fullModName = PYTHON_CLIENT_MOD_FULL + "." + MangledModName;
    py::object spec = importLibUtil.attr("spec_from_file_location")(fullModName, FullPath);
    ClientModule = importLibUtil.attr("module_from_spec")(spec);
    spec.attr("loader").attr("exec_module")(ClientModule);
    clientMod.add_object(MangledModName.c_str(), ClientModule);

    // apparently we need to add to sys.modules ourselves
    // this is really to keep IPython happy - everything else seems to work fine without this.
    // https://stackoverflow.com/questions/65256144/how-to-get-a-path-for-a-module-imported-by-importlib-where-a-class-was-defined
    py::dict modules = cmPythonModules::GetModuleSys().attr("modules");
    modules[fullModName] = clientMod;
}

bool cmPythonScript::ParseScript()
{
    // mod should not yet be initialised...
    assert(!ClientModule);

    auto msg = std::string("Exception whilst importing python script ") + 
        FullPath.string() +" -\n";

    try {
        LoadModule();
    }
    catch(const pybind11::error_already_set& e) {
        OutputExceptionAndSetFatal(*Makefile.GetMessenger(), msg, e);
        return false;
    }
    catch(const std::exception& e) {
        OutputExceptionAndSetFatal(*Makefile.GetMessenger(), msg, e);
        return false;
    }

    assert(ClientModule);
    return true;
}

bool cmPythonScript::RunScript()
{
    assert(ClientModule);

    auto factory = cmPythonFactory::Create(Makefile);

    if(!factory->InvokePythonEntryPoint(ClientModule)) {
        // Script failed - it should have set + logged error state
        return false;
    }

#if 0
    std::cout << "  script completed (factory use count is " << 
        factory.use_count() << ")\n";
#endif
    
    if(Makefile.IsRootMakefile()) {
        return CheckRequiredCommands(factory->raw());
    }

    return true;
}

bool cmPythonScript::CheckRequiredCommands(const cmPythonRaw& raw)
{
    bool ret = true;
    if(!raw.GetSeenProjectCommand()) {
        Makefile.IssueMessage(MessageType::FATAL_ERROR,
            "Root makefile did not include the required call to project()");
        cmSystemTools::SetFatalErrorOccurred();
        ret = false;
    }

    if(!raw.GetSeenMinVersionRequiredCommand()) {
        Makefile.IssueMessage(MessageType::FATAL_ERROR,
            "Root makefile did not include the required call to cmake_minimum_required()");
        cmSystemTools::SetFatalErrorOccurred();
        ret = false;
    }

    return ret;
}
