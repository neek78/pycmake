import cmake
import cmake_integration_test

class TestCallCMakeFromPython(cmake_integration_test.TestCaseBase):
    @classmethod
    def setUpClass(cls):
        pass

    def test_arg_count(self):
        # function requires 2 params
        self.assertRaises(RuntimeError, self.cm_dir.functions.function_return_input)

    def test_return_input(self):
        r = self.cm_dir.functions.function_return_input("bananas", cmake.ReturnParam())
        self.assertEqual(r, "bananas")

    def test_double_return_test(self):
        self.cm_global.message("test_things")
        r1, r2 = self.cm_dir.functions.function2x2(
                "elephant", "rhino",  cmake.ReturnParam(), cmake.ReturnParam())
        self.assertEqual(r1, "foo")
        self.assertEqual(r2, "bar")

    def test_optional_return(self):
        self.assertRaises(
                ValueError,
                self.cm_dir.functions.function2x1opt,
                "e", "r", cmake.ReturnParam(), cmake.ReturnParam(raise_on_unset=True))

    def test_default_return_value(self):
        r1, r2 = self.cm_dir.functions.function2x1opt(
                "elephant", "rhino", 
                cmake.ReturnParam(), cmake.ReturnParam(default_value="hippo"))

        # r2 should have the default value
        self.assertEqual(r1, "foo")
        self.assertEqual(r2, "hippo")

    def test_default_return_value2(self):
        # test that default values only apply when parameters are not 
        # set by called function
        r1, r2 = self.cm_dir.functions.function2x1opt("e", "r",  
                     cmake.ReturnParam(default_value="first_val"),
                     cmake.ReturnParam(default_value="second_val"))

        # r2 is not set by function - it (only) should have the default value
        self.assertEqual(r1, "foo")
        self.assertEqual(r2, "second_val")

    def test_overwrite_protection(self):
        self.cm_dir.var["SPECIAL_VAL"] = 67

        self.assertRaises(
                ValueError,
                self.cm_dir.functions.function2x1opt,
                "e", "r",  
                cmake.ReturnParam(), 
                cmake.ReturnParam(name="SPECIAL_VAL", raise_on_overwrite=True))

    def test_overwrite_protection_off(self):
        self.cm_dir.var["SPECIAL_VAL4"] = 67

        r1, r2 = self.cm_dir.functions.function2x2(
                "e", "r",  
                cmake.ReturnParam(), 
                cmake.ReturnParam(name="SPECIAL_VAL4", raise_on_overwrite=False))

        self.assertEqual(r1, "foo")
        self.assertEqual(r2, "bar")

    def test_auto_cleanup(self):
        r1, r2 = self.cm_dir.functions.function2x2(
                "e", "r",  
                cmake.ReturnParam(), 
                cmake.ReturnParam(name="SPECIAL_VAL2", auto_cleanup=True))

        self.assertEqual(r1, "foo")
        self.assertEqual(r2, "bar")

        # var should have been deleted after marshalling it into return vars
        self.assertVarNotSet("SPECIAL_VAL2")

    def test_auto_cleanup_disabled(self):
        r1, r2 = self.cm_dir.functions.function2x2(
                "e", "r",  
                cmake.ReturnParam(), 
                cmake.ReturnParam(name="SPECIAL_VAL3", auto_cleanup=False))

        self.assertEqual(r1, "foo")
        self.assertEqual(r2, "bar")

        # var should not have been deleted
        self.assertVarEqual("SPECIAL_VAL3", "bar")

# test -
# out only - single
# out only - multi
# in only - single/multi
# holes in return values

def global_setup(*, cm_dir, cm_global):
    cm_global.cmake_minimum_required(cmake.CMakeVersion(3, 26))
    cm_dir.add_subdirectory("subdir1")
    cm_dir.project("fnTest1")

def cmake_main(*, cm_dir, cm_global):
    global_setup(cm_dir=cm_dir, cm_global=cm_global)
    cmake_integration_test.main(module=__name__, cm_dir=cm_dir, cm_global=cm_global)
