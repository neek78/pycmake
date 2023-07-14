/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <string>

#include <pybind11/embed.h>

// common infra used by both cmPythonCache and cmPythonVars
class cmPythonDictInterface 
{
public:
    static void bind(pybind11::module_& m);

    cmPythonDictInterface(const cmPythonDictInterface&) = delete; 
    cmPythonDictInterface& operator=(const cmPythonDictInterface&) = delete;

    virtual ~cmPythonDictInterface() = default;

    virtual std::vector<std::string> GetDefinitions() const = 0;

    virtual bool contains(const pybind11::str& key) = 0;
    virtual pybind11::object getitem(const pybind11::str& key) = 0;

protected:
    cmPythonDictInterface() = default; 

    std::size_t len() const;
    pybind11::object iter();

    pybind11::object keys();
    pybind11::object values();
    pybind11::object items();
};

