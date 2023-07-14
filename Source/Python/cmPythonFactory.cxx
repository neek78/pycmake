/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */


#include "cmPythonFactory.h"
#include "cmPythonCore.h"
#include "cmPythonModules.h"

#include "cmVersion.h"
#include "cmMakefile.h"
#include "cmMessenger.h"

#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

void cmPythonFactory::bind(py::module_& m)
{
    auto cls = py::class_<cmPythonFactory, std::shared_ptr<cmPythonFactory>>(m, "Factory");

    cls.def_property_readonly("global_attr", 
        &cmPythonFactory::global_attr, py::return_value_policy::reference_internal);

    cls.def_property_readonly("var", 
        &cmPythonFactory::var, py::return_value_policy::reference_internal);

    cls.def_property_readonly("combined_var", 
        &cmPythonFactory::combined_var, py::return_value_policy::reference_internal);

    cls.def_property_readonly("cache", 
        &cmPythonFactory::cache, py::return_value_policy::reference_internal);

    cls.def_property_readonly("raw", 
        &cmPythonFactory::raw, py::return_value_policy::reference_internal);

    cls.def_property_readonly("source_dirs", 
        &cmPythonFactory::source_dirs, py::return_value_policy::reference_internal);

    cls.def_property_readonly("dir_attr", 
        &cmPythonFactory::dir_attr, py::return_value_policy::reference_internal);

    cls.def_property_readonly("root_source_dir", 
        &cmPythonFactory::root_source_dir, py::return_value_policy::reference_internal);

    cls.def_property_readonly("function_launcher", 
        &cmPythonFactory::function_launcher, py::return_value_policy::reference_internal);

    cls.def_property_readonly("py_functions", 
        &cmPythonFactory::pyFunctions, py::return_value_policy::reference_internal);

    cls.def_property_readonly("cmake_version",  &cmPythonFactory::cmake_version);

    cls.def("convert_to_relative_source_path",  
        &cmPythonFactory::ConvertToRelativeSourcePath);

    cls.doc() = R"pbdoc(
      Interface to cmake builtin functions
    )pbdoc";
}

std::shared_ptr<cmPythonFactory> cmPythonFactory::Create(cmMakefile& mf) 
{
    return std::shared_ptr<cmPythonFactory>(new cmPythonFactory(mf));
}

cmPythonFactory::cmPythonFactory(cmMakefile& makefile)
:   GlobalAttr(*makefile.GetCMakeInstance()),
    Raw(makefile),
    Var(makefile),
    CombinedVar(makefile),
    Cache(makefile),
    SourceDirs(*makefile.GetGlobalGenerator()),
    DirAttr(makefile),
    FunctionLauncher(makefile),
    PyFunctions(makefile),
    Makefile(makefile),
    BuiltinModule(cmPythonModules::GetBuiltinModule()),
    EntryPoint(cmPythonModules::GetRootModule().attr("invoke_cmake_main"))
{
}

py::kwargs cmPythonFactory::BuildStdKwArgs()
{
    // Pass our factory object into the python code and let it set itself up
    py::module_ rootModule = cmPythonModules::GetRootModule();
    py::object dirObject = rootModule.attr("CMakeDir")();
    py::function injectFactoryDir = dirObject.attr("_inject_factory"); 

    py::object globalObject = rootModule.attr("CMakeGlobal")();
    py::function injectFactoryGlobal = globalObject.attr("_inject_factory"); 

    // inject ourselves into the new python objects. Using a shared ptr means 
    // our lifespan can be extended if something in python holds a ref to anything
    // we hold
    injectFactoryDir(shared_from_this());
    injectFactoryGlobal(shared_from_this());

    py::kwargs ret;
    ret["cm_dir"] = dirObject;
    ret["cm_global"] = globalObject;
    return ret;
}

py::function cmPythonFactory::GetUserEntryPoint(const py::module_& mod)
{
    return mod.attr("cmake_main");
}

void cmPythonFactory::CheckReturnValue(const py::object& ret)
{
    // elephants don't have keys
    if (!ret.is_none()) {
        // Not an error - just issue the warning
        Makefile.IssueMessage(MessageType::WARNING,
            "Python script returned values to cmake - these will be ignored");
    }
}

bool cmPythonFactory::InvokePythonEntryPoint(const pybind11::module_& clientMod)
{
    const char* msg = nullptr;

    // launch the python script. 
    try {
        msg = "Exception finding entrypoint -\n";
        py::function cmakeMain = GetUserEntryPoint(clientMod);

        msg = "Exception whilst building invocation objects-\n";
        py::kwargs kwargs = BuildStdKwArgs();

        msg = "Exception whilst executing python script -\n";
        py::object ret = EntryPoint(cmakeMain, kwargs);

        CheckReturnValue(ret);

        return true;
    }
    catch(const pybind11::error_already_set& e) {
        OutputExceptionAndSetFatal(*Makefile.GetMessenger(), msg, e);
        return false;
    }
    catch(const std::exception& e) {
        OutputExceptionAndSetFatal(*Makefile.GetMessenger(), msg, e);
        return false;
    }
}

cmPythonGlobalAttr& cmPythonFactory::global_attr()
{
    return GlobalAttr;
}

cmPythonRaw& cmPythonFactory::raw()
{
    return Raw;
}

cmPythonCombinedVar& cmPythonFactory::combined_var()
{
    return CombinedVar;
}

cmPythonVar& cmPythonFactory::var()
{
    return Var;
}

cmPythonCache& cmPythonFactory::cache()
{
    return Cache;
}

cmPythonSourceDirs& cmPythonFactory::source_dirs()
{
    return SourceDirs;
}

cmPythonDirAttr& cmPythonFactory::dir_attr()
{
    return DirAttr;
}

py::object cmPythonFactory::root_source_dir()
{
    // a bit ugly - need to search for the right makefile.. 
    // i think GlobalGenerator->GetMakefiles()[0] is the root makefile, but not 
    // 100% sure
    const std::string& root = Makefile.GetHomeDirectory();
    return SourceDirs.getitem(root);
}

cmPythonFunctionLauncher& cmPythonFactory::function_launcher()
{
    return FunctionLauncher;
}

cmPythonPyFunctions& cmPythonFactory::pyFunctions()
{
    return PyFunctions;
}

std::filesystem::path cmPythonFactory::ConvertToRelativeSourcePath(const std::filesystem::path& path) const
{
    return cmSystemTools::RelativePath(Makefile.GetHomeDirectory(), path.string());
}

py::dict cmPythonFactory::cmake_version()
{
    using namespace py::literals;

    auto encoded = CMake_VERSION_ENCODE(
        cmVersion::GetMajorVersion(), 
        cmVersion::GetMinorVersion(), 
        cmVersion::GetPatchVersion());

    return py::dict (
        "version_str"_a = cmVersion::GetCMakeVersion(),
        "major"_a = cmVersion::GetMajorVersion(),
        "minor"_a = cmVersion::GetMinorVersion(),
        "patch"_a = cmVersion::GetPatchVersion(),
        "tweak"_a = cmVersion::GetTweakVersion(),
        "suffix"_a = cmVersion::GetSuffix(),
        "isDirty"_a = cmVersion::GetVersionIsDirty(),
        "encoded"_a = encoded
        );
}

