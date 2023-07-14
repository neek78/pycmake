/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonModBase.h"
#include "cmPythonUtils.h"
#include "cmStateTypes.h"

#include <pybind11/embed.h>

class cmTarget;

class cmPythonTarget : public cmPythonModBase
{
public:
    static void bind(pybind11::module_& m);
    cmPythonTarget(cmMakefile& makefile, std::string name);

private:
    std::string str() const;
    const std::string& GetName() const;
    cmStateEnums::TargetType GetTargetType() const;

    pybind11::list GetIncludeDirectories() const;
    pybind11::list GetSystemIncludeDirectories() const;
    pybind11::list GetCompileOptionsEntries() const;
    pybind11::list GetCompileFeaturesEntries() const;
    pybind11::list GetCompileDefinitionsEntries() const;
    pybind11::list GetPrecompileHeadersEntries() const;
    pybind11::list GetSourceEntries() const;
    pybind11::list GetLinkOptionsEntries() const;
    pybind11::list GetLinkDirectoriesEntries() const;
    pybind11::list GetLinkImplementationEntries() const;
    pybind11::list GetLinkInterfaceEntries() const;
    pybind11::list GetLinkInterfaceDirectEntries() const;
    pybind11::list GetLinkInterfaceDirectExcludeEntries() const;
    pybind11::list GetHeaderSetsEntries() const;
    pybind11::list GetCxxModuleSetsEntries() const;
    pybind11::list GetInterfaceHeaderSetsEntries() const;
    pybind11::list GetInterfaceCxxModuleSetsEntries() const;
    pybind11::list GetManuallyAddedDependencies() const;

    pybind11::dict GetProperties();

    pybind11::dict GetLanguageStandards() const;
    pybind11::dict BuildFileSetDict(const std::vector<std::string>& list) const;
    pybind11::dict GetFileSets() const;
    pybind11::dict GetInterfaceFileSets() const;

    pybind11::object GetPropertyCommand(const std::string &propName) ;

    void ThrowKeyError() const;
    const cmTarget& GetTarget() const;
    bool eq(const cmPythonTarget& other) const;

    cmTarget& GetTarget();

    const std::string Name;
};

