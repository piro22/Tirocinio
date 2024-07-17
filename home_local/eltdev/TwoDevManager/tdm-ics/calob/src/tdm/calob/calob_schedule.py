"""
@ingroup tdm_calob
@copyright ESO - European Southern Observatory
"""

import os

from ifw.calob.calibration_plan import *

from tdm.calob.calob_ext_functions import *
from tdm.calob.calob_tables import *


class CalibrationSchedule(InstrumentCalibrationSchedule):

    def __init__(self,
                 cc = None):
        
        super().__init__(cc)
        self.clb = InstrumentCalibrationLibrary(cc, 
                                                CalibrationExternalFunctions(cc),
                                                CalibrationTables(cc).get_tables())
        super().set_calibration_library(self.clb)

    def init_calibration_schedule(self):
        condition  = "(h['ESO DPR CATG'] == 'SCIENCE' or "
        condition += " h['ESO DPR CATG'] == 'CALIB' or "
        condition += " h['ESO DPR CATG'] == 'ACQUISITION') and "
        condition += " h['ESO DPR TYPE'] != 'BIAS'"
        super().add_filter_condition(condition)

        ##########################################################################################
        # Static calibrations:
        ##########################################################################################
        super().add_static_calib("tmp_bias.obd.json") \
            .set_group("AA") \
            .rule_min_days_since_last_exec(3)
        
        super().add_static_calib("tmp_dark.obd.json") \
            .set_group("AA") \
            .rule_min_days_since_last_exec(1)
        ##########################################################################################

        ##########################################################################################
        # Dynamic calibrations:
        ##########################################################################################
        super().add_template_calib("tmp_bias.obdx.json") \
            .set_group("AB") \
            .rule_min_days_since_last_exec(1) \
            .rule_only_if_key_eq("INS.MODE", "MOS")
        ##########################################################################################