
class KwArg:
    """
    When calling cmake functions that use cmake_parse_arguments(), passing a
    KwArg will render into just its name for True, and nothing for False.
    """
    def __init__(self, enabled : bool = True):
        self._enabled = enabled

    @property
    def enabled(self):
        return self._enabled
