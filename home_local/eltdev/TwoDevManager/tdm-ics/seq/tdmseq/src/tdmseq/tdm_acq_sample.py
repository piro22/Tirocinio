#!/usr/bin/env python
"""
TWODEVMANAGER acquisition template.
"""
import os
import asyncio
import subprocess
from typing import List
from seq.lib.nodes import Sequence, Action, Template, Parallel
from seq.lib.nodes import get_param
from seq.lib import logConfig
from seq.lib.log import getUserLogger

def hlcc_available():
    return ("no"[0].upper() == "Y")

import tdm.fcs.clib.fcs_async_commands as fcs
import ifw.core.stooUtils.consul as consul_utils
import ifw.sup.syssup.clib.syssup_commands as sup
import ifw.fcf.clib.setup_buffer as sbuf
from ifw.ccf.clib.ccf_async_commands import CcfAsyncCommands

if hlcc_available():
    import tdm.seq.tpl.ccsif_async_commands as ccs

logger = getUserLogger()

async def start_viewer(uri: str):
    """Start DDT Viewer."""
    cmd = f"ddtViewer -l \"{uri}/broker/Broker1\" -s \"CcfTest11\" &"
    await asyncio.create_subprocess_shell(cmd, stdin=asyncio.subprocess.DEVNULL,
                                          stdout=asyncio.subprocess.DEVNULL)
async def start_fcfgui(uri: str):
    """Start FCF Engineering Panel"""
    cmd = f"pyfcfgui --uri \"{uri}/\" &"
    await asyncio.create_subprocess_shell(cmd, stdin=asyncio.subprocess.DEVNULL,
                                          stdout=asyncio.subprocess.DEVNULL)

async def start_ccfgui(uri: str):
    """Start CCF Engineering Panel"""
    cmd = f"ccfGui -r \"{uri}/\" -p cii.oldb:///elt/tdm/dcs &"
    await asyncio.create_subprocess_shell(cmd, stdin=asyncio.subprocess.DEVNULL,
                                          stdout=asyncio.subprocess.DEVNULL)

async def start_ccssim():
    """Start CCS IF GUI."""
    if not hlcc_available():
        return
    cmd = f"hlcctelsimui &"
    await asyncio.create_subprocess_shell(cmd, stdin=asyncio.subprocess.DEVNULL,
                                          stdout=asyncio.subprocess.DEVNULL)

class Tpl(Sequence):
    async def init(self):
        """Initializes template."""
        self._running = True

        self.cons = cons = consul_utils.ConsulClient()
        self.ddt_uri = self.cons.get_uri("tdmbroker")

        self.ccf_name = "tdmccf"
        self.ccf_uri = self.cons.get_uri(f"{self.ccf_name}-req")  

        self.fcf_name = "fcs"
        self.fcf_uri = self.cons.get_uri(f"{self.fcf_name}-req")

        if hlcc_available():
            self.ccs_name = "telifsim"
            self.ccs_uri = self.cons.get_uri(f"telifsim")

        self.sup_uri = self.cons.get_uri("syssup-req")

        self.expo_time = 3  # expected integration time is 3s
        self.await_timeout = self.expo_time + 10  # Assume 10s overhead is enough


    async def start_guis(self):
        """Start all GUIs."""
        logger.info("Starting DDT Viewer ...")
        await start_viewer(self.ddt_uri)
        logger.info("Starting FCF Engineering Panel ...")
        await start_fcfgui(self.fcf_uri)
        logger.info("Starting CCF Engineering Panel ...")
        await start_ccfgui(self.ccf_uri)
        if hlcc_available():
            logger.info("Starting Telescope Simulator GUI ...")
            await start_ccssim()


    async def setup_telescope(self):
        """Preset the telescope."""
        if not hlcc_available():
            return
        with sup.SysSupCommands(self.sup_uri, timeout=2000) as supif:
            if not supif.is_operational():  # Instrument should really be Idle
                raise RuntimeError("Telescope is not operational")

        alpha = get_param("TEL.TARG.ALPHA")
        delta = get_param("TEL.TARG.DELTA")
        with ccs.CcsifAsyncCommands(self.ccs_uri, timeout=60000) as ccsif:
            await ccsif.preset(alpha,delta)

    async def setup_subsystems(self):
        """Setup instrument subsystems."""

        with sup.SysSupCommands(self.sup_uri, timeout=2000) as supif:
            if not supif.is_operational():  # Instrument should really be Idle
                raise RuntimeError("Instrument is not operational")

        """Setup CCF"""
        logger.info("Creating CCF client object ...")
        exptime = int(get_param("DET.EXP.TIME"))
        logger.info("Sending setup to CCF ...")
        """Set up exposure time"""
        with CcfAsyncCommands(self.ccf_uri, 60000) as ccf_if:
            await ccf_if.setup(f"expo.time={exptime}")

        with fcs.FcsAsyncCommands(self.fcf_uri, 60000) as fcs_if:
            buffer = sbuf.SetupBuffer(fcs_if._ciiAsync)

            """Handling of template parameters"""
            position = int(get_param("INS.MOT1.POS"))
            intensity = int(get_param("INS.LAMP1.INTPOWER"))
            if get_param("INS.LAMP1.ON").lower() == "true":
                buffer.add_lamp_switch_on_with_intensity("lamp1", intensity)
            else:
                buffer.add_lamp_switch_off("lamp1")

            if get_param("INS.SHUT1.OPEN").lower() == "true":
                buffer.add_shutter_open("shutter1")
            else:
                buffer.add_shutter_close("shutter1")

            buffer.add_motor_move_abs_pos("motor1", position)

            """Setup FCF"""
            logger.info("Setting up FCS ...")
            """Asynchronous setup"""
            await fcs_if._setup(buffer, keep=False)


    async def wait_for_telescope(self):
        """Wait for the telescope to be ready for operations."""
        if not hlcc_available():
            return

        logger.info(" Start checking for telescope")

        attemps = 60
        counter = 0
        with ccs.CcsifAsyncCommands(self.ccs_uri, timeout=60000) as ccsif:
            while True:
                ready = await ccsif.is_ready_for_handover()
                logger.info("telescope is ready: %s", str(ready))
                if ready == True:
                    break
                if (counter >= attemps):
                    raise ValueError(f"Exceeding number checking attemps {attemps}")
                counter += 1
                await asyncio.sleep(1)

            logger.info("... done waiting for telescope")

    @staticmethod
    def create(*args, **kw):
        a = Tpl()
        if hlcc_available():
            p = Parallel.create(a.setup_subsystems, a.setup_telescope, name="Setting up subsystems", **kw)
            s = Sequence.create(a.init,
                                a.start_guis,
                                p,
                                a.wait_for_telescope,
                                name="tdm_img_acq",
                                **kw)
        else:
            p = Parallel.create(a.setup_subsystems, name="Setting up subsystems", **kw)
            s = Sequence.create(a.init,
                                a.start_guis,
                                p,
                                name="tdm_img_acq",
                                **kw)
        return s
