import cmake
import cmake_integration_test

import pathlib

class TestCache(cmake_integration_test.TestCaseBase):
    """tests for the cm_global.cache interface"""

    def check_cached_val(self, name, value, python_type, cache_type, advanced=False):
        # should be visible in the cache
        self.assertCacheValEqual(name, value)
        self.assertCachePythonTypeEqual(name, python_type)
        self.assertCacheCacheTypeEqual(name, cache_type) 
        self.assertCacheAdvancedEqual(name, advanced) 

        # should not be visible in regular vars
        self.assertVarNotSet(name)

        # should be visible in combined view
        self.assertCombinedVarEqual(name, value)
        self.assertCombinedVarTypeEqual(name, python_type)

    def test_cache_values_from_cmake(self):
        "check cache values from cmake are available in the cache(only)"
        self.check_cached_val("CACHE_STRING_VALUE1", "test_value1", 
                              str, cmake._builtin.CacheEntryType.STRING)

        self.check_cached_val("CACHE_BOOL_VALUE_T", True, 
                              bool, cmake._builtin.CacheEntryType.BOOL)

        self.check_cached_val("CACHE_BOOL_VALUE_F", False, 
                              bool, cmake._builtin.CacheEntryType.BOOL)

        self.check_cached_val("CACHE_FILEPATH_VALUE", pathlib.Path('/path/to/some/file'), 
                              pathlib.Path, cmake._builtin.CacheEntryType.FILEPATH)

        self.check_cached_val("CACHE_PATH_VALUE", pathlib.Path('/path/to/dir/somewhere'), 
                              pathlib.Path, cmake._builtin.CacheEntryType.PATH)

    def test_cache_values_from_python_dict_interface(self):
        self.cm_global.cache['SET_FROM_DICT_INTERFACE1'] = True
        self.cm_global.cache['SET_FROM_DICT_INTERFACE2'] = "badger"
        self.cm_global.cache['SET_FROM_DICT_INTERFACE3'] = pathlib.Path("/some/file")

        self.check_cached_val("SET_FROM_DICT_INTERFACE1", True, 
                              bool, cmake._builtin.CacheEntryType.BOOL)

        self.check_cached_val("SET_FROM_DICT_INTERFACE2", "badger", 
                              str, cmake._builtin.CacheEntryType.STRING)

        self.check_cached_val("SET_FROM_DICT_INTERFACE3", pathlib.Path('/some/file'), 
                              pathlib.Path, cmake._builtin.CacheEntryType.FILEPATH)

        del self.cm_global.cache["SET_FROM_DICT_INTERFACE1"]
        del self.cm_global.cache["SET_FROM_DICT_INTERFACE2"]
        del self.cm_global.cache["SET_FROM_DICT_INTERFACE3"]

        self.assertCacheValNotSet("SET_FROM_DICT_INTERFACE1")
        self.assertCacheValNotSet("SET_FROM_DICT_INTERFACE2")
        self.assertCacheValNotSet("SET_FROM_DICT_INTERFACE3")

    def test_cache_values_from_python_set_interface(self):
        self.cm_global.cache.set('SET_FROM_DICT_INTERFACE_10', "koala", doc_string="docs")

        self.check_cached_val("SET_FROM_DICT_INTERFACE_10", "koala", 
                              str, cmake._builtin.CacheEntryType.STRING)

        # we don't allow deleting values with a blank string
        self.assertRaises(ValueError, self.cm_global.cache.set, 'SET_FROM_DICT_INTERFACE_10', "",
                          doc_string="ds")

        # hokay, test unset()
        self.cm_global.cache.unset('SET_FROM_DICT_INTERFACE_10')
        self.assertCacheValNotSet("SET_FROM_DICT_INTERFACE_10")

    def test_cache_values_empty_var_name(self):
        self.assertRaises(TypeError, self.cm_global.cache.set, None, "something", 
                          doc_string='eggs')

        self.assertRaises(ValueError, self.cm_global.cache.set, "", "something",
                          doc_string="bread")

    def test_cache_unset_non_existent_value(self):
        # unset() should fail silently when key doesn't exist
        self.cm_global.cache.unset("DOES_NOT_EXIST")

    def test_cache_unset_non_existent_value(self):
        # __delitem__ should raise when key does not exist
        self.assertRaises(KeyError, self.cm_global.cache.__delitem__, "DOES_NOT_EXIST2")

    def test_cache_set_env_var(self):
        # it makes no sense to try to set an envvar with the cache interface 
        self.assertRaises(ValueError, self.cm_global.cache.set, 
                          "ENV{MYVAR}", "something", doc_string="this cant work")

    def test_cache_set_docstring(self):
        self.cm_global.cache.set('SET_FROM_DICT_INTERFACE_12', "quokka", 
                                  doc_string="little furry guy")
        entry = self.cm_global.cache['SET_FROM_DICT_INTERFACE_12']
        self.assertEqual(entry.properties['HELPSTRING'], "little furry guy") 

    def test_cache_values_advanced(self):
        self.cm_global.cache.set('NOT_ADVANCED', 'beaver', doc_string="docstr")
        self.cm_global.cache['ALSO_NOT_ADVANCED'] = "hamster"
        self.cm_global.cache.set('IS_ADVANCED', "badger", advanced=True, 
                                  doc_string="docstr2")

        self.check_cached_val("NOT_ADVANCED", 'beaver', 
                              str, cmake._builtin.CacheEntryType.STRING, False)

        self.check_cached_val("ALSO_NOT_ADVANCED", 'hamster', 
                              str, cmake._builtin.CacheEntryType.STRING, False)

        self.check_cached_val("IS_ADVANCED", 'badger', 
                              str, cmake._builtin.CacheEntryType.STRING, True)

    def test_cache_values_existing_value_advanced(self):
        # clear out any possible stale values
        self.cm_global.cache.unset('MAYBE_ADVANCED')

        self.cm_global.cache['MAYBE_ADVANCED'] = "spider"
        self.assertCacheAdvancedEqual('MAYBE_ADVANCED', False) 

        ent = self.cm_global.cache['MAYBE_ADVANCED']

        ent.advanced = True
        self.assertCacheAdvancedEqual('MAYBE_ADVANCED', True) 

        ent.advanced = False
        self.assertCacheAdvancedEqual('MAYBE_ADVANCED', False) 

class TestVar(cmake_integration_test.TestCaseBase):
    def check_var(self, name, value, python_type):
        # should be visible in the cache
        self.assertVarEqual(name, value)
        self.assertVarTypeEqual(name, python_type)

        # should not be visible in cache
        self.assertCacheValNotSet(name)

        # should be visible in combined view
        self.assertCombinedVarEqual(name, value)
        self.assertCombinedVarTypeEqual(name, python_type)

    def test_var_values_empty_var_name(self):
        self.assertRaises(TypeError, self.cm_dir.var.set, None, "something")

        self.assertRaises(ValueError, self.cm_dir.var.set, "", "something")

    def test_var_values_empty_var_name(self):
        self.cm_dir.var["VAR1"] = "yep"

        self.assertRaises(ValueError, self.cm_dir.var.set, "VAR1", "")

    def test_var_set_env_var(self):
        self.assertRaises(ValueError, self.cm_dir.var.set, "ENV{MYVAR}", "something")

    def test_var_values_from_python_set_interface(self):
        self.cm_dir.var["VAR2"] = 57
        self.check_var("VAR2", 57, int)

        self.cm_dir.var["VAR3"] = 100.9
        self.check_var("VAR3", 100.9, float)

        self.cm_dir.var["VAR4"] = "some_string"
        self.check_var("VAR4", "some_string", str)

        # test overwriting var
        self.cm_dir.var["VAR4"] = 1255.99
        self.check_var("VAR4", 1255.99, float)

    def test_var_unset_non_existent_value(self):
        # delitem should raise when no value exists, whilst unset() doesnt
        # FIXME: is there a better way to call __delitem__ in this context?
        self.assertRaises(KeyError, self.cm_dir.var.__delitem__, "ALSO_DOES_NOT_EXIST")

    def test_var_unset_parent_from_root_scope(self):
        # unset_parent() should raise when we're in the root scope 
        self.assertRaises(RuntimeError, self.cm_dir.var.unset_parent_scope, "DOES_NOT_EXIST3")

    def test_var_is_root_scope(self):
        self.assertFalse(self.cm_dir.var.has_parent_scope)

    def test_var_push_through_root_scope(self):
        self.cm_dir.var.set("YET_ANOTHER_IMPORTANT_VALUE", 3)

        # () should raise as we're in the root scope 
        self.assertRaises(RuntimeError, self.cm_dir.var.push_to_parent_scope, "YET_ANOTHER_IMPORTANT_VALUE")

    # FIXME: test parent_scope stuff (needs a new scope)
    # FIXME: test reading vars set by CMake code
    # FIXME: test unset against parent scope - esp error case / throw - non  throw

class TestCombined(cmake_integration_test.TestCaseBase):
    def test_cached(self):
        self.cm_global.cache.set('SET_FROM_DICT_INTERFACE_100', 'emu', doc_string="docstr")
        self.assertCombinedVarEqual("SET_FROM_DICT_INTERFACE_100", "emu")

    def test_var(self):
        self.cm_dir.var.set('SET_FROM_DICT_INTERFACE_101', 'ostrich')
        self.assertCombinedVarEqual("SET_FROM_DICT_INTERFACE_101", "ostrich")

    def test_expand_from_var(self):
        self.cm_dir.var.set('SET_FROM_DICT_INTERFACE_102', 'ostrich')
        self.assertEqual(self.cm_dir.combined_var.expand("${SET_FROM_DICT_INTERFACE_102}"), 'ostrich')

    def test_expand_from_cache(self):
        self.cm_global.cache.set('SET_FROM_DICT_INTERFACE_103', 'duck', doc_string="docstr")
        self.assertEqual(self.cm_dir.combined_var.expand("${SET_FROM_DICT_INTERFACE_103}"), 'duck')

    def test_expand_non_existant_var(self):
        # normal version should raise
        self.assertRaises(ValueError, self.cm_dir.combined_var.expand, "${NOT_AN_EXIT}")

    def test_expand_relaxed_non_existant_var(self):
        # relaxed version should quietly expand undefined vars to nothing
        self.assertEqual(self.cm_dir.combined_var.expand_relaxed(
            "${ALSO_DOES_NOT_EXIST_REALLY_THIS_TIME}"), '')

def global_setup(*, cm_dir, cm_global):
    cm_global.cmake_minimum_required(cmake.CMakeVersion(3, 26))
    cm_dir.add_subdirectory("subdir1")
    cm_dir.project("CacheAndVarTest")

def cmake_main(*, cm_dir, cm_global):
    global_setup(cm_dir=cm_dir, cm_global=cm_global)
    cmake_integration_test.main(module=__name__, cm_dir=cm_dir, cm_global=cm_global)
