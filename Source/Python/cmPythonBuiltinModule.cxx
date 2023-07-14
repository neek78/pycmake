/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */

#include "cmPythonCache.h"
#include "cmPythonCacheEntry.h"
#include "cmPythonCombinedVar.h"
#include "cmPythonDirAttr.h"
#include "cmPythonEnums.h"
#include "cmPythonFactory.h"
#include "cmPythonFileSet.h"
#include "cmPythonFunctionLauncher.h"
#include "cmPythonGlobalAttr.h"
#include "cmPythonPyFunctions.h"
#include "cmPythonRaw.h"
#include "cmPythonSourceDirs.h"
#include "cmPythonSourceFile.h"
#include "cmPythonTarget.h"
#include "cmPythonTypeConversion.h"
#include "cmPythonVar.h"
#include "cmPythonDictView.h"
#include "cmPythonDictInterface.h"
#include "cmPythonViewIter.h"

#include <iostream>

#include <pybind11/embed.h>

namespace py = pybind11;

void BindBuiltinModule(py::module_ m)
{
    BindEnums(m);

    cmPythonDictViewCommon::bind(m);
    cmPythonDictInterface::bind(m);

    cmPythonRaw::bind(m);
    cmPythonViewIter::bind(m);
    cmPythonVar::bind(m);
    cmPythonCache::bind(m);
    cmPythonCacheEntry::bind(m);
    cmPythonCombinedVar::bind(m);
    cmPythonTarget::bind(m);
    cmPythonFileSet::bind(m);
    cmPythonDirAttr::bind(m);
    cmPythonSourceDirs::bind(m);
    cmPythonFunctionLauncher::bind(m);
    cmPythonPyFunctions::bind(m);
    cmPythonFactory::bind(m);
    cmPythonGlobalAttr::bind(m);
    cmPythonSourceFile::bind(m);

    BindTypeConversion(m);
}
