# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.  */

"""
The CMakeGlobal and its ilk provide access to global-scope entities within CMake.
"""

from . import utils
from . import enums 
from . import shared 

import enum

# uncomment when needed....
#class CMakeGlobalLegacy():
#    "Global scope entities that are considered legacy interfaces. Their use is discouraged \
#    in new code"
#    def __init__(self, raw):
#        self._raw = raw

class CMakeGlobal():
    def _inject_factory(self, factory):
        self._factory = factory
        self.raw = self._factory.raw
        self.cache = self._factory.cache
        self.global_attr = self._factory.global_attr
        self.cmake_version = self._factory.cmake_version
        self.source_dirs = self._factory.source_dirs
        self.root_source_dir = self._factory.root_source_dir
        self.convert_to_relative_source_path = self._factory.convert_to_relative_source_path
        # self.legacy = CMakeGlobalLegacy(self.raw)

    #cm.raw.cmake_minimum_required("VERSION", "3.26")
    def cmake_minimum_required(self, version, policy_max=None):
        version_str = shared.build_version_str(version, policy_max)
        args = ["VERSION", version_str]
        self.raw.cmake_minimum_required(*args)

    def message(self, *args, level : enums.MsgLevel = None):
        out = []
        if level is not None:
            out.append(level.name)
            
        out += utils.compact_to_list(args)
        self.raw.message(*out)

    def option(self, variable, help_text, value=None):
        args = [variable, help_text]
        if value is not None:
            args.append(value)
        self.raw.option(*args)

