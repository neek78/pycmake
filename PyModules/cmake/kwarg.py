
class KwArg:
    def __init__(self, enabled : bool = True):
        self._enabled = enabled

    @property
    def enabled(self):
        return self._enabled
