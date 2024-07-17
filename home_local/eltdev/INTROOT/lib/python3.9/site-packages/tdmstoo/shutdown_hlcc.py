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


    def stop_job(self, nomad_client, job_id):
        """
        Stop a nomad job
        """
        if nomad_utils.is_running(nomad_client, job_id):
            reply = nomad_client.job.deregister_job(job_id, purge=True)
            time.sleep(1)

    @staticmethod
    def create_stop_sw(tpl=None, **kw):
        if not tpl:
            tpl = Tpl()
        my_nomad = tpl.nomad

        ccsif = Sequence.create(
            ait(partial(tpl.stop_job, nomad_client=my_nomad, job_id="hlcc"),
                    name="stop CCS TELIF simulator"),
               name="Stop CCS TELIF Simulators")


        return Sequence.create(ccsif,  name="HLCC Stop")


    @staticmethod
    def create(**kw):
        p = Tpl()

        return Sequence.create(
            Tpl.create_stop_sw(p),
            name = "Shutdown Sequence"
        )
