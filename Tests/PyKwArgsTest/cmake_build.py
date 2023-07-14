
import cmake
import cmake_integration_test

class TestKwArgsToCMakeFunction(cmake_integration_test.TestCaseBase):
    def test_kwargs(self):

        # call cmake functions that use cmake_parse_arguments() and 
        # thus Cmake's version of kwargs. We automatically convert python
        # kwargs the best we can.

        # these functions report their own test failures, so just call 
        # them here...

        # test all 3 kinds of keyword args
        self.cm_dir.functions.kw_func1(
                B1=cmake.KwArg(), B2=cmake.KwArg(True), B3=cmake.KwArg(False), 
                OV1="Cats",
                MV1=[1,2,3], MV2=["Dogs", True, 7])

        # test a mix of positional and kw args
        self.cm_dir.functions.kw_and_pos_func1(
                "lions", "tigers", B1=cmake.KwArg(), B2=cmake.KwArg(True))

    def test_reserved_name(self):
        self.assertRaises(ValueError, self.cm_dir.functions.kw_func1, 
                          B1=True, B2=False, cm_dir="foo")

    def test_reserved_name2(self):
        self.assertRaises(ValueError, self.cm_dir.functions.kw_func1, 
                          B1=True, B2=False, cm_global="bar")

def global_setup(*, cm_dir, cm_global):
        cm_global.cmake_minimum_required(cmake.CMakeVersion(3, 26))
        cm_dir.project("PyKwArgsTest")
        cm_dir.include("stuff.cmake")

def cmake_main(*, cm_dir, cm_global):
    global_setup(cm_dir=cm_dir, cm_global=cm_global)
    cmake_integration_test.main(module=__name__, cm_dir=cm_dir, cm_global=cm_global)
