"""
@ingroup tdm_calob
@copyright ESO - European Southern Observatory
"""

import os

from ifw.calob.calibration_plan import *

class CalibrationTables(InstrumentCalibrationTables):

    def __init__(self, cc):
        super().__init__(cc)

    def get_tables(self):

        self.tbl['USERVAR'] = {
            "DET_HGAIN"     : 1.43,
            "DET_LGAIN"     : 0.80,
            "REF_BINX"      : 2.0,
            "REF_BINY"      : 2.0,
            "REF_SLIT"      : 1.0,
            "WIDE_SLIT"     : 5.0,
            "MOS_PARK_POS"  : -118.5,
            "HIT_MAX_SLITS" : 2,
            "MOS_MAX_SLITS" : 19,
            "MXU_MAX_SLITS" : 200,
            "N_SHIFT"       : 41.0
        }

        self.tbl['DET.MODE'] = {
            ("100Kps/2ports/high_gain", 2, 2): "100kHz,2x2,high",
            ("100Kps/2ports/high_gain", 1, 1): "100kHz,1x1,high",
            ("200Kps/2ports/low_gain",  2, 2): "200kHz,2x2,low",
            ("200Kps/2ports/low_gain",  1, 1): "200kHz,1x1,low",
            ("625Kps/2ports/low_gain",  2, 2): "200kHz,2x2,low",
            ("HIT-MS",                  2, 2): "HIT-MS",
            ("HIT-OS1-1sec",            1, 1): "100kHz,2x2,high",
            ("HIT-OS2-4sec",            1, 1): "100kHz,2x2,high",
            ("HIT-OS3-16sec",           1, 1): "100kHz,2x2,high",
            ("HIT-OS4-64sec",           1, 1): "100kHz,2x2,high",
            ("HIT-OS5-256sec",          1, 1): "100kHz,2x2,high",
            ("HIT-OS6-1024sec",         1, 1): "100kHz,2x2,high",
            ("HIT-OS1-1sec",            2, 2): "100kHz,2x2,high",
            ("HIT-OS2-4sec",            2, 2): "100kHz,2x2,high",
            ("HIT-OS3-16sec",           2, 2): "100kHz,2x2,high",
            ("HIT-OS4-64sec",           2, 2): "100kHz,2x2,high",
            ("HIT-OS5-256sec",          2, 2): "100kHz,2x2,high",
            ("HIT-OS6-1024sec",         2, 2): "100kHz,2x2,high"
        }

        self.tbl["DET.WIN"] = {
            4096 : "F" ,
            4136 : "F"
        }

        return self.tbl
