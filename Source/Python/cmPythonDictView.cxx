#include "cmPythonDictView.h"
#include "cmState.h"
#include "cmPythonUtils.h"
#include "cmPythonDictInterface.h"

namespace py = pybind11;

void cmPythonDictViewCommon::bind(pybind11::module_& m)
{
    auto cls = py::class_<cmPythonDictViewCommon>(m, "DictViewCommon")
        .def("__repr__", &cmPythonDictViewCommon::repr)
        .def("__iter__", &cmPythonDictViewCommon::iter, py::keep_alive<0, 1>())
        .def("__len__", &cmPythonDictViewCommon::size);

    auto keys = py::class_<cmPythonDictViewKeys, cmPythonDictViewCommon>(m, "dict_view_keys")
        .def("__contains__", &cmPythonDictViewKeys::contains);

    auto values = py::class_<cmPythonDictViewValues, cmPythonDictViewCommon>(m, "dict_view_values");

    auto items = py::class_<cmPythonDictViewItems, cmPythonDictViewCommon>(m, "dict_view_items")
        .def("__contains__", &cmPythonDictViewItems::contains);
}

cmPythonDictViewCommon::cmPythonDictViewCommon(cmPythonDictInterface& parent, const char* name)
:   Parent(parent),
    Name(name)
{
}

cmPythonViewIter cmPythonDictViewCommon::iter()
{
    return cmPythonViewIter(*this);
}

unsigned int cmPythonDictViewCommon::size() const
{
    return Parent.GetDefinitions().size();
}

pybind11::object cmPythonDictViewCommon::get(unsigned int idx)
{
    return get(Parent.GetDefinitions().at(idx));
}

py::str cmPythonDictViewCommon::repr()
{
    py::list out;
    for(const auto& k : Parent.GetDefinitions()){
        out.append(get(k));
    }
    return py::repr(out);
}

bool cmPythonDictViewCommon::contains(const std::string& key)
{
    return Parent.contains(key);
}

// cmPythonVarKeys

cmPythonDictViewKeys::cmPythonDictViewKeys(cmPythonDictInterface& parent)
:   cmPythonDictViewCommon(parent, "dict_view_keys")
{
}


py::object cmPythonDictViewKeys::get(const std::string& key) 
{
    return py::cast(key);
}

// cmPythonVarValues

cmPythonDictViewValues::cmPythonDictViewValues(cmPythonDictInterface& parent)
:   cmPythonDictViewCommon(parent, "dict_view_values")
{
}

py::object cmPythonDictViewValues::get(const std::string& key) 
{
    assert(contains(key));
    return Parent.getitem(key);
}

// cmPythonVarItems
  
cmPythonDictViewItems::cmPythonDictViewItems(cmPythonDictInterface& parent)
:   cmPythonDictViewCommon(parent, "dict_view_items")
{
}

py::object cmPythonDictViewItems::get(const std::string& key) 
{
    assert(contains(key));
    return py::cast(std::make_pair(key, Parent.getitem(key)));
}

