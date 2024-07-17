# This Python file uses the following encoding: utf-8
from PySide2.QtCore import Slot
from PySide2.QtCore import Qt
from PySide2.QtGui import QColor

import taurus
from taurus.qt.qtgui.dialog import ProtectTaurusMessageBox

from f1.fcs.pyfcsgui.wdglib.ui_taurusmos import Ui_TaurusMos
from ifw.fcf.pyfcfgui.taurusdevicebase import TaurusDeviceBase
from ifw.fcf.pyfcfgui.taurusdevicebase import EventValueMap
from ifw.fcf.pyfcfgui.taurusdevicebase import CustomListener
import f1.fcs.clib.setup_buffer as sbuf

from enum import Enum


class SubstateEnum(Enum):
    Undefined: int = 0
    NotReady: int = 100
    Ready: int = 101
    Initialising: int = 102
    Error: int = 199
    Idle: int = 206
    OpError: int = 299


class TaurusMos(TaurusDeviceBase):

    substates_dict = {
        0: "Undefined",
        100: "NotReady",
        101: "Ready",
        102: "Initialising",
        199: "Error",
        206: "Idle",
        299: "Error",
        }

    def __init__(self, device_name, device_mgr):
        """
        Class constructor
        Args:
            device_name(str):  Device identifier
            device_mgr: Instance of fcfguicomms.DeviceManangerFacade
        """
        TaurusDeviceBase.__init__(self, device_name, device_mgr)
        self.ui = Ui_TaurusMos()
        self.ui.setupUi(self)

        self.buffer = sbuf.SetupBuffer(device_mgr.get_cii())
        self.init_wdg()
        self.start_monitoring()

    def init_wdg(self):
        """
        Initialize the widget
        """
        self.ui.nameLabel.setText(self.devname)
        self.ui.nameLabel.setToolTip(self.overview)
        self.ui.nameLabel.setWhatsThis(self.description)
        self.ui.stateLabel.insertEventFilter(self.state_filter)
        self.ui.optionsComboBox.activated[int].connect(self.on_activated_index)


    def start_monitoring(self):
        """
        Create the connections to the signals
        and set the corresponding listeners
        """
        self.listener = CustomListener()
        self.listener.changedSimulated.connect(self.update_simulated)
        self.listener.changedIgnored.connect(self.update_ignored)
        self.listener.changedLocal.connect(self.update_local)
        self.listener.changedErrorCode.connect(self.update_error_code)
        self.listener.changedErrorStr.connect(self.update_error_str)
        self.listener.changedSubstate.connect(self.update_substate)
        self.attr_simulated.addListener(self.listener)
        self.attr_ignored.addListener(self.listener)
        self.attr_local.addListener(self.listener)
        self.attr_error_code.addListener(self.listener)
        self.attr_error_str.addListener(self.listener)
        self.attr_substate.addListener(self.listener)

    def set_enable(self, flag: bool):
        """
        Disable/Enable all UI children in the widget.

        Args:
            flag (bool):   Flag to deciding to enable/disable

        """
        self.set_property(self.ui.nameLabel, "disabled", not flag)
        self.set_property(self.ui.stateLabel, "disabled", not flag)
        self.set_property(self.ui.substateLabel, "disabled", not flag)
        self.set_property(self.ui.optionsComboBox, "disabled", not flag)
        self.set_property(self.ui.setupButton, "disabled", not flag)
        self.ui.nameLabel.setEnabled(flag)
        self.ui.stateLabel.setEnabled(flag)
        self.ui.substateLabel.setEnabled(flag)
        self.ui.optionsComboBox.setEnabled(flag)
        self.ui.setupButton.setEnabled(flag)

    @Slot()
    def update_simulated(self, flag:bool):
        """
        Updates simulated button according device status

        Args:
            flag (bool):   Flag indicating simulation state
        """
        self.set_property(self.ui.simLed, "simled", flag)

    @Slot()
    def update_ignored(self, flag:bool):
        """
        Updates widget according device status

        Args:
            flag (bool):   Flag indicating ignored state
        """
        self.set_enable(not flag)

    @Slot()
    def update_local(self, flag:bool):
        """
        Updates local button according device status

        Args:
            flag (bool):   Flag indicating local state
        """
        self.set_property(self.ui.localLed, "localled", flag)

    @Slot()
    def update_error_code(self, code:int):
        """
        Updates error button according device status

        Args:
            code (int):   Error code
        """
        if code != 0:
            self.set_property(self.ui.errorLed, "errled", True)
        else:
            self.set_property(self.ui.errorLed, "errled", False)
            self.ui.errorLed.setToolTip("")

    @Slot()
    def update_error_str(self, text:str):
        """
        Updates error button tooltip according device status

        Args:
            text (str):   Error message
        """
        self.ui.errorLed.setToolTip(text)

    @Slot()
    def update_substate(self, substate:int):
        """
        Updates substate according device status

        Args:
            substate (int):   Substate code
        """
        self.ui.substateLabel.setText(TaurusMos.substates_dict[substate])


    @Slot()
    def prepareMessage(self, buffer):
        """
        Prepare the message.

        Args:
            buffer: setup buffer
        """
        tip = str(self.ui.tip.value())
        tilt = str(self.ui.tilt.value())
        piston = str(self.ui.piston.value())
        """ Generates JSON serialization """
        parameters = "{\"tip\": " + tip + "," \
                    + "\"tilt\": " + tilt + "," \
                    + "\"piston\": " + piston + "," \
                    + "\"action\": \"" + self.ui.optionsComboBox.currentText() + "\"}"

        buffer.add_custom_setup(self.devname, parameters)


    @ProtectTaurusMessageBox(title="Unexpected error when executing command")
    @Slot()
    def on_setupButton_clicked(self):
        """
        Prepare the message and send the setup command.

        Args:

        """

        self.buffer.reset_buffer()
        self.prepareMessage(self.buffer)
        self.mgr.setup(self.on_setupReply_received,
                       self.on_setupError_received,self.buffer)

    @Slot()
    def on_activated_index(self, index):
        """
        Enables automatically device checkbox when changing options.

        Args:
            index (int):   New selected index in optionsComboBox
        """
        self.ui.enableCheckBox.setCheckState(Qt.CheckState.Checked);

    @Slot()
    def on_setupReply_received(self, *args):
        """
        Actions when receiving a succesful setup reply.

        Args:
            args:   reply received
        """

        self.info(*args)


    @Slot()
    def on_setupError_received(self, *args):
        """
        Actions when receiving an error setup reply.

        Args:
            args:   reply received
        """
        #print(str(args[0][1]))

        self.error('Exception of type %s received during execution. Traceback: \n%s'
                       % (args[0][0].__name__, str(args[0][2])))
