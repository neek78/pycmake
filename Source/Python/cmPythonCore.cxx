/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
                              
#include "cmPythonCore.h"
#include "cmPythonConstants.h"
#include "cmPythonModules.h"

#include "cmPythonBuiltinModule.h"
#include "cmake.h"

#include <string>
#include <sstream>

#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

void cmPythonCore::DisableBytecodeCache()
{
    cmPythonModules::GetModuleSys().attr("dont_write_bytecode") = true;
}

bool cmPythonCore::init()
{
    const char* msg = "";
    // We have this in a separate (non-constructor) function because of
    // problems with destruction of the error_already_set exception objects. 
    //
    // This seems to rely on holding the python GIL, and when we destruct, the scoped_interpreter 
    // (and thus holding the GIL) goes, and the error_already_set destructor blows up. 
    // The owner of us doesn't care - it just needs to 
    // know construction failed, so just return false after logging the problem
    try {
        // check for double init.
        assert(!Guard);

        // init the scoped interpreter here (it can throw)
        msg = "initialising python interpreter";
        Guard = std::make_unique<pybind11::scoped_interpreter>();

        // for now turn off bytecode caching so we don't create __pycache__ dirs everywhere.
        // perhaps it's worth turning this on in future once things have settled down.
        msg = "disabling bytecode cache";
        DisableBytecodeCache();

        // load cmake.py
        msg = "loading cmake module";
        buildRootModule();
    }
    catch(const pybind11::error_already_set& e) {
        std::stringstream ss;
        ss << "Error whilst " << msg << " - " << e.what() << std::endl;
        ss << "type " << e.type() << std::endl;
        ss << "value " << e.value() << std::endl;
        ss << "trace " << e.trace() << std::endl;

        cmSystemTools::Error(ss.str());
        return false;
    }
    catch(const std::exception& e) {
        std::stringstream ss;
        ss << "Error whilst " << msg << " - " << e.what() << std::endl;
        cmSystemTools::Error(ss.str());
        return false;
    }

    return true;
}

py::object cmPythonCore::GetPythonPath()
{
    // go via sys.path = we do it this way as Py_GetPath() is deprecated
    return cmPythonModules::GetModuleSys().attr("path");
}

std::optional<std::wstring> cmPythonCore::GetPythonHomeStr()
{
    // go via sys.path = we do it this way as Py_GetPath() is deprecated
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    if (config.home == nullptr) {
        return {};
    }
    return std::wstring(config.home);
}

void cmPythonCore::AppendPythonPath(const std::filesystem::path& path)
{
    py::object p = GetPythonPath();

    // this needs pybind11/stl/filesystem.h to work.
    // ... and it still doesn't work. std::filesystem::path objects get
    // converted (correctly) to pathlib.PosixPath objects - but the python module 
    // loader insists on only str objects (as of 3.11.3) and will quietly ignore
    // non-str objects
    // hence, we explicitly call c_str()
    p.attr("append")(path.c_str());
}

std::wstring cmPythonCore::JoinList(const py::object& o)
{
    assert(!o.is_none());

    py::str res(":");
    auto s = res.attr("join")(o);
    return s.cast<std::wstring>();
}

std::wstring cmPythonCore::GetPythonPathStr()
{
    // join the python path down to a string for use in the --python-information output
    return JoinList(GetPythonPath());
}

std::filesystem::path cmPythonCore::GetModulePath()
{
    return std::filesystem::path(cmSystemTools::GetCMakeRoot()) / PYTHON_MOD_DIR;
}

std::string cmPythonCore::GetModuleName()
{
    return PYTHON_CMAKE_MOD;
}

void cmPythonCore::buildRootModule()
{
    AppendPythonPath(GetModulePath());

    // load module
    RootModule = py::module_::import(GetModuleName().c_str());

    // bind in c++ code
    py::module_ builtin = RootModule.def_submodule(PYTHON_BUILTIN_MOD.c_str(), "interface to c++ code");
    BindBuiltinModule(builtin);

    // submodule for client code to load into
    py::module_ client = RootModule.def_submodule(PYTHON_CLIENT_MOD.c_str(), "location of client code");
}

void cmPythonCore::ipython()
{
    py::exec("import IPython;IPython.embed()");
}

bool cmPythonCore::PrintPythonInfo(std::wostream& os)
{
    // check init()'s been called
    assert(Guard);

    try {
        os << "\n";
        os << "compiled with python - " << PY_VERSION << "\n";
        os << "running with python - " << Py_GetVersion() << "\n";
        os << "build info - " << Py_GetBuildInfo() << "\n";
        os << "compiler - " << Py_GetCompiler() << "\n";
        os << "platform - " << Py_GetPlatform() << "\n";

        // no longer use Py_GetPath() as it's deprecated. 
        os << "starting python path - " << GetPythonPathStr() << "\n\n";
        //os << "starting python path old: " << Py_GetPath() << "\n\n";

        os << "cmake module python path - " << GetModulePath().c_str() << "\n\n";
        os << "cmake root module name - " << GetModuleName().c_str() << "\n\n";

        // PyConfig.home
        auto home = GetPythonHomeStr();
        if(home) {
            os << "python home - " << *home << "\n";
        } else {
            os << "python home not set" << "\n";
        }
    } catch (const std::exception& e) {
        os << "error getting python version info - " << e.what() << "\n";
        return false;
    }
    return true;
}

