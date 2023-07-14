#include "cmPythonTarget.h"

#include "cmMakefile.h"
#include "cmPythonFileSet.h"
#include "cmPropertyMap.h"
#include "cmGlobalGenerator.h"
#include "cmPythonTypeConversion.h"

#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

void cmPythonTarget::bind(py::module_& m)
{
    auto cls = py::class_<cmPythonTarget>(m, "Target")
        .def("__str__", &cmPythonTarget::str)
        .def("__eq__", &cmPythonTarget::eq)
        .def_property_readonly("name", &cmPythonTarget::GetName)
        .def_property_readonly("properties", &cmPythonTarget::GetProperties)

        .def_property_readonly("include_directories", &cmPythonTarget::GetIncludeDirectories)
        .def_property_readonly("system_include_directories", &cmPythonTarget::GetSystemIncludeDirectories)
        .def_property_readonly("compile_options", &cmPythonTarget::GetCompileOptionsEntries)
        .def_property_readonly("compile_features", &cmPythonTarget::GetCompileFeaturesEntries)
        .def_property_readonly("compile_definitions", &cmPythonTarget::GetCompileDefinitionsEntries)
        .def_property_readonly("precompile_headers", &cmPythonTarget::GetPrecompileHeadersEntries)
        .def_property_readonly("source", &cmPythonTarget::GetSourceEntries)
        .def_property_readonly("link_options", &cmPythonTarget::GetLinkOptionsEntries)
        .def_property_readonly("link_directories", &cmPythonTarget::GetLinkDirectoriesEntries)
        .def_property_readonly("link_implementation", &cmPythonTarget::GetLinkImplementationEntries)
        .def_property_readonly("link_interface", &cmPythonTarget::GetLinkInterfaceEntries)
        .def_property_readonly("link_interface_direct", &cmPythonTarget::GetLinkInterfaceDirectEntries)
        .def_property_readonly("link_interface_direct_exclude", 
            &cmPythonTarget::GetLinkInterfaceDirectExcludeEntries)
        .def_property_readonly("header_sets", &cmPythonTarget::GetHeaderSetsEntries)
        .def_property_readonly("cxx_module_sets", &cmPythonTarget::GetCxxModuleSetsEntries)
        .def_property_readonly("interface_header_sets", &cmPythonTarget::GetInterfaceHeaderSetsEntries)
        .def_property_readonly("interface_cxx_module_sets", 
            &cmPythonTarget::GetInterfaceCxxModuleSetsEntries)
        .def_property_readonly("target_type", &cmPythonTarget::GetTargetType)
        .def_property_readonly("file_sets", &cmPythonTarget::GetFileSets)
        .def_property_readonly("interface_file_sets", &cmPythonTarget::GetInterfaceFileSets)
        .def_property_readonly("language_standards", &cmPythonTarget::GetLanguageStandards)
        .def_property_readonly("manually_added_dependencies", &cmPythonTarget::GetManuallyAddedDependencies)
        .def("get_property_command", &cmPythonTarget::GetPropertyCommand)
        ;
}

cmPythonTarget::cmPythonTarget(cmMakefile& makefile, std::string name)
:   cmPythonModBase(makefile),
    Name(std::move(name))
{
}

std::string cmPythonTarget::str() const
{
    return std::string("Target ") + Name;
}

const std::string& cmPythonTarget::GetName() const
{
    return Name;
}

void cmPythonTarget::ThrowKeyError() const
{
    const std::string err = "target " + GetName() + " has been deleted out from under us";
    throw py::key_error(err);
}

cmTarget& cmPythonTarget::GetTarget() 
{
    auto& targets = GetMakefile().GetTargets();
    auto it = targets.find(GetName());
    
    // not sure if this can happen, but let's be safe 
    if(it == targets.end()){
        ThrowKeyError();
    }
    return it->second;
}

const cmTarget& cmPythonTarget::GetTarget() const
{
    const auto& targets = GetMakefile().GetTargets();
    auto it = targets.find(GetName());
    
    // not sure if this can happen, but let's be safe 
    if(it == targets.end()){
        ThrowKeyError();
    }
    return it->second;
}


py::dict cmPythonTarget::GetLanguageStandards() const
{
    const auto& t = GetTarget();
    py::dict out;

    auto f = [&t, &out](const char* paramName)
    {
        const auto* val = t.GetLanguageStandardProperty(paramName);
        if (val) {
            out[paramName] = val->Value;
        }
    };

    // sigh. these strings are defined as constants but they're locked away 
    // where we can't get them
    // we really should return these keys as an enum as they are hard coded...
    f("CXX_STANDARD");
    f("C_STANDARD");
    f("CUDA_STANDARD");
    f("OBJC_STANDARD");
    f("OBJCXX_STANDARD");

    return out;
}

py::list cmPythonTarget::GetIncludeDirectories() const
{
    return ConvertToList(GetTarget().GetIncludeDirectoriesEntries());
}

py::list cmPythonTarget::GetSystemIncludeDirectories() const
{
    return ConvertToList(GetTarget().GetSystemIncludeDirectories());
}

py::list cmPythonTarget::GetCompileOptionsEntries() const
{
    return ConvertToList(GetTarget().GetCompileOptionsEntries());
}

py::list cmPythonTarget::GetCompileFeaturesEntries() const
{
    return ConvertToList(GetTarget().GetCompileFeaturesEntries());
} 

py::list cmPythonTarget::GetCompileDefinitionsEntries() const
{
    return ConvertToList(GetTarget().GetCompileDefinitionsEntries());
}

py::list cmPythonTarget::GetPrecompileHeadersEntries() const
{
    return ConvertToList(GetTarget().GetPrecompileHeadersEntries());
}

py::list cmPythonTarget::GetSourceEntries() const
{
    return ConvertToList(GetTarget().GetSourceEntries());
}

py::list cmPythonTarget::GetLinkOptionsEntries() const
{
    return ConvertToList(GetTarget().GetLinkOptionsEntries());
}

py::list cmPythonTarget::GetLinkDirectoriesEntries() const
{
    return ConvertToList(GetTarget().GetLinkDirectoriesEntries());
}

py::list cmPythonTarget::GetLinkImplementationEntries() const
{
    return ConvertToList(GetTarget().GetLinkImplementationEntries());
}

py::list cmPythonTarget::GetLinkInterfaceEntries() const
{
    return ConvertToList(GetTarget().GetLinkInterfaceEntries());
}

py::list cmPythonTarget::GetLinkInterfaceDirectEntries() const
{
    return ConvertToList(GetTarget().GetLinkInterfaceDirectEntries());
}

py::list cmPythonTarget::GetLinkInterfaceDirectExcludeEntries() const
{
    return ConvertToList(GetTarget().GetLinkInterfaceDirectExcludeEntries());
}

py::list cmPythonTarget::GetHeaderSetsEntries() const
{
    return ConvertToList(GetTarget().GetHeaderSetsEntries());
}

py::list cmPythonTarget::GetCxxModuleSetsEntries() const
{
    return ConvertToList(GetTarget().GetCxxModuleSetsEntries());
}

py::list cmPythonTarget::GetInterfaceHeaderSetsEntries() const
{
    return ConvertToList(GetTarget().GetInterfaceHeaderSetsEntries());
}

py::list cmPythonTarget::GetInterfaceCxxModuleSetsEntries() const
{
    return ConvertToList(GetTarget().GetInterfaceCxxModuleSetsEntries());
}

cmStateEnums::TargetType cmPythonTarget::GetTargetType() const
{
    return GetTarget().GetType();
}

pybind11::dict cmPythonTarget::BuildFileSetDict(const std::vector<std::string>& list) const
{
    py::dict ret;

    for(const auto& name : list) {
        const cmFileSet* f = GetTarget().GetFileSet(name);
        ret[py::str(name)] = cmPythonFileSet(*f);
    }

    return ret;
}

py::dict cmPythonTarget::GetFileSets() const
{
    return BuildFileSetDict(GetTarget().GetAllFileSetNames());
}

py::dict cmPythonTarget::GetInterfaceFileSets() const
{
    return BuildFileSetDict(GetTarget().GetAllInterfaceFileSets());
}

py::list cmPythonTarget::GetManuallyAddedDependencies() const
{
    py::list out;

    const auto& m = GetTarget().GetUtilities();
    for(const auto& v : m) {
        const auto& util = v.Value;
        py::dict d;
        d["name"] = util.first;
        d["cross"] = util.second;
        out.append(d);
    }
    return out;
}

py::object cmPythonTarget::GetPropertyCommand(const std::string &propName) 
{
    const auto& target = GetTarget();

    // try to replicate the behaviour of get_target_property
    cmValue prop;

#if 0
    // this logic is not quite correct
    bool alias = GetMakefile().IsAlias(GetName()); 

    if(alias) {
        if (propName == "ALIASED_TARGET") {
          prop = tgt->GetName();
        } else if (propName == "ALIAS_GLOBAL") {
          prop = GetMakefile().GetGlobalGenerator()->IsAlias(targetName);
        }
    }
#endif

    if (!prop) {
        prop = target.GetComputedProperty(propName, GetMakefile());
    }

    if (!prop) {
        prop = target.GetProperty(propName);
    }
    
    py::object ret = ConvertArgCMakeToPython(prop);

    // throw instead of returning none
    if(ret.is_none()) {
        std::string errMsg = "no property for " + propName +
            " found in target " + target.GetName();
        throw pybind11::key_error(errMsg);
    }

    return ret;
}

bool cmPythonTarget::eq(const cmPythonTarget& other) const
{
    return GetName() == other.GetName();
}

py::dict cmPythonTarget::GetProperties() 
{
    const auto& t = GetTarget();
    const cmPropertyMap& props = t.GetProperties();
    const auto& list = props.GetList();

    py::dict res;
    for(const auto& item : list) {
        res[py::str(item.first)] = ConvertArgCMakeToPython(item.second);
    }

#if 0
    // replicate the logic from get_target_property()
    // this logic is not quite correct
    if (GetMakefile().IsAlias(GetName())) {
        res["ALIASED_TARGET"] = t.GetName();
        res["ALIAS_GLOBAL"] = GetMakefile().GetGlobalGenerator()->IsAlias(GetName());
    }
#endif

    // At this point, we don't include computed properties - you'd have to reverse engineer
    // how and when all of their names are mangled..
    return res;
}
