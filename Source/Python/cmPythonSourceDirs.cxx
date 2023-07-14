#include "cmPythonSourceDirs.h"
#include "cmPythonDirAttr.h"
#include "cmPythonTypeConversion.h"
#include "cmMakefile.h"
#include "cmPropertyMap.h"

#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

void cmPythonSourceDirs::bind(pybind11::module_& m)
{
    auto cls = pybind11::class_<cmPythonSourceDirs, cmPythonDictInterface>(m, "Directories");
}

cmPythonSourceDirs::cmPythonSourceDirs(cmGlobalGenerator& globalGenerator)
:   GlobalGenerator(globalGenerator)
{
}


#if 0
std::string cmPythonDirectories::collapse(const std::string& path) const
{
    const auto& srcDir = GetMakefile().GetCurrentSourceDirectory();
    return cmSystemTools::CollapseFullPath(path, srcDir);
}
#endif

const std::vector<std::unique_ptr<cmMakefile>>& cmPythonSourceDirs::GetIdx() const
{
    return GlobalGenerator.GetMakefiles();
}

cmMakefile* cmPythonSourceDirs::FindBySrcDir(const KeyType& key) const
{
    auto res = std::find_if(std::begin(GetIdx()), std::end(GetIdx()), [key](const auto& k) {
            return (k->GetCurrentSourceDirectory() == key);
            });

    if(res == GetIdx().end()) {
        return nullptr;
    }

    return res->get();
}

py::object cmPythonSourceDirs::getitem(const py::str& key) 
{
    auto* res = FindBySrcDir(key);

    if (!res) {
        const py::str err = "src directory "_pys + key + " does not exist"_pys;
        throw py::key_error(err);
    }

    return py::cast(cmPythonDirAttr(*res));
}

bool cmPythonSourceDirs::contains(const py::str& key) 
{
    const auto* res = FindBySrcDir(key);
    return (res != nullptr);
}

std::vector<std::string> cmPythonSourceDirs::GetDefinitions() const
{
    std::vector<std::string> keys;
    std::transform(std::begin(GetIdx()), std::end(GetIdx()), std::back_inserter(keys),
        [](auto const& item) { 
            // return collapse(item->GetCurrentSourceDirectory());
            return (item->GetCurrentSourceDirectory());
        });
    return keys;
}

