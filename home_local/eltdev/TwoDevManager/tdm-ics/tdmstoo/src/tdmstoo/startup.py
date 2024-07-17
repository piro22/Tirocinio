"""
This is the STOO startup
"""

import os
import nomad
import pathlib

def hlcc_available():
    return ("no"[0].upper() == "Y")

import ifw.core.stooUtils.nomad as nomad_utils
from seq.lib.nodes import Action, Sequence
from seq.lib import partial
from seq.lib.nodes import ActionInThread as ait
from seq.lib.log import getUserLogger
import ifw.sup.syssup.clib.setup_buffer as setup_buffer
import ifw.core.stooUtils.consul as consul_utils
import ifw.sup.syssup.clib.syssup_commands as sup
from ifw.core.acli.std_commands import StdCommands
import time
import threading
from tdm.seq.tpl.daq import LoadOlasConfig


logger = getUserLogger()


class Tpl(Sequence):

    def __init__(self, **kw):
        super().__init__(**kw)
        self.nomad = nomad.Nomad()
        self.cons = consul_utils.ConsulClient()

    def stop_job(self, nomad_client, job_id):
        """
        Stop a nomad job
        """
        if nomad_utils.is_running(nomad_client, job_id):
            reply = nomad_client.job.deregister_job(job_id, purge=True)
            time.sleep(1)

    def start_job(self, nomad_client, job_file):
        """
        Start a nomad job_file
        """
        filename = "/home_local/eltdev/TwoDevManager/tdm-resource/nomad" + "/" + job_file
        job_id = nomad_utils.start_job(nomad_client, filename)
        nomad_utils.wait_until_healthy(nomad_client, job_id)
        if not nomad_utils.is_running(nomad_client, job_id):
            raise ValueError(f"Job {job_id} is not properly running")


    def init_sw(self, timeout):
        """
        Send a request to the Supervisor (stdif)
        """
        if hlcc_available():
            telifsim = self.cons.get_uri("telifsim")
            eltpksim = self.cons.get_uri("eltpksim")
            telmon   = self.cons.get_uri("telmon")
            
            
        uri = self.cons.get_uri("syssup-req")
        supif = sup.SysSupCommands(uri, timeout)
        supif.init()
        
        """
		Qui mando NotOperational/Ready l'FCS2
		"""
        uri2 = self.cons.get_uri("fcs2-req")
        supif2 = sup.SysSupCommands(uri2, timeout)
        supif2.init()

        if hlcc_available():
            uri = self.cons.get_uri("telifsim");
            ccsif = StdCommands(uri,timeout,"telifsim")
            ccsif.init()

            uri = self.cons.get_uri("eltpksim");
            ccsif = StdCommands(uri,timeout,"eltpksim")
            ccsif.init()

            uri = self.cons.get_uri("telmon");
            ccsif = StdCommands(uri,timeout,"telmon")
            ccsif.init()


    def enable_sw(self, timeout):
        """
        Send a request to the Supervisor (stdif)
        """
        """
		Qui mando Operational/Idle l'FCS e poi l'FCS2
		"""
        uri = self.cons.get_uri("syssup-req")
        supif = sup.SysSupCommands(uri, timeout)
        supif.enable()
        
        uri2 = self.cons.get_uri("fcs2-req")
        supif2 = sup.SysSupCommands(uri2, timeout)
        supif2.enable()
        
        if hlcc_available():
            uri = self.cons.get_uri("telifsim");
            ccsif = StdCommands(uri,timeout,"telifsim")
            ccsif.enable()

            uri = self.cons.get_uri("eltpksim");
            ccsif = StdCommands(uri,timeout,"eltpksim")
            ccsif.enable()

            uri = self.cons.get_uri("telmon");
            ccsif = StdCommands(uri,timeout,"telmon")
            ccsif.enable()


    def start_viewer(self, process):
        """
        """
        cons = consul_utils.ConsulClient()
        uri = cons.get_uri(process)
        cmd = f"ddtViewer -s \"{uri}/broker/Broker1 CcfTest11\" &"


    @staticmethod
    def create_stop_sw(tpl=None, **kw):
        if not tpl:
            tpl = Tpl()
        my_nomad = tpl.nomad
        systems = Sequence.create(
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="syssup"),
                    name="stop system supervisor"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="fcs"),
                    name="stop FCS"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="fcs2"),
                    name="stop secondary FCS"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="tdmccf"),
                    name="stop CCF"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="dpm"),
                    name="stop DPM"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="ocm"),
                    name="stop OCM"),
                name="Stop Subsystem Jobs")

        devsim = Sequence.create(
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="lamp1sim"),
                    name="stop lamp simulator"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="shutter1sim"),
                    name="stop shutter simulator"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="motor1sim"),
                    name="stop motor simulator"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="iodev1sim"),
                    name="stop iodev simulator"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="iodev2sim"),
                    name="stop iodev2 simulator"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="mos1sim"),
                    name="stop mos simulator"),
                name="Stop Device Simulators")

        subsim = Sequence.create(
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="subsim1"),
                    name="stop subsystem1 simulator"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="subsim2"),
                    name="stop subsystem2 simulator"),
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="subsim3"),
                    name="stop subsystem3 simulator"),
                name="Stop Subsystem Simulators")

        if hlcc_available():
            ccsif = Sequence.create(
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="hlcc"),
                        name="stop CCS TELIF simulator"),
                        name="Stop CCS TELIF Simulators")

        broker = ait(partial(tpl.stop_job, nomad_client=my_nomad,
                 job_id="tdmbroker"), name="stop DDT Broker")

        if hlcc_available():
            return Sequence.create(systems, devsim, subsim, ccsif, broker, name="System Stop")
        else:
            return Sequence.create(systems, devsim, subsim, broker, name="System Stop")


    @staticmethod
    def create_start_sw(tpl=None, **kw):
        if not Tpl:
            tpl = Tpl()
        my_nomad = tpl.nomad

        broker = ait(partial(tpl.start_job, nomad_client=my_nomad,
                job_file="tdmbroker.nomad"), name="start DDT broker")

        devsim = Sequence.create(
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="lamp1sim.nomad"),
                        name="start lamp simulator"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="shutter1sim.nomad"),
                        name="start shutter simulator"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="motor1sim.nomad"),
                        name="start motor simulator"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="iodev1sim.nomad"),
                        name="start iodev simulator"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="iodev2sim.nomad"),
                        name="start iodev2 simulator"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="mos1sim.nomad"),
                        name="start mos simulator"),
                    name="Start Device Simulators")

        subsim = Sequence.create(
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="subsim1.nomad"),
                        name="start subsystem1 simulator"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="subsim2.nomad"),
                        name="start subsystem2 simulator"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="subsim3.nomad"),
                        name="start subsystem3 simulator"),
                        name="Start Subsystem Simulators")

        if hlcc_available():
                ccsif = Sequence.create(
                        ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="hlcc.nomad"),
                                    name="start CCS TELIF simulator"),
                                    name="Start CCS TELIF Simulators")

        systems = Sequence.create(
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="dpm.nomad"),
                            name="start DPM"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="fcs.nomad"),
                            name="start FCS"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="fcs2.nomad"),
                            name="start secondary FCS"),
                ait(partial(tpl.start_job, nomad_client=my_nomad,
                            job_file="tdmccf.nomad"),
                            name="start CCF"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="ocm.nomad"),
                            name="start OCM"),
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="syssup.nomad"),
                            name="start system supervisor"),
                            name="Start Jobs")

        if hlcc_available():
                return Sequence.create(broker, devsim, subsim, ccsif, systems, name="System Start")
        else:
                return Sequence.create(broker, devsim, subsim, systems, name="System Start")


    @staticmethod
    def create_operational(tpl=None, **kw):
        if not Tpl:
            tpl = Tpl()


        init = ait(partial(tpl.init_sw, timeout=15000),
                       name="Move to NotOperational/Ready")
        enable = ait(partial(tpl.enable_sw, timeout=15000),
                        name="Move to Operational/Idle")

        return Sequence.create(init, enable, name="Move to Operational")


    @staticmethod
    def start_panels(tpl=None, **kw):
        if not Tpl:
            tpl = Tpl()

        job = "tdmbroker"
        viewer = ait(partial(tpl.start_viewer, process=job),
                       name="Start DDT viewer")

        return Sequence.create(viewer, name="Start Panels")

    async def create_dirs(self):
        """Create necessary directories"""
        # Create OLAS directory structure under the assumption this is a single host deployment.
        # If template is modified to run on multiple hosts this should be updated or removed.
        olas_path = pathlib.Path("")
        if not olas_path.is_dir():
            olas_path.mkdir(parents=True)
            logger.info("Created OLAS directory at %s", olas_path)
        else:
            logger.info("OLAS directory already exist at %s", olas_path)


    @staticmethod
    def create(**kw):
        p = Tpl()

        return Sequence.create(
            Tpl.create_stop_sw(p),
            Action(p.create_dirs, name="Create directories"),
            Tpl.create_start_sw(p),
            Tpl.create_operational(p),
            name = "System Startup/Shutdown Sequence"
        )
