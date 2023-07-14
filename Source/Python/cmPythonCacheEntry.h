/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include "cmPythonModBase.h"
#include "cmStateTypes.h"
#include "cmValue.h"

#include <pybind11/embed.h>

#include <string>

class cmMakefile;
class cmValue;

class cmPythonCacheEntry :
    public cmPythonModBase
{
public:
    static void bind(pybind11::module_& m);

    cmPythonCacheEntry(cmMakefile& makefile, std::string entryName); 

    // this method is static to allow it to be unit tested without building objects 
    static pybind11::object convertToPython(const cmValue& val, 
            cmStateEnums::CacheEntryType type, const std::string& name);

    pybind11::object value() const;

protected:
    const std::string& name() const;
    pybind11::dict properties() const;
    cmStateEnums::CacheEntryType cachetype() const;
    pybind11::str repr() const;
    bool GetAdvanced() const;
    void SetAdvanced(bool advanced);

    void ThrowKeyError() const;
    void CheckStillAlive() const;

    const std::string EntryName;
};


