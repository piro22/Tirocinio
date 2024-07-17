import ifw.fcf.clib.spf_buffer as spf


class SpfBuffer(spf.SpfBuffer):
    """ Class manage the buffer of the setup request"""

    def __init__(self):
        super().__init__()
        self._actions["Mos"] = self.mos


    def mos(self, dic):
        """
        """
        return(self.action("mos", dic))


