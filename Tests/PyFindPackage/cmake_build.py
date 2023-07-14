
import cmake
import cmake_integration_test

class TestFindPacakge(cmake_integration_test.TestCaseBase):
    def test_find_package(self):
        # we don't really do anything with these yet, just check they don't explode
        self.cm_dir.find_package("ZLIB", quiet=True)

        # Look for a package that has a find module and may be found.
        self.cm_dir.find_package("OpenGL", quiet=True)

        # Look for a package that has an advanced find module.
        self.cm_dir.find_package("Boost", quiet=True)


    # FIXME: it'd be nice to test this, but it doesn't look like we can trigger a fail
    # and catch the exception.. well we can catch the exception, but because
    # fatal error's been triggered, the whole test instance fails...
    #def test_find_package_fail(self):
    #    # Look for a package that has no find module and will not be found.
    #    self.assertRaises(RuntimeError, 
    #        self.cm_dir.find_package, "NotAPackage", quiet=True, required=True)

def global_setup(*, cm_dir, cm_global):
    cm_global.cmake_minimum_required(cmake.CMakeVersion(major = 3, minor = 26))
    cm_dir.project("findTest", version="1.2.3")

    # based on Tests/FindPackageTest
    cm_dir.cmake_policy_set("CMP0017", mode=cmake.PolicyMode.NEW)

def cmake_main(*, cm_dir, cm_global):
    global_setup(cm_dir=cm_dir, cm_global=cm_global)
    cmake_integration_test.main(module=__name__, cm_dir=cm_dir, cm_global=cm_global)
