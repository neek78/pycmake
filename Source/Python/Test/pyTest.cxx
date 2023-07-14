
#include <gtest/gtest.h>
#include <pybind11/embed.h>

namespace py = pybind11;

// Please don't add any more tests to this file - 
//
// there needs to be exactly one of these objects live whenever using python/pybind11
// originally, this was a local in each unit test (it's fine to create/destroy them)
// However, if an unexpected exception occured that wasn't caught by EXPECT_* or similar.
// it would fly out to the test infrastructure. By then, the guard would be destroyed, and
// all kinds of undefined behaviour happens when accessing the exception-
// including when destroying it; this requires the guard to be live. So we'll just make sure
// we have this one here, only.  It's in this file because it's unlikely to be commented out
// in CMakeLists.txt
py::scoped_interpreter guard{};

// a simple test to make sure that we can access the python world, and we dont get the 
// whole-archive problem
TEST(PythonImport, PythonTests)
{
    py::module mod = py::module::import("sys");
    EXPECT_FALSE(mod.is_none());
}
