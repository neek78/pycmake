"""Provides Python's built in unittest infrastructure for testing Python 
scripting under CMake"""

import unittest
import cmake
from unittest.util import safe_repr

class TestCaseBase(unittest.TestCase):
    def inject_common_obj(self, *, cm_dir, cm_global):
        self.cm_dir = cm_dir
        self.cm_global= cm_global

    ## VARS
    def assertVarSet(self, varName, msg=None):
        """Check that the var is set in cm_dir.var"""
        self.assertIn(varName, self.cm_dir.var)

    def assertVarNotSet(self, varName, msg=None):
        """Check that the var is NOT set in cm_dir.var"""
        self.assertNotIn(varName, self.cm_dir.var)

    def assertVarEqual(self, varName, value, msg=None):
        """check that the var is set in cm_dir.var"""
        self.assertVarSet(varName, msg)
        self.assertEqual(self.cm_dir.var[varName], value, msg)

    def assertVarTypeEqual(self, varName, t, msg=None):
        """Check that the var is of the cmake type specified"""
        self.assertVarSet(varName, msg)
        self.assertIsInstance(self.cm_dir.var[varName], t, msg)

    ## CACHE
    def assertCacheValSet(self, varName, msg=None):
        """Check that the var is set in cm_global.cache"""
        self.assertIn(varName, self.cm_global.cache)

    def assertCacheValNotSet(self, varName, msg=None):
        """Check that the var is NOT set in cm_global.cache"""
        self.assertNotIn(varName, self.cm_global.cache)

    def assertCacheValEqual(self, varName, value, msg=None):
        """Check that the var is set in cm_global.cache"""
        self.assertCacheValSet(varName, msg)
        self.assertEqual(self.cm_global.cache[varName].value, value, msg)

    def assertCachePythonTypeEqual(self, varName, t, msg=None):
        """Check that the var is of the cmake type specified"""
        self.assertCacheValSet(varName, msg)
        self.assertIsInstance(self.cm_global.cache[varName].value, t, msg)

    def assertCacheCacheTypeEqual(self, varName, t, msg=None):
        """Check that the var is of the cmake type specified"""
        self.assertCacheValSet(varName, msg)
        self.assertEqual(self.cm_global.cache[varName].cache_type, t, msg)

    def assertCacheAdvancedEqual(self, varName, advanced, msg=None):
        """Check that the var is / is not marked as advanced"""
        self.assertCacheValSet(varName, msg)
        self.assertEqual(self.cm_global.cache[varName].advanced, advanced, msg)

    ## Combined
    def assertCombinedVarSet(self, varName, msg=None):
        """Check that the var is set in cm_dir.combined_var"""
        self.assertIn(varName, self.cm_dir.combined_var)

    def assertCombinedVarNotSet(self, varName, msg=None):
        """Check that the var is not set in cm_dir.combined_var"""
        self.assertNotIn(varName, self.cm_dir.combined_var)

    def assertCombinedVarEqual(self, varName, value, msg=None):
        """check that the var is set in cm_dir.combined_var"""
        self.assertCombinedVarSet(varName, msg)
        self.assertEqual(self.cm_dir.combined_var[varName], value, msg)

    def assertCombinedVarTypeEqual(self, varName, t, msg=None):
        """Check that the var is of the cmake type specified"""
        self.assertCombinedVarSet(varName, msg)
        self.assertIsInstance(self.cm_dir.combined_var[varName], t, msg)

class CustomResult(unittest.runner.TextTestResult):
    def __init__(self, *args, cm_dir, cm_global, **kwargs):
        self.cm_dir = cm_dir
        self.cm_global = cm_global
        super(CustomResult, self).__init__(*args, **kwargs)

    def addSuccess(self, test):
        super(CustomResult, self).addSuccess(test)
        self.cm_global.message(str(test), " passed")

    def addFailure(self, test, err):
        super(CustomResult, self).addFailure(test, err)
        self.cm_global.message(str(test), " failed - ", 
                                str(err), level=cmake.MsgLevel.SEND_ERROR)

    def addSubTest(self, test, subtest, err):
        super(CustomResult, self).addSubTest(test, subtest, err)
        print("addSubtest")
        if err is None:
            self.cm_global.message(str(test), " subtest passed")
        else:
            self.cm_global.message(str(test), " SUBTEST ERROR - ", 
                                   str(err), level=cmake.MsgLevel.SEND_ERROR)

    def addError(self, test, err):
        super(CustomResult, self).addError(test, err)
        self.cm_global.message(str(test), " ERROR - ", 
                                str(err), level=cmake.MsgLevel.SEND_ERROR)

    def addSkip(self, test, reason):
        super(CustomResult, self).addSkip(test, reason)
        self.cm_global.message(str(test), " - skipped - ", str(reason))

    def addExpectedFailure(self, test, err):
        super(CustomResult, self).addExpectedFailure(test, err)
        self.cm_global.message(str(test), " EXPECTED FAILURE - ", 
                                str(err), level=cmake.MsgLevel.SEND_ERROR)

    def addUnexpectedSuccess(self, test):
        super(CustomResult, self).addExpectedSuccess(test)
        self.cm_global.message(str(test), " UNEXPECTED SUCCESS - ", 
                                level=cmake.MsgLevel.SEND_ERROR)

class TestRunner(unittest.runner.TextTestRunner):
    "Custom TestRunner which routes test results back to CTest"
    def __init__(self, *args, cm_dir, cm_global, **kwargs):

        rc = lambda stream, descriptions, verbosity, cm_dir=cm_dir, cm_global=cm_global: \
                CustomResult(stream, descriptions, verbosity, cm_dir=cm_dir, cm_global=cm_global)

        super(TestRunner, self).__init__(resultclass = rc, *args, **kwargs)

class TestLoader(unittest.loader.TestLoader):
    "Custom TestLoader which provides cm_dir and cm_global to test suites"
    def __init__(self, *args, cm_dir, cm_global, **kwargs):
        self.cm_dir = cm_dir
        self.cm_global= cm_global
        super(TestLoader, self).__init__(*args, **kwargs)

    def loadTestsFromTestCase(self, testCaseClass):
        loaded_suite = super(TestLoader, self).loadTestsFromTestCase(testCaseClass)
        #  inject our objects into the base type of our tests
        for t in loaded_suite:
            if isinstance(t, TestCaseBase):
                t.inject_common_obj(cm_dir=self.cm_dir, cm_global=self.cm_global)
        return loaded_suite

def main(*args, module, cm_dir, cm_global, **kwargs):
    "Entrypoint, sets up our custom types and calls unittest.main()"
    unittest.main(*args,
            exit=False, 
            module=module, 
            testRunner=TestRunner(cm_dir=cm_dir, cm_global=cm_global),
            testLoader=TestLoader(cm_dir=cm_dir, cm_global=cm_global),
            **kwargs)
