/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonDirAttr.h"
#include "cmPythonTypeConversion.h"
#include "cmPythonTarget.h"
#include "cmMakefile.h"
#include "cmPropertyMap.h"
#include "cmStateDirectory.h"
#include "cmSetPropertyCommand.h"
#include "cmPythonSourceFile.h"
#include "cmGlobalGenerator.h"
#include "cmPolicies.h"

#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include <iostream>

namespace py = pybind11;

void cmPythonDirAttr::bind(pybind11::module_& m)
{
    auto cls = pybind11::class_<cmPythonDirAttr>(m, "DirAttr")
        .def("__repr__", &cmPythonDirAttr::repr)
        .def_property_readonly("properties", &cmPythonDirAttr::Properties)
        .def_property_readonly("list_files", &cmPythonDirAttr::ListFiles)
        .def_property_readonly("script_type", &cmPythonDirAttr::ScriptType)
        .def_property_readonly("output_files", &cmPythonDirAttr::OutputFiles)
        .def_property_readonly("source_dir", &cmPythonDirAttr::SourceDir)
        .def_property_readonly("relative_source_dir", &cmPythonDirAttr::RelativeSourceDir)
        .def_property_readonly("build_dir", &cmPythonDirAttr::BuildDir)
        .def_property_readonly("project_name", &cmPythonDirAttr::ProjectName)
        .def_property_readonly("include_dirs", &cmPythonDirAttr::IncludeDirs)
        .def_property_readonly("compile_definitions", &cmPythonDirAttr::CompileDefinitions)
        .def_property_readonly("compile_options", &cmPythonDirAttr::CompileOptions)
        .def_property_readonly("link_options", &cmPythonDirAttr::LinkOptions)
        .def_property_readonly("link_directories", &cmPythonDirAttr::LinkDirectories)

        .def_property_readonly("is_root_source_dir", &cmPythonDirAttr::IsRootSourceDir)
        .def_property_readonly("parent", &cmPythonDirAttr::Parent)
        .def_property_readonly("children", &cmPythonDirAttr::Children)

        // targets
        .def_property_readonly("build_system_targets", &cmPythonDirAttr::BuildSystemTargets)
        .def_property_readonly("imported_targets", &cmPythonDirAttr::ImportedTargets)
        .def_property_readonly("alias_targets", &cmPythonDirAttr::AliasTargets)
        .def("find_target", &cmPythonDirAttr::FindTarget)
        .def("get_source_file", &cmPythonDirAttr::GetSourceFile)
        .def("get_policy_status", &cmPythonDirAttr::GetPolicyStatus)
        
        .def("__eq__", &cmPythonDirAttr::eq)
        ;
}

cmPythonDirAttr::cmPythonDirAttr(cmMakefile& makefile)
:   cmPythonModBase(makefile)
{
}

py::str cmPythonDirAttr::repr()
{
    return std::string("DirAttr('") + GetMakefile().GetCurrentSourceDirectory() + "')";
}

cmStateDirectory cmPythonDirAttr::GetDirectory() const
{
    return GetMakefile().GetStateSnapshot().GetDirectory();
}

bool cmPythonDirAttr::IsRootSourceDir() const
{
    return GetMakefile().IsRootMakefile();
}

CMakeScriptType cmPythonDirAttr::ScriptType() const
{
    return GetMakefile().GetIsPython() ? 
        CMakeScriptType::Python :
        CMakeScriptType::CMakeScript;
}

py::dict cmPythonDirAttr::Properties() const
{
    py::dict ret;
    const auto& props = GetMakefile().GetPropertyKeys();
    for(const auto& key: props) {
        const cmValue val = GetMakefile().GetProperty(key);
        ret[py::str(key)] = ConvertArgCMakeToPython(val);
    }
    return ret;
}

py::list cmPythonDirAttr::ListFiles() const
{
    // FIXME:convert to path?
    return ConvertToList(GetMakefile().GetListFiles());
}

py::list cmPythonDirAttr::OutputFiles() const
{
    // FIXME:convert to path?
    return ConvertToList(GetMakefile().GetOutputFiles());
}

std::filesystem::path cmPythonDirAttr::SourceDir() const
{
    // FIXME  should we collapse these?
    return GetDirectory().GetCurrentSource();
}

std::filesystem::path cmPythonDirAttr::BuildDir() const
{
    // FIXME  should we collapse these?
    return GetDirectory().GetCurrentBinary();
}

std::filesystem::path cmPythonDirAttr::RelativeSourceDir() const
{
    // FIXME  should we collapse these?
    return cmSystemTools::RelativePath( 
        GetMakefile().GetHomeDirectory(), GetDirectory().GetCurrentSource());
}

std::string cmPythonDirAttr::ProjectName() const
{
    return GetMakefile().GetStateSnapshot().GetProjectName();
}

py::list cmPythonDirAttr::IncludeDirs() const
{
    return ConvertToPathList(GetDirectory().GetIncludeDirectoriesEntries());
}

py::list cmPythonDirAttr::CompileDefinitions() const
{
    return ConvertToList(GetDirectory().GetCompileDefinitionsEntries());
}

py::list cmPythonDirAttr::CompileOptions() const
{
    return ConvertToList(GetDirectory().GetCompileOptionsEntries());
}

py::list cmPythonDirAttr::LinkOptions() const
{
    return ConvertToList(GetDirectory().GetLinkOptionsEntries());
}

py::list cmPythonDirAttr::LinkDirectories() const
{
    return ConvertToPathList(GetDirectory().GetLinkDirectoriesEntries());
}

py::dict cmPythonDirAttr::BuildSystemTargets()
{
    py::dict out;
    for(const auto& t : GetMakefile().GetTargets()) {
       const std::string& name = t.first;
       out[py::str(name)] = cmPythonTarget(GetMakefile(), name);
    }
    return out;
}

py::dict cmPythonDirAttr::ImportedTargets() 
{
    py::dict out;
    for(const auto* t : GetMakefile().GetImportedTargets()) {
        assert(t);
        const std::string& name = t->GetName();
        out[py::str(name)] = cmPythonTarget(GetMakefile(), name);
    }
    return out;
}

py::dict cmPythonDirAttr::AliasTargets() const
{
    return ConvertToDict(GetMakefile().GetAliasTargets());
}

// this method uses the same call that get_target_properties uses to find a target
// from a string
cmPythonTarget cmPythonDirAttr::FindTarget(const std::string& name) 
{
    const cmTarget* t = GetMakefile().FindTargetToUse(name);
    if (!t) {
        std::string msg = "could not find target '" + name + "'";
        throw pybind11::key_error(msg);
    }
    return cmPythonTarget(GetMakefile(), t->GetName());
}

cmPythonSourceFile cmPythonDirAttr::GetSourceFile(const pybind11::str& name)
{
    std::string p = name;
    std::string absolutePath = cmSystemTools::CollapseFullPath(
        p, GetMakefile().GetCurrentSourceDirectory());

    cmSourceFile* sf = GetMakefile().GetSource(absolutePath, cmSourceFileLocationKind::Known);

    if (!sf) {
        throw pybind11::key_error("couldn't find source file");
    }

    return cmPythonSourceFile(*sf);
}

cmMakefile* cmPythonDirAttr::FindMakefile(const std::string& key) const
{
    const auto& mfIdx = GetMakefile().GetGlobalGenerator()->GetMakefiles();
    auto res = std::find_if(std::begin(mfIdx), std::end(mfIdx), [key](const auto& k) {
            return (k->GetCurrentSourceDirectory() == key);
            });

   if (res == mfIdx.end()) {
       return nullptr;
   }
   return res->get();
}

pybind11::object cmPythonDirAttr::Parent() const
{
    auto parent = GetMakefile().GetStateSnapshot().GetBuildsystemDirectoryParent();
    if (!parent.IsValid()) {
        return py::none();
    }

    cmMakefile* mf = FindMakefile(parent.GetDirectory().GetCurrentSource());

    return py::cast(cmPythonDirAttr(*mf));
}

pybind11::dict cmPythonDirAttr::Children() const
{
    auto children = GetMakefile().GetStateSnapshot().GetChildren();

    py::dict out;

    for(const auto& child : children) {
        const std::string& path = child.GetDirectory().GetCurrentSource();
        cmMakefile* mf = FindMakefile(path);

        assert(mf != nullptr);

        // copy d here, but it really is only the size of one ref
        cmPythonDirAttr d(*mf);
        out[py::str(d.RelativeSourceDir().string())] = d;
    }

    return out;
}

bool cmPythonDirAttr::eq(const cmPythonDirAttr& other) const
{
    return GetMakefile().GetCurrentSourceDirectory() == other.GetMakefile().GetCurrentSourceDirectory();
}

py::object cmPythonDirAttr::GetPolicyStatus(const pybind11::object& policy) const
{
    using namespace literals;

    cmPolicies::PolicyID pid;
    if (py::isinstance<py::int_>(policy)) {
        int pol = policy.cast<int>();

        if(!cmPolicies::GetPolicyID(pol, pid)) {
            py::str err = 
                "policy "_pys + py::str(py::int_(pol)) +
                " is not known to this version of CMake."_pys;
            throw py::value_error(err);
        }

    } else {
        const std::string id = std::string(py::str(policy));

        if(!cmPolicies::GetPolicyID(id.c_str(), pid)) {
            py::str err = 
                "policy "_pys + py::str(id) +
                " is not known to this version of CMake."_pys;
            throw py::value_error(err);
        }
    }

    cmPolicies::PolicyStatus policyStatus = GetMakefile().GetPolicyStatus(pid);

    switch (policyStatus) {
        case cmPolicies::OLD:
            return py::bool_(false);

        case cmPolicies::WARN:
            return py::none();

        case cmPolicies::NEW:
            return py::bool_(true);

        case cmPolicies::REQUIRED_IF_USED:
        case cmPolicies::REQUIRED_ALWAYS:
            py::str err = 
                py::str(cmPolicies::GetRequiredPolicyError(pid)) +
                "\nThe call to get_policy_status "_pys +  
                py::str(policy) +
                " ...) at which this "_pys +
                "error appears requests the policy, and this version of CMake "_pys +
                "requires that the policy be set to NEW before it is checked."_pys;
            throw py::value_error(err);
    }
}
