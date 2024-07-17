"""
@file
@ingroup devmgr_fcfclib
@copyright ESO - European Southern Observatory
@brief Library of functions for setting up a MOTOR device with CII.
"""

from ModFcfif.Fcfif import SetupElem
from ModFcfif.Fcfif import CustomDevice


class MosSetup:
    """
    MosSetup provides all methods to build
    the payload of device of type mos.
    """

    def __init__(self, fcfmal, device):
        """
        Mos Setup Class
        """
        self._elem = fcfmal.createDataEntity(SetupElem)
        self._container = self._elem.getDevice()
        self._custom = fcfmal.createDataEntity(CustomDevice)

        name = device.replace('"', '')
        self._elem.setId(name)

    def set_dictionary(self, dict):
        """ Set parameters according to a dictionary """
        self._custom.setParameters(str(dict))

    def get_buffer(self):
        """ Get device payload data (union) """
        self._container.setCustom(self._custom)
        return self._elem
