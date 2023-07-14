
import cmake
import cmake_integration_test

class TestCallMacro(cmake_integration_test.TestCaseBase):
    def test_call_macro(self):
        self.cm_global.cmake_minimum_required(cmake.CMakeVersion(3, 26))
        self.cm_dir.project("fnTest1")
                                    
        self.cm_dir.add_subdirectory("subdir1")

        r1, r2 = self.cm_dir.functions.macro2x2(
                "elephant", "rhino",  cmake.ReturnParam(), cmake.ReturnParam())

        self.assertEqual(r1, "foo")
        self.assertEqual(r2, "bar")

def cmake_main(*, cm_dir, cm_global):
    cmake_integration_test.main(module=__name__, cm_dir=cm_dir, cm_global=cm_global)
