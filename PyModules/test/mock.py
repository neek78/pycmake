#!/usr/bin/env python3

import cmake
import unittest
import unittest.mock

# just identify that there is a return param - no futher detail at the moment.
# FIXME: maybe replace this with a unittest.mock.XXX ?
class MockReturnParam():
    def __eq__(self, other):
        """return true, if the other is a real return param"""
        return type(other) == cmake.ReturnParam

class Common(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(Common, self).__init__(*args, **kwargs)
        self.factory = unittest.mock.MagicMock()

class CommonDir(Common):
    def __init__(self, *args, **kwargs):
        super(CommonDir, self).__init__(*args, **kwargs)

        # object under test
        self.d = cmake.CMakeDir()
        self.d._inject_factory(self.factory)

class CommonGlobal(Common):
    def __init__(self, *args, **kwargs):
        super(CommonGlobal, self).__init__(*args, **kwargs)

        # object under test
        self.g = cmake.CMakeGlobal()
        self.g._inject_factory(self.factory)
