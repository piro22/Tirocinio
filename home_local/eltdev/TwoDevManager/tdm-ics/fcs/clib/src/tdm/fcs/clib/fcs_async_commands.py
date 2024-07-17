"""
@copyright ESO - European Southern Observatory
@brief Template for custom client interface classs.
"""

import ifw.fcf.clib.devmgr_async_commands as dm
import tdm.fcs.clib.setup_buffer as sbuf

import sys
sys.path.insert(0, "..")


class FcsAsyncCommands(dm.DevmgrAsyncCommands):
    """
    """

    def __init__(self, uri, timeout):
        """ Timeout is in milliseconds """
        super().__init__(uri, timeout, sbuf.SetupBuffer)

