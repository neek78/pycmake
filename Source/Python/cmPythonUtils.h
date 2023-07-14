/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#pragma once

#include <pybind11/embed.h>

#include <string>
#include <exception>

class cmMessenger;

// overload this so we might have the option to extract python specifics from the former
void OutputExceptionAndSetFatal(cmMessenger& messenger, const std::string_view& extra, 
    const pybind11::error_already_set& ex);

void OutputExceptionAndSetFatal(cmMessenger& messenger, const std::string_view& extra, 
    const std::exception& ex);

// sigh, cmake also has a _s operator"" in the global namespace, so it crashes into 
// pybind11's _s.. make our own
inline namespace literals {
inline pybind11::str operator"" _pys(const char *s, size_t size) { return {s, size}; }
} // namespace 
  //
template<typename T>
T SinglePropExtract(const pybind11::handle& entity, const char* attrName)
{
    auto ret = entity.attr(attrName);

    // ... for some reason, these sometimes come back as a tuple of size 1
    if(pybind11::isinstance<pybind11::tuple>(ret)) {
        pybind11::tuple t = ret;
        assert(t.size() == 1);
        return t[0].cast<T>();
    }

    return ret.cast<T>();
}

