"""
This is the STOO startup
"""

import os
import nomad
import ifw.core.stooUtils.nomad as nomad_utils
from seq.lib.nodes import Sequence
from seq.lib import partial
from seq.lib.nodes import ActionInThread as ait
import ifw.core.stooUtils.consul as consul_utils
from ifw.core.acli.std_commands import StdCommands
import time
import threading


class Tpl(Sequence):
    def __init__(self, **kw):
        super().__init__(**kw)
        self.nomad = nomad.Nomad()
        self.cons = consul_utils.ConsulClient()


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
        telifsim = self.cons.get_uri("telifsim")
        eltpksim = self.cons.get_uri("eltpksim")
        telmon   = self.cons.get_uri("telmon")

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

        uri = self.cons.get_uri("telifsim");
        ccsif = StdCommands(uri,timeout,"telifsim")
        ccsif.enable()

        uri = self.cons.get_uri("eltpksim");
        ccsif = StdCommands(uri,timeout,"eltpksim")
        ccsif.enable()

        uri = self.cons.get_uri("telmon");
        ccsif = StdCommands(uri,timeout,"telmon")
        ccsif.enable()


    @staticmethod
    def create_start_sw(tpl=None, **kw):
        if not Tpl:
            tpl = Tpl()
        my_nomad = tpl.nomad

        ccsif  = Sequence.create(
                ait(partial(tpl.start_job, nomad_client=my_nomad, job_file="hlcc.nomad"),
                        name="start CCS TELIF simulator"),
                    name="Start CCS TELIF Simulators")


        return Sequence.create(ccsif,  name="System Start")

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
    def create(**kw):
        p = Tpl()

        return Sequence.create(
            Tpl.create_start_sw(p),
            Tpl.create_operational(p),
            name = "System Startup Sequence"
        )
