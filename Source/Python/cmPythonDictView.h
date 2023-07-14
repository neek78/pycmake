/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonViewIter.h"

#include <pybind11/embed.h>

#include <string>

class cmValue;
class cmPythonDictInterface;

class cmPythonDictViewCommon :
    public cmPythonViewIterSrc
{
public:
    static void bind(pybind11::module_& m);

    cmPythonDictViewCommon(cmPythonDictInterface& parent, const char* name);

protected:
    pybind11::str repr();
    cmPythonViewIter iter();

    // dict interface
    bool contains(const std::string& key);

    pybind11::object get(unsigned int idx) override;
    virtual pybind11::object get(const std::string& key) = 0;

    unsigned int size() const override;

    cmPythonDictInterface& Parent;
    const char* Name;
};

class cmPythonDictViewKeys :
    public cmPythonDictViewCommon 
{
public:
    cmPythonDictViewKeys(cmPythonDictInterface& parent);

protected:
    pybind11::object get(const std::string& key) override;
};

class cmPythonDictViewValues :
    public cmPythonDictViewCommon 
{
public:
    cmPythonDictViewValues(cmPythonDictInterface& parent);

protected:
    pybind11::object get(const std::string& key) override;
};

class cmPythonDictViewItems :
    public cmPythonDictViewCommon 
{
public:
    cmPythonDictViewItems(cmPythonDictInterface& parent);

protected:
    pybind11::object get(const std::string& key) override;
};

