# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.  */

class ReturnParam:
    def __init__(self,
            name = None,
            default_value = None, 
            auto_cleanup = True, 
            return_value = True,
            raise_on_overwrite = True,
            raise_on_unset = True):
        if name is not None:
            self._name = name 
        else:
            self._name = self._gen_name()
        self._default_value = default_value
        self._auto_cleanup = auto_cleanup
        self._return_value = return_value
        self._raise_on_overwrite = raise_on_overwrite
        self._raise_on_unset = raise_on_unset
        self._arg_pos_ = None

    @property
    def name(self):
        """variable name exposed to cmake"""
        return self._name

    @property
    def default_value(self):
        """value returned if function does not set something"""
        return self._default_value

    @property
    def auto_cleanup(self):
        """remove this value from global vars after returning it"""
        return self._auto_cleanup

    @property
    def return_value(self):
        """put this value in the return values from the function"""
        return self._return_value

    @property
    def raise_on_overwrite(self):
        """raise an exception if the result variable is already defined"""
        return self._raise_on_overwrite

    @property
    def raise_on_unset(self):
        """raise an exception if the cmake function does not set a value"""
        return self._raise_on_unset

    @property
    def _arg_pos(self):
        """position in arg list - internally used by cmake"""
        return self._arg_pos_

    @_arg_pos.setter
    def _arg_pos(self, value):
        self._arg_pos_ = value

    # throw if a function did not set a value, AND no default value is set
    @property
    def should_raise_on_unset(self):
        return self._raise_on_unset and self._default_value is None

    # class var giving next seq num in name generation
    _seq = 0

    @classmethod
    def _next_seq(cls):
        "get next global cmake var number"
        s = cls._seq
        cls._seq += 1
        return s

    def _gen_name(self):
        "create an auto-generated variable name"
        return "PYCMAKE_AUTO_RET_" + str(self._next_seq())
