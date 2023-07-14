
import cmake

def cmake_main(*, cm_dir, cm_global):
    cm_global.message("CMAKE VERSION ", str(cm_global.cmake_version))
    cm_global.message("CMAKE_ROOT is ", cm_dir.combined_var['CMAKE_ROOT'])

    cm_global.cmake_minimum_required(cmake.CMakeVersion(3, 26))

    cm_dir.project("simpleCpp")

    cm_dir.add_executable("simpleCpp", "simpleCpp.cpp")

    # should really test that this explodes
    #cm_dir.var['foo']='bar'
    #cm_dir.var.push_to_parent_scope('foo')

