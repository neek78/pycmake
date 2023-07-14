
#include "Python/cmPythonCache.h"

#include <gtest/gtest.h>

#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include <string>
#include <filesystem>

namespace py = pybind11;

TEST(cacheDetermineCacheTypeTest, PythonTests)
{
    // check passthrough works - ie type is specified
    EXPECT_EQ(cmPythonCache::DetermineType(cmStateEnums::CacheEntryType::BOOL, py::bool_(true)),
            cmStateEnums::CacheEntryType::BOOL);

    EXPECT_EQ(cmPythonCache::DetermineType(std::nullopt, py::bool_(true)),
            cmStateEnums::CacheEntryType::BOOL);

    EXPECT_EQ(cmPythonCache::DetermineType(std::nullopt, py::bool_(false)),
            cmStateEnums::CacheEntryType::BOOL);

    EXPECT_EQ(cmPythonCache::DetermineType(std::nullopt, py::str("some string")),
            cmStateEnums::CacheEntryType::STRING);

    auto mod = py::module_::import("pathlib");
    auto path = mod.attr("Path")("/path/to/some/thing");
    EXPECT_EQ(cmPythonCache::DetermineType(std::nullopt, path),
            cmStateEnums::CacheEntryType::FILEPATH);
}
