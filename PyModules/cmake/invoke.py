# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.  */

"""
Important docs...
"""

# need this to access cmake._builtin, but it creates an infinite recursion(?)
from . import _builtin
from . import utils

import enum
import inspect

class ReturnType:
    pass

class OptionalReturnType:
    pass

ParamKind = enum.Enum('ParamKind', ["In", "Return", "OptionalReturn"])

def scan_for_annotations(f):
    params = inspect.signature(f).parameters;

    annotations = []
    for key in params:
        param = params[key]
        annotation = param.annotation

        found = False
        if annotation is ReturnType:
            k= ParamKind.Return
            found = True
        elif annotation == OptionalReturnType:
            k = ParamKind.OptionalReturn
            found = True
        elif annotation == inspect._empty:
            k = ParamKind.In
        else:
             raise TypeError("unexpected annotation of type " + str((annotation)))

        # we can only handle annotations on positional params
        if param.kind == param.KEYWORD_ONLY:
            if found:
                raise TypeError("can't handle annotation on kw parameter")
            else:
                continue
        annotations.append(k)
    return annotations

def get_fn_desc(f):
    "return function name/file/line in a string"
    loc = f.__code__.co_filename + "(" + str(f.__code__.co_firstlineno) + ")"
    return loc + ": " + f.__name__ 

def scan_named_args(f, kwargs):
    "scan a function prototype, checking the named args it expects" \
    "comparing them to the provided dict of args. Returns a dict of args" \
    "which the fn actually wants. Raises if the prototype doesn't match what we want"
    params = inspect.signature(f).parameters;
    out = {}
    for key in params:
        param = params[key]
        if param.kind != param.KEYWORD_ONLY:
            # expected as named param, but is positional?
            # note this will also catch POSITIONAL_OR_KEYWORD - we do this
            # to prevent ambiguity as cmake script args are passed in as positionals
            if param.name in kwargs:
                raise KeyError("user function " + get_fn_desc(f) + " has argument " 
                             + param.name + " that should be keyword-only")
            continue
        if param.name not in kwargs:
            raise KeyError("user function " + get_fn_desc(f) + " has unknown named argument " 
                             + param.name)
        out[param.name] = kwargs[param.name]
    return out

# note this will fail when we're not called from under c++, as _builtin will be empty
def convert_value_python_to_cmake(val):
    return _builtin.convert_arg_python_to_cmake(val)

def scan_return_values(annotations, args, ret_values, cm_dir):
    # FIXME: what happens when calling a *args function?
    assert len(args) == len(annotations), "len mismatch" 

    args2 = list(args)
    args2.reverse()
    annotations.reverse()

    ret_values = utils.make_iterable(ret_values)

    if ret_values is not None:
        return_param_count = 0
        for ret in ret_values:
            # consume remaining annotations, find one for a return value
            while True:
                if len(annotations) == 0:
                    # ran out of annotations before we got through all the return values
                    msg = "python function returned " + str(len(ret)) + \
                            " values, but function signature specifies only " \
                            + str(return_param_count)
                    raise ValueError(msg)

                k = annotations.pop() 
                varName = args2.pop()

                if k == ParamKind.In:
                    continue

                return_param_count += 1

                # got a return value
                if ret is None:
                    if k == ParamKind.Return:
                        # required parameter, but got none..
                        # FIXME: error message could be better
                        raise ValueError("got None for a required return value")

                    # we got none for an optional return value - that's fine, 
                    # do nothing. onto the next return value
                    break

                # convert and set the value. let any exceptions fly out
                ret_converted = convert_value_python_to_cmake(ret)

                # write the value to its variable
                # this is effectively SET(varName value PARENT_SCOPE)
                cm_dir.var.set_parent_scope(varName, ret_converted)

                # onto next return value
                break 

    # we've now walkted through all return values - were there enough?
    for a in annotations:
        if a == ParamKind.Return:
            #FIXME: error message could be better
            raise ValueError("function did not return enough values for signature")

def invoke_py_user_fn(fn, args, kwargs):

    # check nothing stupid's happening in c++ land
    cm_dir = kwargs['cm_dir'] 
    assert(cm_dir is not None)
    assert(kwargs['cm_global'] is not None)
    assert(fn is not None)

    # even though we don't need it until the user function has returned,
    # scan for annotations now to report any errors and fail early
    annotations = scan_for_annotations(fn)

    # work out what params the user function actually wants, and blow up
    # if there are unknown named args in its signature (etc)
    expurgated_kwargs = scan_named_args(fn, kwargs)

    # launch the user function. We've not checked that the number of positional args
    # is correct, but python's more than happy to do that itself
    ret = fn(*args, **expurgated_kwargs)

    # process the return values from the user function
    scan_return_values(annotations, args, ret, cm_dir)

def invoke_cmake_main(fn, kwargs):
    "simplified version of invoke_py_user_fn, used for calling cmake_main()"

    # check nothing stupid's happening in c++ land
    assert(fn is not None)
    assert(kwargs['cm_global'] is not None)
    assert(kwargs['cm_dir'] is not None)

    # work out what args the user function actually wants, and blow up
    # if there are unknown named params in its signature
    expurgated_kwargs = scan_named_args(fn, kwargs)

    # call the user function. note that return values don't really go anywhere...
    # we give them back to c++ so it can issue a warning.
    return fn(**expurgated_kwargs)

