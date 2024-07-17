"""
tdm_img_cal_dark template
"""
import asyncio
import json
import os
import signal
import subprocess
from typing import Iterable, List, Optional, Union

import elt.pymal as mal
import ifw.core.stooUtils.consul as consul_utils
import ifw.sup.syssup.clib.syssup_commands as sup
from ifw.ccf.clib.ccf_async_commands import CcfAsyncCommands
from seq.lib.log import getUserLogger
from seq.lib.nodes import Parallel, Sequence, get_param
from tdm.seq.tpl.daq import (DaqException, DaqState, DaqSubState,
                             LoadOlasConfig, MetadataSource, OlasReceiver,
                             PrimaryDataSource, SimpleDaq)

logger = getUserLogger()


class Tpl(Sequence):
    async def init(self):
        """Initializes template."""
        self._running = True

        self.mon_proc = None

        self.cons = cons = consul_utils.ConsulClient()

        self.ccf_name = "tdmccf"
        self.ccf_uri = cons.get_uri(f"{self.ccf_name}-req")

        self.fcf_name = "fcs"
        self.fcf_uri = cons.get_uri(f"{self.fcf_name}-req")

        self.ocm_uri = cons.get_uri("ocm-req")
        self.ocm_ps_uri = cons.get_uri("ocm-pub", cii_srv="zpb.ps")

        self.sup_uri = cons.get_uri("syssup-req")

        self.expo_time = 3  # expected integration time is 3s
        self.await_timeout = self.expo_time + 10  # Assume 10s overhead is enough

        # Load OLAS receiver configuration from project configuration
        self.olas_receiver: OlasReceiver = LoadOlasConfig("TwoDevManager")
        self.mal_factory = mal.CiiFactory.getInstance()
        self.data_sources = [
            PrimaryDataSource(
                name=self.ccf_name,
                rr_uri=f"{self.ccf_uri}/RecCmds",
            ),
            MetadataSource(
                name=self.fcf_name,
                rr_uri=f"{self.fcf_uri}/MetaDaq",
            ),
        ]

    async def setup(self):
        """Set up exposure time."""
        #self.expo_time = int(get_param("DET.EXP.TIME"))
        self.expo_time = 1.1
        self.await_timeout = self.expo_time + 10  # Assume 10s overhead is enough
        with sup.SysSupCommands(self.sup_uri, timeout=2000) as supif:
            if not supif.is_operational():  # Instrument should really be Idle
                raise RuntimeError("Instrument is not operational")
            with CcfAsyncCommands(self.ccf_uri, 60000) as ccf_if:
                await ccf_if.setup(f"expo.time={self.expo_time}")

    async def take_exposure(self):
        """Acquire data from CCF"""
        logger.info(f"Taking an image with exposure time: {self.expo_time} ...")

        # Start acquisition (not recording)
        with CcfAsyncCommands(self.ccf_uri, 60000) as ccf_if:
            await ccf_if.start()
            try:
                daq = SimpleDaq(
                    mal=self.mal_factory,
                    uri=self.ocm_uri,
                    sources=self.data_sources,
                    merge_target=self.ccf_name,  # merge into (first) FITS produced by CCF
                    olas=self.olas_receiver,
                )
                await daq.async_start_daq()
                timeout = self.await_timeout
                logger.info(
                    "Awaiting completion of data acquisition '%s' with a timeout of %s",
                    daq.daq_id,
                    timeout,
                )
                reply: AwaitDaqReply = await daq.async_await_daq_state(
                    state=DaqState.StateAcquiring,
                    substate=DaqSubState.Stopped,
                    timeout=timeout,
                )
                did_timeout = reply.getTimeout()
                if did_timeout:
                    # To keep things simple we abort
                    reply = await daq.async_force_abort()
                    raise RuntimeError(
                        "Data acquisition '%s' did not complete within %s seconds timeout. "
                        "DCS integration time too long?",
                        daq.daq_id,
                        timeout,
                    )
            except DaqException as exc:
                logger.error(f"DAQ Command failed: {exc.getMessage()}")
                # Do not raise MAL execptions as the __str__ is not useful.
                raise RuntimeError(exc.getMessage())
            finally:
                await ccf_if.stop()
                if self.mon_proc:
                    logger.debug("Stopping monitoring process")
                    try:
                        os.kill(self.mon_proc.pid, signal.SIGINT)
                    except ProcessLookupError:
                        pass
                self._running = False

    async def monitor(self):
        """Monitor acquisition status."""
        # Start daqOcmCtl with only --status and it will subscribe to all changes
        logger.info(" Start monitoring")
        self.mon_proc = proc = await asyncio.create_subprocess_exec(
            "daqOcmCtl",
            "--pep",
            self.ocm_ps_uri,
            "--status",
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE,
        )

        async def mon(stream):
            while True:
                buf = await stream.read()
                logger.info("OCM monitor: %s", buf.decode("utf-8"))
                if not buf:
                    break

        await asyncio.gather(mon(proc.stdout), mon(proc.stderr))
        await proc.communicate()
        self.mon_proc = None
        if proc.returncode != 0:
            raise RuntimeError(
                "Subscription to status changes failed with code %s", proc.returncode
            )

        logger.info("... done monitoring")

    @staticmethod
    def create(*args, **kw):
        a = Tpl()
        p = Parallel.create(a.take_exposure, a.monitor, name="exposure", **kw)
        s = Sequence.create(a.init, a.setup, p, name="tdm_img_cal_dark", **kw)
        return s