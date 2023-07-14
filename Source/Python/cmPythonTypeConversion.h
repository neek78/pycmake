/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmAlgorithms.h"

#include <string>
#include <map>

#include <pybind11/embed.h>

class cmValue;
class cmList;
class cmPropertyMap;

void BindTypeConversion(pybind11::module_& m);

// stuff to convert python types <-> cmake
std::string ConvertArgPythonToCMake(const pybind11::handle& arg);

pybind11::object ConvertArgCMakeToPython(const std::string& s);
pybind11::object ConvertArgCMakeToPython(const cmValue& s);

pybind11::dict ConvertToDict(const cmPropertyMap& m);

pybind11::list ConvertToList(const cmBTStringRange& range);
pybind11::list ConvertToPathList(const cmBTStringRange& range);
pybind11::list ConvertToList(const std::vector<BT<std::string>>& range);
pybind11::list ConvertToList(const cmList& list);

template<typename CollectionType>
pybind11::list ConvertToList(const CollectionType& range)
{
    pybind11::list ret;
    for(const auto& a : range) {
        ret.append(a);
    }
    return ret;
}

template<typename T>
pybind11::dict ConvertToDict(const T& m)
{
    pybind11::dict ret;
    for(const auto& t : m) {
       const std::string& name = t.first;
       ret[pybind11::str(name)] = ConvertArgCMakeToPython(t.second);
    }
    return ret;
}
