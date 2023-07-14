#!/usr/bin/env python3

from cmake import utils

import unittest

class TestCompactToList(unittest.TestCase):
    def test_test1(self):
        res = utils.compact_to_list(["src1", "src2", "src3.cpp", "src4.h"])
        expect = ["src1", "src2", "src3.cpp", "src4.h"]
        self.assertEqual(expect, res)

        # mix of loose params and a list
        #self.d.add_executable("foo", "src1", "src2", ["src3.cpp", "src4.h"])
        #self.expect("foo", "src1", "src2", "src3.cpp", "src4.h")

        #... tuple
        #self.d.add_executable("foo", "src1", ("src2", "src3.cpp", "src4.h"))
        #self.expect("foo", "src1", "src2", "src3.cpp", "src4.h")

        # set
        # multi-depth

if __name__ == "__main__":
    unittest.main()
