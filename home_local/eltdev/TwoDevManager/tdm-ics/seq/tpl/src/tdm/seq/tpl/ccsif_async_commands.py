
import asyncio
import sys
import string
import re
import elt.pymal as mal
from ifw.core.acli.daq_async_commands import DaqAsyncCommands
import ifw.core.acli.mal_client as cii
import hlcc.utils


from tdm.seq.tpl import log
#
# Import CCS-INS command interface and
# some structures
#
from ModStdif.Stdif import ExceptionErr as AppExceptionErr
from ModCcsinsif.Ccsinsif.Commands import CommandsAsync
from ModCcsinsif.Ccsinsif import PresetArgs
from ModCcsinsif.Ccsinsif import PresetData
from ModCcsinsif.Ccsinsif import GuideStarData


import astropy.units as u
from astropy.time import Time
from astropy.coordinates import SkyCoord, EarthLocation, AltAz, Angle


class CcsifAsyncCommands(DaqAsyncCommands):
    """ Class managing the interface with a FCF DevMgr"""

    def __init__(self, uri, timeout=60000):
        """
        Class constructor

        Args:
            uri (str): service URI (CII endpoint) in ms.
            timeout (bool, optional): CII communication timeout.

        """
        super().__init__(uri, timeout, "telifsim")
        self._uri_app = '{}/telifsim/Commands'.format(uri)

        """ Create instance of CII client """
        """ Timeout is in milliseconds """
        self._ciiAsync = cii.MalClient(self._uri_app, self._factory,
                                             CommandsAsync, int(timeout) / 1000)

        self._client = self._ciiAsync.get_mal_client()

    def __enter__(self):
        """
        @brief Support for Context manager protocol.
        """
        return self

    def __exit__(self, *args):
        """
        @brief Support for Context manager protocol.
        """
        self._disconnect()
        return False

    async def preset(self, ra, dec):
        """
        Preset the telescope using HLCC helper class
        Args:
            ra (str):   Right Ascension, e.g. 21h08m46.86357s
            dec (str):  Declination, e.g. -88d57m23.3983s

        This method builds the PresetArgs using the HLCC helper class.
        This helper class sets all defaults.
        """

        preset_args_helper = hlcc.utils.PresetArgsHelper()
        ra_rad = Angle(ra).to(u.rad)
        dec_rad = Angle(dec).to(u.rad)
        preset_args_helper.prs_struct.getPreset_data().setRa(ra_rad.to(u.rad).value)
        preset_args_helper.prs_struct.getPreset_data().setDec(dec_rad.to(u.rad).value)

        try:
            reply = await self._client.Preset(preset_args_helper.prs_struct).create_future()
            return reply
        except AppExceptionErr as e:
            msg = (f"Got exception from command Preset: {e.getDesc()}")
            log.error(msg)
            raise RuntimeError(msg)
        except mal.TimeoutException:
            msg = (f"Got a timeout exception from command Preset after {self._timeout}[ms]")
            log.error(msg)
            raise TimeoutError(msg)


    async def is_ready_for_handover(self):
        """
        Check whether the telescope is ready for handover

        """
        status = await self.status()
        if status == "On::Operational::Idle;On::Operational:ReadyForHandover:True":
            return True
        else:
            return False


    async def _get_cii(self):
        """Return the CII interface"""
        return await self._cii

    def _disconnect(self):
        """
        Closes the MAL connection
        """
        super()._disconnect()
        self._ciiAsync.close_connection()


    async def help(self, func=None):
        """
        Provides a text based help of the Command class functions.

        Args:
            func (str, optional): Name of the function from where to get the help

        This method is to be used for the implementation of CLIs.
        Functions are methods of the class.
        If the parameter <func> is not provided then the help provides a list of
        functions. Functions started with '_' are skipped.
        """
        return await self._help(self.__class__, func)