
#include "Python/cmPythonCacheEntry.h"

#include <gtest/gtest.h>

#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include <string>
#include <filesystem>

namespace py = pybind11;

TEST(cacheEntryTypeConversionTest, PythonTests)
{
    // c++20 would be nice
    // using enum cmStateEnums::CacheEntryType;

    EXPECT_TRUE(cmPythonCacheEntry::convertToPython(
                cmValue{""}, cmStateEnums::CacheEntryType::BOOL, "test").is_none());

    EXPECT_TRUE(cmPythonCacheEntry::convertToPython(
                cmValue{"true"}, cmStateEnums::CacheEntryType::BOOL, "test").equal(py::bool_(true)));

    EXPECT_TRUE(cmPythonCacheEntry::convertToPython(
                cmValue{"false"}, cmStateEnums::CacheEntryType::BOOL, "test").equal(py::bool_(false)));

    EXPECT_THROW(cmPythonCacheEntry::convertToPython(
                cmValue{"random text"}, cmStateEnums::CacheEntryType::BOOL, "test"), py::type_error);

    py::object p = cmPythonCacheEntry::convertToPython(
        cmValue{"/path/to/some/file"}, cmStateEnums::CacheEntryType::FILEPATH, "test");

    // make sure we got a path object back
    py::module_ pathLib = py::module_::import("pathlib");
    py::object pathObj = pathLib.attr("Path")();
    py::type pathType = py::type::of(pathObj);

    EXPECT_TRUE(py::type::of(p).is(pathType));

    std::filesystem::path pt = "/path/to/some/file";

    EXPECT_TRUE(p.equal(py::cast(pt)));
}
