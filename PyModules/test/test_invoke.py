#!/usr/bin/env python3

import cmake
import unittest

def user_fn1(p1, p2, ret1 : cmake.ReturnType, ret2 : cmake.OptionalReturnType):
    pass

class NaughtyType:
    pass

def user_fn2(p1, p2, ret1 : NaughtyType, ret2 : cmake.ReturnType):
    pass

def user_fn_annotated_kwarg(p1, p2, ret1 : cmake.ReturnType, *, cm_dir, other : cmake.ReturnType):
    pass

class TestAnnotionScanner(unittest.TestCase):
    def test_test1(self):
        f = user_fn1
        res = cmake.scan_for_annotations(f)
        expect = [cmake.ParamKind.In, cmake.ParamKind.In, cmake.ParamKind.Return,
                  cmake.ParamKind.OptionalReturn]
        assert(res == expect)

    def test_test2(self):
        f = user_fn2
        # check we catch unexpected type annotation
        self.assertRaises(TypeError, cmake.scan_for_annotations, f)

    def test_test3(self):
        f = user_fn_annotated_kwarg
        # check we catch unexpected type annotation
        self.assertRaises(TypeError, cmake.scan_for_annotations, f)

# check taking both named params
def user_fn3(p1, p2, ret1 : cmake.ReturnType, *, cm_dir, cm_global):
    pass

#check taking no named params
def user_fn4(p1, p2, ret1 : cmake.ReturnType):
    pass

#check taking unexpected named params
def user_fn5(p1, p2, ret1 : cmake.ReturnType, *, foo, cm_dir):
    pass

def user_fn_just_one_param(p1, p2, ret1 : cmake.ReturnType, *, cm_dir):
    pass

class TestType1:
    pass

class TestType2:
    pass

test_dict = {'cm_dir' : TestType1, 'cm_global' : TestType2}
test_dict2 = {'cm_dir' : TestType1}

# cm_dir and cm_global as positional parameters - currently an error
def pos_param1(cm_dir, /):
    pass

def pos_param2(cm_global, /):
    pass

def pos_param3(cm_dir, cm_global, /):
    pass

#unknown named arg that will never be filled
def pos_param4(cm_dir, cm_global, *, other_named_arg=False):
    pass

# default values - a bit misleading, but legal
def pos_param5(*, cm_dir=None, cm_global=None):
    pass

class TestNamedParamsScanner(unittest.TestCase):
    def test_test1(self):
        f = user_fn3
        res = cmake.scan_named_args(f, test_dict)
        self.assertEqual(res, test_dict)

    def test_test2(self):
        f = user_fn4
        res = cmake.scan_named_args(f, test_dict)
        self.assertEqual(res, {})

    def test_test3(self):
        f = user_fn5
        self.assertRaises(KeyError, cmake.scan_named_args, f, test_dict)

    def test_test4(self):
        f = user_fn_just_one_param
        res = cmake.scan_named_args(f, test_dict)
        self.assertEqual(res, test_dict2)

    def test_pos1(self):
        self.assertRaises(KeyError, cmake.scan_named_args, pos_param1, test_dict)

    def test_pos2(self):
        self.assertRaises(KeyError, cmake.scan_named_args, pos_param2, test_dict)

    def test_pos3(self):
        self.assertRaises(KeyError, cmake.scan_named_args, pos_param3, test_dict)

    def test_pos4(self):
        self.assertRaises(KeyError, cmake.scan_named_args, pos_param4, test_dict)

    def test_pos5(self):
        cmake.scan_named_args(pos_param5, test_dict)

def fn0():
    pass

def fn1(*, cm_dir, cm_global):
    assert(cm_dir is not None)
    assert(cm_global is not None)

def fn2(*, cm_dir):
    assert(cm_dir is not None)

def fn3(*, cm_global):
    assert(cm_global is not None)

# typo in argname
def fn4(*, cm_globals):
    pass

class TestInvokeCMakeMain(unittest.TestCase):
    def test_fn0(self):
        cmake.invoke_cmake_main(fn0, test_dict)
        self.assertRaises(KeyError, cmake.invoke_cmake_main, fn0, test_dict2)

    def test_fn1(self):
        cmake.invoke_cmake_main(fn1, test_dict)
        self.assertRaises(KeyError, cmake.invoke_cmake_main, fn1, test_dict2)

    def test_fn2(self):
        cmake.invoke_cmake_main(fn2, test_dict)
        self.assertRaises(KeyError, cmake.invoke_cmake_main, fn2, test_dict2)

    def test_fn3(self):
        cmake.invoke_cmake_main(fn3, test_dict)
        self.assertRaises(KeyError, cmake.invoke_cmake_main, fn3, test_dict2)

    def test_fn4(self):
        self.assertRaises(KeyError, cmake.invoke_cmake_main, fn4, test_dict)
        self.assertRaises(KeyError, cmake.invoke_cmake_main, fn4, test_dict2)

if __name__ == "__main__":
    unittest.main()
