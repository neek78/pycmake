#!/usr/bin/env python3

import cmake
import unittest

from . import mock

#FIXME: implement
class TestCommandLegacyIncludeDirectories(mock.Common):
    pass

class TestCommandMessage(mock.CommonGlobal):
    def test_simple(self):
        self.g.message("important message")
        self.g.raw.message.assert_called_once_with("important message")

    def test_level_status(self):
        self.g.message("other", level = cmake.MsgLevel.STATUS)
        self.g.raw.message.assert_called_once_with("STATUS", "other")

    def test_level_fatal_error(self):
        self.g.message("other", level = cmake.MsgLevel.FATAL_ERROR)
        self.g.raw.message.assert_called_once_with("FATAL_ERROR", "other")

    def test_level_notice(self):
        self.g.message("abc", "def", "ghi", level = cmake.MsgLevel.NOTICE)
        self.g.raw.message.assert_called_once_with("NOTICE", "abc", "def", "ghi") 

    def test_splat(self):
        self.g.message(["abc", "def", "ghi"], level = cmake.MsgLevel.NOTICE)
        self.g.raw.message.assert_called_once_with("NOTICE", "abc", "def", "ghi") 

class TestCommandCMakeMinimumRequired(mock.CommonGlobal):
    def test_cmake_minimum_required_str_version(self):
        self.g.cmake_minimum_required("3.26")
        self.g.raw.cmake_minimum_required.assert_called_once_with("VERSION", "3.26")

    def test_cmake_minimum_required_version_object(self):
        self.g.cmake_minimum_required(cmake.CMakeVersion(5, 30, 5))
        self.g.raw.cmake_minimum_required.assert_called_once_with("VERSION", "5.30.5")

    def test_cmake_minimum_required_policy_max(self):
        vers_min = cmake.CMakeVersion(3, 1)
        vers_max = cmake.CMakeVersion(4, 5, 2)

        self.g.cmake_minimum_required(vers_min, vers_max)

        self.g.raw.cmake_minimum_required.assert_called_once_with("VERSION", "3.1...4.5.2")

class TestCommandOption(mock.CommonGlobal):
    def test_option_simple(self):
        self.g.option("enable_frontabuliser", 
                      "enable if your system doesn't have a built in frontabulation function")

        self.g.raw.option.assert_called_once_with(
            'enable_frontabuliser', 
            "enable if your system doesn't have a built in frontabulation function")

    def test_option_var(self):
        self.g.option("enable_thing", 
                      "enable the thing",
                      True)

        self.g.raw.option.assert_called_once_with('enable_thing', "enable the thing", True)
