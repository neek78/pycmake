
import cmake

def cmake_main(*, cm_dir):
    cm_dir.add_subdirectory("dir2")

    val = cm_dir.var['BUBBLE_UP']
    cm_dir.var.set_parent_scope("BUBBLE_UP", val)

    cm_dir.var.push_to_parent_scope("BUBBLE_UP2")
