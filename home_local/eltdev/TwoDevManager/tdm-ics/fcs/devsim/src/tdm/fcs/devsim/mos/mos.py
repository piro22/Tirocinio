"""
@copyright ESO - European Southern Observatory
@brief Device Simulator for the standard Mos Device Manager.
"""

import sys
import time
import logging

from asyncua import ua, uamethod, Server

from scxml4py.activity import ThreadedActivity as Scxml4pyActivity
from scxml4py.event import Event as Scxml4pyEvent

import ifw.core.utils.utils as core_utils
import ifw.core.opcualib.utils.utils as core_opcua_utils
import ifw.core.opcualib.srvlib.serverinst as core_opcua_srv_inst
import ifw.core.opcualib.srvlib.statemachine as core_opcua_sm
import ifw.core.opcualib.srvlib.serverbase as core_opcua_sm_srv

import ifw.fcf.devsim.lib.device_simulator_base as devsim_base

#import fcs_devsim_mos.defines as mos_def
import tdm.fcs.devsim.mos.defines as mos_def

class ActivityInitialising(core_opcua_sm.Activity):

    def __init__(self,
                 event_queue,
                 data):
        core_opcua_sm.Activity.__init__(self, "Initialising", event_queue, data)

    def run(self):
        srv = self.get_srv()
        while self.isRunning() == True:
            srv.log().debug("Activity %s is running ..." % self.getId())
            time.sleep(srv.sim_init_time)

            # TODO: Handle InitOn as well.
            self.sendInternalEvent(Scxml4pyEvent("InitDone"))

            if (srv.get_cfg().get_value("auto_enter_op")):
                self.sendInternalEvent(Scxml4pyEvent("Enable"))

            self.setRunning(False)
            srv.log().debug("Activity %s terminating" % self.getId())
            break


class ActivityDisabling(core_opcua_sm.Activity):

    def __init__(self,
                 event_queue,
                 data):
        Scxml4pyActivity.__init__(self, "Disabling", event_queue, data)

    def run(self):
        srv = core_opcua_srv_inst.get_srv()
        while self.isRunning() == True:
          time.sleep(1)
          self.sendInternalEvent(Scxml4pyEvent("DisableDone"))
          self.setRunning(False)
          break


class ActionMgr(core_opcua_sm.ActionMgr):

    def __init__(self, srv_instance):
        core_opcua_sm.ActionMgr.__init__(self, srv_instance)

    def create_actions(self,
                       event_queue,
                       data):
        self.set_actions([core_opcua_sm.ActionState(),                          
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.DISABLE_EXECUTE),
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.ENABLE_COMPLETE),
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.ERR_EXECUTE),
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.INIT_COMPLETE),
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.INIT_EXECUTE),
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.INIT_RESET),
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.INIT_STOP),
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.RESET_EXECUTE),
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.STOP_EXECUTE),
                          core_opcua_sm.SetActionDesc(mos_def.MOS_ACTION.UNEXP_EXECUTE)])

    def create_activities(self,
                          event_queue,
                          data):
        self.set_activities([ActivityDisabling(event_queue, data),
                             ActivityInitialising(event_queue, data)])


class DevsimMos(devsim_base.DeviceSimulatorBase):
    """
    Device Simulator class for the Standard Mos Device.
    """

    def __init__(self): 
        """
        Initialise the Mos Device simulator class; set the members.
        """
        self.log().info("Initialising Device Simulator")

        devsim_base.DeviceSimulatorBase.__init__(self)

        self.set_device_info("Standard Mos Device",
                             "FCF DeviceSimulatorMos",
                             "WS Simulator",
                             "Standard Mos Device",
                             "2019-05-06",
                             "1", "0")
        self.initialised = False
        self.sim_init_time = 1.0

        self.state_reg().\
        add_mapping("NOTOP", "NotOperational", mos_def.MOS_STATE.NOTOP).\
        add_mapping("OP", "Operational", mos_def.MOS_STATE.OP).\
        add_mapping("NOTOP_NOTREADY", "NotOperational_NotReady",
                    mos_def.MOS_SUBSTATE.NOTOP_NOTREADY).\
        add_mapping("NOTOP_READY", "NotOperational_Ready",
                    mos_def.MOS_SUBSTATE.NOTOP_READY).\
        add_mapping("NOTOP_INITIALISING", "NotOperational_Initialising",
                    mos_def.MOS_SUBSTATE.NOTOP_INITIALISING).\
        add_mapping("NOTOP_ERROR", "NotOperational_Error",
                    mos_def.MOS_SUBSTATE.NOTOP_ERROR).\
        add_mapping("OP_DISABLING", "Operational_Disabling",
                    mos_def.MOS_SUBSTATE.OP_DISABLING).\
        add_mapping("OP_IDLE", "Operational_Idle",
                    mos_def.MOS_SUBSTATE.OP_IDLE). \
        add_mapping("OP_ERROR", "Operational_Error",
                    mos_def.MOS_SUBSTATE.OP_ERROR)


    async def initialise(self):
        """
        Initialise various parameters of the simulated device.
        """
        self.log().info("Initialising Device Simulator")
        self.execute_sim_thr()
        cfg = self.get_cfg()

        # Set simulation parameters:
        self.sim_init_time = cfg.get_value("sim_init_time")

        # Set default values for various configuration, control and status parameters:
        self.write_node("ctrl.bExecute", False)
        self.write_node("ctrl.nCommand", 0)

        local_mode = cfg.get_value("local_mode")
        self.log().info("Local Mode is: %s" % local_mode)
        self.write_node("stat.bLocal", local_mode)

        self.set_error_attribs(mos_def.MOS_ERROR.OK)
        self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.OK)

        await devsim_base.DeviceSimulatorBase.initialise(self)

        if (self.get_cfg().get_value("auto_enter_op")):
            self.write_node("ctrl.nCommand", mos_def.MOS_COMMAND.INIT)
            self.write_node("ctrl.bExecute", True)


    def set_error_attribs(self,
                          error_code):
        """
        Set the "stat.nErrorCode"/"stat.sErrorText" node values.
        """
        self.write_node("stat.nErrorCode", error_code)
        self.write_node("stat.sErrorText", mos_def.MOS_ERROR_TO_TEXT[error_code])


    def set_rpc_error_attribs(self,
                              rpc_error_code):
        """
        Set the "stat.nRpcErrorCode"/"stat.sRpcErrorText" node values.
        """
        self.write_node("stat.nRpcErrorCode", rpc_error_code)
        self.write_node("stat.sRpcErrorText", mos_def.MOS_RPC_ERROR_TO_TEXT[rpc_error_code])


    def sim_thr_user(self):
        """
        Simulation Thread managing the mos status.
        """
        pass


    def data_change_handler(self,
                            node,
                            value,
                            data):
        """
        Invoked when subscribed "ctrl." and "stat." nodes change value.
        """
        self.log().debug("node=|%s|, value=|%s|" % (node.nodeid.to_string(), value))
        node_id = core_opcua_utils.get_node_id(node)
        self.log().debug("%s=%s, %s" % (node_id, str(value), type(value)))
        if ((node_id.find("ctrl.bExecute") != -1) and (value == True)):
            self.set_error_attribs(mos_def.MOS_ERROR.OK)
            command = self.read_node("ctrl.nCommand")
            self.write_node("ctrl.nCommand", mos_def.MOS_COMMAND.NONE)
            self.write_node("ctrl.bExecute", False)

            if (command == mos_def.MOS_COMMAND.RESET):
                self.schedule_event(Scxml4pyEvent("Reset"), "Reset Mos Device")

            elif (command == mos_def.MOS_COMMAND.INIT):
                self.schedule_event(Scxml4pyEvent("Init"), "Init Mos Device")

            elif (command == mos_def.MOS_COMMAND.STOP):
                self.schedule_event(Scxml4pyEvent("Stop"), "Stop On-Going Operation")

            elif (command == mos_def.MOS_COMMAND.ENABLE):
                self.schedule_event(Scxml4pyEvent("Enable"), "Enable Mos Device")

            elif (command == mos_def.MOS_COMMAND.DISABLE):
                self.schedule_event(Scxml4pyEvent("Disable"), "Disable Mos Device")
            elif (command == mos_def.MOS_COMMAND.PING):
                self.schedule_event(Scxml4pyEvent("Ping"), "Ping Mos Device")
            else:
                self.set_error_attribs(mos_def.MOS_ERROR.UNKNOWN_CMD)


    @uamethod
    def RPC_Init(self,
                 parent):
        """
        Initiate the initialisation procedure.
        """
        self.log().debug("Entering RPC_Init()")

        if (self.local_mode()):
            self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.LOCAL)
            return ua.Variant(mos_def.MOS_RPC_ERROR.LOCAL, ua.VariantType.Int16)

        if (self.read_node("stat.nSubstate") not in
            [mos_def.MOS_SUBSTATE.NOTOP_NOTREADY, mos_def.MOS_SUBSTATE.NOTOP_ERROR]):
            self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.NOT_NOTOP_NOTREADY)
            return ua.Variant(mos_def.MOS_RPC_ERROR.NOT_NOTOP_NOTREADY, ua.VariantType.Int16)

        # Trigger the initialisation to start.
        self.write_node("ctrl.nCommand", mos_def.MOS_COMMAND.INIT)
        self.write_node("ctrl.bExecute", True)

        self.set_last_command("RPC_Init")
        self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.OK)
        return ua.Variant(self.RPC_RET_OK, ua.VariantType.Int16)


    @uamethod
    def RPC_Reset(self,
                  parent):
        """
        Reset the Mos Device (bring it to NotOp/NotReady).
        """
        if (self.local_mode()):
            self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.LOCAL)
            return ua.Variant(mos_def.MOS_RPC_ERROR.LOCAL, ua.VariantType.Int16)
        self.write_node("ctrl.nCommand", mos_def.MOS_COMMAND.RESET)
        self.write_node("ctrl.bExecute", True)
        self.set_error_attribs(mos_def.MOS_ERROR.OK)
        self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.OK)
        self.set_last_command("RPC_Reset")
        return ua.Variant(self.RPC_RET_OK, ua.VariantType.Int16)


    @uamethod
    def RPC_Enable(self,
                   parent):
        """
        Execute SM Enable operation.
        """
        if (self.local_mode()):
            self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.LOCAL)
            return ua.Variant(self.RPC_RET_LOCAL_MODE, ua.VariantType.Int16)

        if ((self.read_node("stat.nState") == mos_def.MOS_STATE.NOTOP) and
            (self.read_node("stat.nSubstate") not in
             [mos_def.MOS_SUBSTATE.NOTOP_READY])):
            self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.NOT_NOTOP_NOTREADY)
            return ua.Variant(mos_def.MOS_RPC_ERROR.NOT_NOTOP_NOTREADY, ua.VariantType.Int16)

        # Reset off timer + submit "Enable" command to command engine.

        self.write_node("ctrl.nCommand", mos_def.MOS_COMMAND.ENABLE)
        self.write_node("ctrl.bExecute", True)

        self.set_last_command("RPC_Enable")
        self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.OK)
        return ua.Variant(self.RPC_RET_OK, ua.VariantType.Int16)


    @uamethod
    def RPC_Ping(self,
                   parent):
        """
        Execute SM Ping operation.
        """
        if (self.local_mode()):
            self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.LOCAL)
            return ua.Variant(self.RPC_RET_LOCAL_MODE, ua.VariantType.Int16)

        if ((self.read_node("stat.nState") == mos_def.MOS_STATE.NOTOP) and
            (self.read_node("stat.nSubstate") not in
             [mos_def.MOS_SUBSTATE.NOTOP_READY])):
            self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.NOT_NOTOP_NOTREADY)
            return ua.Variant(mos_def.MOS_RPC_ERROR.NOT_NOTOP_NOTREADY, ua.VariantType.Int16)

        # Reset off timer + submit "Enable" command to command engine.

        self.write_node("ctrl.nCommand", mos_def.MOS_COMMAND.PING)
        self.write_node("ctrl.bExecute", True)

        self.set_last_command("RPC_Ping")
        self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.OK)
        return ua.Variant(self.RPC_RET_OK, ua.VariantType.Int16)


    @uamethod
    def RPC_Disable(self,
                   parent):
        """
        Execute SM Disable operation.
        """
        if (self.local_mode()):
            self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.LOCAL)
            return ua.Variant(self.RPC_RET_LOCAL_MODE, ua.VariantType.Int16)

        if (self.read_node("stat.nState") != mos_def.MOS_STATE.OP):
            self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.NOT_OP)
            return ua.Variant(mos_def.MOS_RPC_ERROR.NOT_OP, ua.VariantType.Int16)

        self.write_node("ctrl.nCommand", mos_def.MOS_COMMAND.DISABLE)
        self.write_node("ctrl.bExecute", True)

        self.set_last_command("RPC_Disable")
        self.set_rpc_error_attribs(mos_def.MOS_RPC_ERROR.OK)
        return ua.Variant(self.RPC_RET_OK, ua.VariantType.Int16)


# EOF