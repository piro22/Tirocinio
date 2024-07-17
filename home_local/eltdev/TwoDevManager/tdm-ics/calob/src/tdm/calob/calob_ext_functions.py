"""
@ingroup tdm_calob
@copyright ESO - European Southern Observatory
"""

import os

from ifw.calob.calibration_plan import *

class CalibrationExternalFunctions(InstrumentCalibrationExternalFunctions):

    def __init__(self, cc):
        super().__init__(cc)

    def tdm_camera(self, h):
        camera = h['DET.CHIP1.NAME'].upper().strip()
        if camera.startswith("EEV"):
            camera = 'B'
        elif camera.startswith("MIT"):
            camera = 'R'
        else:
            camera = 'U'

        return camera

    def tdm_mos(self, h, clb):
        txt = ""
        mosmax = int(clb.tbl['USERVAR']['MOS_MAX_SLITS'])
        for i in range(1, mosmax + 1):
            key = f"INS.MOS{i}"
            pos = float(h[key + ".POS"])
            wid = float(h[key + ".WIDTH"])
            txt += f"{key}.POS {pos:.3f} {key}.WIDTH {wid:.3f} "

        return txt