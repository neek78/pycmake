/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonGlobalAttr.h"
#include "cmake.h"

namespace py = pybind11;

void cmPythonGlobalAttr::bind(py::module_& m)
{
    auto cls = pybind11::class_<cmPythonGlobalAttr>(m, "GlobalAttr")
        .def_property_readonly("test", &cmPythonGlobalAttr::test)
        ;
}

cmPythonGlobalAttr::cmPythonGlobalAttr(cmake& cm)
:   CMake(cm)
{
}

void cmPythonGlobalAttr::test() const
{
    //CMake.GetState()->GetGlobalProperty(propertyName);
}
#if 0
  cmake* cm = status.GetMakefile().GetCMakeInstance();
  return StoreResult(infoType, status.GetMakefile(), variable,
                     cm->GetState()->GetGlobalProperty(propertyName));
#endif

