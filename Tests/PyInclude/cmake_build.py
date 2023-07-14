
import cmake
import cmake_integration_test

class TestIncludeCMakeCodeInPython(cmake_integration_test.TestCaseBase):
    def test_include(self):
        self.cm_global.cmake_minimum_required(cmake.CMakeVersion(3, 26))
                                    
        loc = self.cm_dir.include("some_module.cmake")
        relative_loc = self.cm_global.convert_to_relative_source_path (loc)

        self.assertEqual(str(relative_loc), "some_module.cmake")

        self.cm_dir.project("fnTest1")

        self.assertVarEqual("YEP_INCLUDED", "FROGS")

        r = self.cm_dir.functions.some_function("goanna", cmake.ReturnParam())
        self.assertEqual(r, "snake")

def cmake_main(*, cm_dir, cm_global):
    cmake_integration_test.main(module=__name__, cm_dir=cm_dir, cm_global=cm_global)
