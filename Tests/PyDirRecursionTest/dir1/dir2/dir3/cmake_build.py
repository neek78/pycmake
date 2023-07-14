
import cmake

def cmake_main(*, cm_dir, cm_global):
    cm_dir.add_executable("test_target", "test.cpp")

    cm_dir.var.set_parent_scope("BUBBLE_UP", "suds");
    cm_dir.var.set_parent_scope("BUBBLE_UP2", "suds2");

