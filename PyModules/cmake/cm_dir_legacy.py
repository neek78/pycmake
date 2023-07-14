
from . import utils
from . import enums 

import enum

class CMakeDirLegacy():
    """
    commands in this class are supported to ease porting existing code, but their use
    is discouraged in new code.
    """
    def __init__(self, raw):
        self.raw = raw

    def include_directories(self, dir1, *dirs, system : bool = False, end : enums.End = None):
        p = utils.scan_params()
        flags = utils.build_params(p, default_key_names=False, default_enum_names=False)

        d = [dir1]
        d += utils.compact_to_list(dirs)
        self.raw.include_directories(*d, *flags)

    def add_definitions(self, *defs):
        d = utils.compact_to_list(defs)
        self.raw.add_definitions(*d)

    def remove_definitions(self, *defs):
        d = utils.compact_to_list(defs)
        self.raw.remove_definitions(*d)

