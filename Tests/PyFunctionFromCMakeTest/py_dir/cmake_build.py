
import cmake

class Naughty:
    pass

def bad_annotations_function(msg, other : Naughty):
    print("some_function", msg)

def old_fashioned_return(p1, r1, *, cm_dir, cm_global):
    v = p1 * 2
    cm_dir.var.set_parent_scope(r1, v)

def some_function(msg, *, cm_dir, cm_global):
    cm_global.message("some_function ", msg)

def single_return_value(ret1 : cmake.ReturnType, *, cm_dir):
    return 57

def complicated_mathematics(in1, ret1 : cmake.ReturnType, *, cm_dir):
    return in1 + 1

def two_return_values(ret1 : cmake.ReturnType, ret2 : cmake.ReturnType, *, cm_dir):
    return "cats", "dogs"

def final_destination(ret1 : cmake.ReturnType, *, cm_dir, cm_global):
    return "secret_message"

def register_another_function(*, cm_dir, cm_global):
    cm_dir.py_functions.add(final_destination)

def call_me_back(function_name, ret1 : cmake.ReturnType, *, cm_dir, cm_global):
    #for i in cm_dir.functions.items():
    #    print(i)
    t = cm_dir.functions[function_name]
    #FIXME: shouldn't call directly into _builtin
    assert(t[0] == cmake._builtin.ScriptedCommandType.Function)
    fn = t[1]
    print ("FN", fn)
    return fn(cm_dir.functions, cmake.ReturnParam())

def raise_exception(*, cm_dir, cm_global):
    raise ValueError("something went wrong")

def cmake_main(*, cm_dir, cm_global):
    cm_dir.py_functions.add(some_function)
    cm_dir.py_functions.add("alternative_name", some_function)
    cm_dir.py_functions.add(single_return_value)
    cm_dir.py_functions.add(two_return_values)
    cm_dir.py_functions.add(complicated_mathematics)
    cm_dir.py_functions.add(register_another_function)
    cm_dir.py_functions.add(call_me_back)
    cm_dir.py_functions.add(raise_exception)
    cm_dir.py_functions.add(old_fashioned_return)

    #cm_dir.py_functions.add(bad_annotations_function)


