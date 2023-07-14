#!/usr/bin/env python3

import cmake
import unittest

from . import mock

class TestCommandLegacyIncludeDirectories(mock.CommonDir):
    def test_include_directories(self):
        self.d.legacy.include_directories("some_dir")
        self.d.raw.include_directories.assert_called_once_with("some_dir")

    def test_include_directories_multi(self):
        self.d.legacy.include_directories("some_dir", "dir2", "dir3")
        self.d.raw.include_directories.assert_called_once_with("some_dir", "dir2", "dir3")

    def test_include_directories_multi_list(self):
        self.d.legacy.include_directories("some_dir", ["dir2", "dir3"])
        self.d.raw.include_directories.assert_called_once_with("some_dir", "dir2", "dir3")

    def test_include_directories_end(self):
        self.d.legacy.include_directories("some_dir", end=cmake.End.BEFORE)
        self.d.raw.include_directories.assert_called_once_with("some_dir", "BEFORE")

    def test_include_directories_system(self):
        self.d.legacy.include_directories("some_dir", system=True)
        self.d.raw.include_directories.assert_called_once_with("some_dir", "SYSTEM")

class TestCommandLegacyAddDefinitions(mock.CommonDir):
    def test_add_definitions(self):
        self.d.legacy.add_definitions("-DTHING")
        self.d.raw.add_definitions.assert_called_once_with("-DTHING")

    def test_add_definitions_multi(self):
        self.d.legacy.add_definitions("-DTHING", "-DOTHER_THING")
        self.d.raw.add_definitions.assert_called_once_with("-DTHING", "-DOTHER_THING")

class TestCommandLegacyRemoveDefinitions(mock.CommonDir):
    def test_add_definitions(self):
        self.d.legacy.remove_definitions("-DSTUFF")
        self.d.raw.remove_definitions.assert_called_once_with("-DSTUFF")

    def test_add_definitions_multi(self):
        self.d.legacy.remove_definitions("-DTHINGS", "-DOTHER_THINGS")
        self.d.raw.remove_definitions.assert_called_once_with("-DTHINGS", "-DOTHER_THINGS")

if __name__ == "__main__":
    unittest.main()

