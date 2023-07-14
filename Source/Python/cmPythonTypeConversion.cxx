#include "cmPythonTypeConversion.h"
#include "cmListFileCache.h"
#include "cmValue.h"
#include "cmStringAlgorithms.h"
#include "cmPropertyMap.h"

#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <filesystem>

namespace py = pybind11;

void BindTypeConversion(pybind11::module_& m)
{
    m.def("convert_arg_python_to_cmake", ConvertArgPythonToCMake);
}

template<typename T>
std::string flattenCollection(const T& arg)
{
    // FIXME: this doesn't really elegantly handle lists in lists (etc)
    assert(arg);

    std::stringstream ss;

    bool first = true;
    for (const auto& a : arg) {
        if (!first) {
            ss << ";";
        } 
        // recurse
        ss << ConvertArgPythonToCMake(a);
        first = false;
    }
    return ss.str();
}

std::string ConvertArgPythonToCMake(const py::handle& arg)
{
    // for now, blow up on none. I'm not quite sure what to do here - CMake 
    // kinda has a concept of null - sometimes a blank string is considered null.
    // (see cmValue). Maybe we should just convert none to blank string
    if(arg.is_none()) {
        throw pybind11::type_error("currently you can't pass None to a cmake function");
    }

    // bools will convert perfectly to strings, but -
    // explicitly convert bools to caps... it seems to be common cmake practice
    // to write bools this way, even though cmake is case insensitive in the case 
    // of true/false literals. User code might do STREQUALS instead
    if(py::isinstance<py::bool_>(arg)) {
        return arg.cast<py::bool_>() ? "TRUE" : "FALSE";
    }

    // don't think there's an obvious way to represent a dictionary in cmake
    if(py::isinstance<py::dict>(arg)) {
        throw pybind11::type_error("can't pass dict to a cmake function");
    }

    // handle dict_keys/dict_values
    // there must be a better way to get the types of these objects 
    // note it's tempting to make these static, but that causes issues with 
    // lifetimes/GIL etc
    // also possible to do this, and avoid temp objects.. but the string comparison is 
    // rather heinous - 
    // std::string t = py::str(py::type::of(arg));
    // if(t == "<class 'dict_keys'>" || t == "<class 'dict_values'>") {
    py::dict d;
    py::type DictItemsType = py::type::of(d.attr("items")());
    py::type t = py::type::of(arg);

    // don't think there's an obvious way to represent a dict_items in cmake
    if(t.is(DictItemsType)) {
        throw pybind11::type_error("can't pass dict_items to a cmake function");
    }

    // dict_keys/dict_values are fine
    py::type DictKeysType = py::type::of(d.attr("keys")());
    py::type DictValuesType = py::type::of(d.attr("values")());

    if(t.is(DictKeysType) || t.is(DictValuesType)) {
        py::list l = arg.cast<py::list>();
        return flattenCollection(l);
    }

    // ... something that can be flattened into a sequence?
    //  FIXME: is there a way this can be shrunk with py::sequence
    if(py::isinstance<py::list>(arg)) {
        return flattenCollection(arg.cast<py::list>());
    }

    if(py::isinstance<py::tuple>(arg)) {
        return flattenCollection(arg.cast<py::tuple>());
    }

    if(py::isinstance<py::set>(arg)) {
        return flattenCollection(arg.cast<py::set>());
    }

    // hokay, just try default string conversion
    return py::str(arg);
}

// FIXME: maybe should return handle?
py::object ConvertElem(const cmValue& val)
{
    // originally, used things like val.IsOn(), but these caused problems
    // with converting many things to bool - for example, they consider 1 and 0
    // to be bools. 
    py::str s = py::str(*val);

    // so, first try to convert to int - thus values 1 or 0 will remain ints (rather than 
    // assuming they're bools)
    // FIXME: is this the best way to  do this?
    try {
        return py::int_(s);
    } catch (const py::error_already_set& e) {}

    try {
        return py::float_(s);
    } catch (const py::error_already_set& e) { }

    // now we've grabbed ints/floats, see if we've got a bool. This will pickup stings such as TRUE
    if(val.IsOn()) {
        return py::bool_(true);
    }

    // FIXME: this will also return if it's a NOTFOUND value. Should we explicitly pick these
    // up first and return something else?
    if(val.IsOff()) {
        return py::bool_(false);
    }

    // should probably also check 
    // cmValue::IsInternallyOn() though I'm not quite sure what to do with that case.
    // This is helpfully clarified as -
    // 'This is not the same as On, but this may be considered as "internally switched on"'.
    // just assume it's a string i guess
      
    // just return as string
    return s;
}

pybind11::object ConvertArgCMakeToPython(const cmValue& val)
{
    // how to interpret cmValue is a nightmare and this is almost certainly wrong

    // this also handles the NOTFOUND case
    if(!val.IsSet()) {
        return py::none();
    } 
   
    // see if this can be parsed into a list 
    auto list = cmList{val};
    if(list.size() > 1) {
        py::list out;
        for (const auto& l : list) {
            // ick, temp creation of a value object..
            out.append(ConvertElem(cmValue(l)));
        }
        return out;
    }

    // just return single item
    return ConvertElem(val);
}

// FIXME: maybe should return handle?
pybind11::object ConvertArgCMakeToPython(const std::string& s)
{
    return ConvertArgCMakeToPython(cmValue(s));
}

pybind11::list ConvertToPathList(const cmBTStringRange& range)
{
    py::list ret;
    for(const auto& e : range) {
        ret.append(std::filesystem::path(e.Value));
    }
    return ret;
}

// FIXME: could probably use std::transform and a lambda or similer in these functions
pybind11::list ConvertToList(const cmBTStringRange& range)
{
    py::list ret;
    for(const auto& e : range) {
        ret.append(e.Value);
    }
    return ret;
}

// FIXME: this actually different from the impl above?
pybind11::list ConvertToList(const std::vector<BT<std::string>>& range)
{
    py::list ret;
    for(const auto& a : range) {
        ret.append(a.Value);
    }
    return ret;
}

py::list ConvertToList(const cmList& list)
{
    py::list ret;
    for (auto const& e : list) {
        ret.append(e);
    }
    return ret;
}

pybind11::dict ConvertToDict(const cmPropertyMap& m)
{
    return ConvertToDict(m.GetList());
}




