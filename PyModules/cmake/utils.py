# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.  */

"""
Important docs...
"""

import inspect
import enum 

def is_string(v):
    return type(v) is str

# https://stackoverflow.com/questions/2937114/python-check-if-an-object-is-a-sequence
def is_sequence_but_not_string(obj):
    if is_string(obj):
        return False
    try:
        len(obj)
        obj[0:0]
        return True
    except TypeError:
        return False

def try_remove_leading_underscore(s):
    if type(s) is not str:
        return s
    if s[0] == '_':
        return s[1:]
    return s

# https://aivarsk.com/2021/07/16/inspecting-python-functions/
def scan_params():
    frame = inspect.currentframe().f_back
    code = frame.f_code
    loc = frame.f_locals
    fin = code.co_argcount + code.co_kwonlyargcount
    keys = code.co_varnames[slice(code.co_argcount, fin, 1)]
    out = {}
    for k in keys:
        out[k] = loc[k]
    return out

def conv_seq(key, value, add_list_names):
    out = []
    if add_list_names and key is not None:
        out.append(key.upper())

    for i in value:
        o = conv_param(None, i, False, {}, False, {}, False, {})
        out += o

    return out

def add_key(key, default, exclusions):
    excluded = key in exclusions
    if excluded:
        return not default 
    else:
        return default

def is_renderable(item):
    return hasattr(item, 'render')

def conv_param(key, value, 
                 default_key_names, key_name_exclusions,
                 default_enum_names, enum_name_exclusions,
                 default_list_names, list_name_exclusions):

    if value is None:
        return None

    if type(value) == bool:
        if value:
            return [key.upper()]
        else:
            return None

    if is_renderable(value):
        return value.render()

    # FIXME should recurse and handle this case
    if type(value) in [dict]:
        raise TypeError("can't handle dict as parameter (name = " + key + ")")

    if is_sequence_but_not_string(value):
        return conv_seq(key, value, add_key(key, default_list_names, list_name_exclusions))

    if isinstance(value, enum.Enum):
        if add_key(key, default_enum_names, enum_name_exclusions):
            return [key.upper(), value.name.upper()]
        else:
            return [value.name.upper()]

    if type(value) ==  str:
        if add_key(key, default_key_names, key_name_exclusions):
            return [key.upper(), value]
        else:
            return [value]

    return None

def build_one_param(key, value, 
                 default_key_names, key_name_exclusions, 
                 default_enum_names, enum_name_exclusions,
                 default_list_names, list_name_exclusions):

    r = conv_param(key, value, 
            default_key_names, key_name_exclusions,
            default_enum_names, enum_name_exclusions, 
            default_list_names, list_name_exclusions)

    if r is None:
        return None

    out = []
    for v in r:
        u = try_remove_leading_underscore(v)
        out.append(u)
    return out

def build_params(params, 
                 default_key_names=True, key_name_exclusions = {}, 
                 default_enum_names=True, enum_name_exclusions = {},
                 default_list_names=True, list_name_exclusions = {},
                 push_to_back = {}):
    out = []
    deferred = []

    for (k,v) in params.items():
        if k in push_to_back:
            deferred.append((k,v))
            continue

        o = build_one_param(k,v, 
            default_key_names, key_name_exclusions,
            default_enum_names, enum_name_exclusions, 
            default_list_names, list_name_exclusions)

        if o is not None:
            out += o

    for (k,v) in deferred:
        o = build_one_param(k,v, 
            default_key_names, key_name_exclusions,
            default_enum_names, enum_name_exclusions, 
            default_list_names, list_name_exclusions)

        if o is not None:
            out += o
    return out

def make_iterable(thing):
    if thing is None:
        return None
    if not is_sequence_but_not_string(thing):
        return [thing]
    return thing

def compact_enums_to_list(v):
    if is_string(v):
        return [v]

    if(isinstance(v, enum.Enum)):
        return [v.name]

    if is_sequence_but_not_string(v) or isinstance(v, set):
        res=[]
        for e in v:
            res.append(e.name)
        return res

    raise TypeError("Don't know how to compact this type")

def compact_to_list(v):
    out = []
    for elem in v:
        if is_string(elem):
            out.append(elem)
            continue

        if is_sequence_but_not_string(elem):
            out += compact_to_list(elem)
            continue

        raise TypeError("Don't know how to compact this type - ", type(elem))
    return out

def dict_key_diff(before, after):
    "return new dict showing items added in after relative to before"
    diff = set(after) - set(before)
    return {k: after [k] for k in after.keys() & diff}

def exec_and_return_entity_diff(cm_dir, function, args):
    "exec function, and return a smörgåsbord of cmake entities that have been added during exec"
    var_before = dict(cm_dir.var)
    build_system_targets_before = dict(cm_dir.dir_attr.build_system_targets)
    imported_targets_before = dict(cm_dir.dir_attr.imported_targets)
    alias_targets_before = dict(cm_dir.dir_attr.alias_targets)

    function(*args)

    var_after = dict(cm_dir.var)
    build_system_targets_after = dict(cm_dir.dir_attr.build_system_targets)
    imported_targets_after = dict(cm_dir.dir_attr.imported_targets)
    alias_targets_after = dict(cm_dir.dir_attr.alias_targets)

    result = dict();
    result["var"] = dict_key_diff(var_before, var_after)
    result["build_system_targets"] = dict_key_diff(
            build_system_targets_before, build_system_targets_after)
    result["imported_targets"] = dict_key_diff(imported_targets_before, imported_targets_after)
    result["alias_targets"] = dict_key_diff(alias_targets_before, alias_targets_after)

    return result

def target_handle_args(args_seq):
    "common args handling for target_* commands"
    if args_seq is None:
        raise TypeError("none passed as args_seq")
    args = []
    for arg in args_seq:
        if arg is None:
            raise TypeError("none in args_seq")
        elif isinstance(arg, str):
            args.append(arg)
        elif is_renderable(arg):
            args += arg.render()
        else:
            raise TypeError("unexpected type " + str(type(arg)))
    return args

