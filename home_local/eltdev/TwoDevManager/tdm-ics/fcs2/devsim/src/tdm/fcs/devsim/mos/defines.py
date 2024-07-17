"""
@copyright ESO - European Southern Observatory
@brief PLC Constants defined for the motor device.
"""

#################################################################################################
class MOS_COMMAND():
    NONE           = 0
    RESET          = 1
    INIT           = 2
    STOP           = 3
    ENABLE         = 4
    DISABLE        = 5
    PING           = 6

#################################################################################################

#################################################################################################
class MOS_ERROR():
    OK                    =    0
    HW_NOT_OP             =    1
    INIT_FAILURE          =    2
    UNKNOWN_CMD           = 1000
#################################################################################################
class MOS_ERROR_TEXT():
    OK                    = "OK"
    HW_NOT_OP             = "HW not operational"
    INIT_FAILURE          = "Initialisation failure"
    UNKNOWN_CMD           = "Unknown command encountered"
#################################################################################################
MOS_ERROR_TO_TEXT = { MOS_ERROR.OK:               MOS_ERROR_TEXT.OK,
                      MOS_ERROR.HW_NOT_OP:        MOS_ERROR_TEXT.HW_NOT_OP,
                      MOS_ERROR.INIT_FAILURE:     MOS_ERROR_TEXT.INIT_FAILURE,    
                      MOS_ERROR.UNKNOWN_CMD:      MOS_ERROR_TEXT.UNKNOWN_CMD}

MOS_ERROR_TO_NUM = {error_text: error for error, error_text in MOS_ERROR_TO_TEXT.items()}
#################################################################################################

#################################################################################################
class MOS_SIGNAL():

    # Signals corresponding to commands:
    RESET           = "Reset"
    INIT            = "Init"
    STOP            = "Stop"
    ENABLE          = "Enable"
    DISABLE         = "Disable"

    # Example of signal
    PING            = "Ping"

    # Signals corresponding to internal events:
    INIT_DONE       = "InitDone"

    # HW signals:
    HW_OK           = "HwOk"
    ERR_HW          = "ErrHw"
    ERR_INIT        = "ErrInit"
    ERR_DISABLE     = "ErrDisable"

#################################################################################################

#################################################################################################
class MOS_ACTION():
    DISABLE_EXECUTE      = "Disable.Execute"
    ENABLE_COMPLETE      = "Enable.Complete"
    ERR_EXECUTE          = "Err.Execute"
    INIT_COMPLETE        = "Init.Complete"
    INIT_EXECUTE         = "Init.Execute"
    INIT_RESET           = "Init.Reset"
    INIT_STOP            = "Init.Stop"

    RESET_EXECUTE        = "Reset.Execute"
    STOP_EXECUTE         = "Stop.Execute"
    UNEXP_EXECUTE        = "Unexp.Execute"

#################################################################################################

#################################################################################################
class MOS_HW_STATUS():
    UNKNOWN   = 0
    ERROR     = 1

#################################################################################################

#################################################################################################
class MOS_RPC_ERROR():
    OK                  =  0
    NOT_OP              = -1
    NOT_NOTOP_READY     = -2
    NOT_NOTOP_NOTREADY  = -3
    LOCAL               = -7
#################################################################################################
class MOS_RPC_ERROR_TEXT():
    OK                    = "OK"
    NOT_OP                = "Device not in Operational state"
    NOT_NOTOP_READY       = "Device not in Not-Operational-Ready state"
    NOT_NOTOP_NOTREADY    = "Device not in Not-Operational-Not-Ready state"
    LOCAL                 = "Device in Local Mode - cannot control remotely"
#################################################################################################
MOS_RPC_ERROR_TO_TEXT = { MOS_RPC_ERROR.OK:                 MOS_RPC_ERROR_TEXT.OK,
                          MOS_RPC_ERROR.NOT_OP:         MOS_RPC_ERROR_TEXT.NOT_OP,
                          MOS_RPC_ERROR.NOT_NOTOP_READY:    MOS_RPC_ERROR_TEXT.NOT_NOTOP_READY,
                          MOS_RPC_ERROR.NOT_NOTOP_NOTREADY: MOS_RPC_ERROR_TEXT.NOT_NOTOP_READY,                   
                          MOS_RPC_ERROR.LOCAL:              MOS_RPC_ERROR_TEXT.LOCAL}

MOS_RPC_ERROR_TO_NUM = {error_text: error for error, error_text in MOS_RPC_ERROR_TO_TEXT.items()}
#################################################################################################

#################################################################################################
class MOS_STATUS():
    OK        = 0
    ERROR     = 1
#################################################################################################

#################################################################################################
class MOS_STATE():
    NONE  = 0
    NOTOP = 1
    OP    = 2
#################################################################################################

#################################################################################################
class MOS_SUBSTATE():
    NONE                  =   0
    NOTOP_NOTREADY        = 100
    NOTOP_READY           = 101
    NOTOP_INITIALISING    = 102
    NOTOP_ERROR           = 199
    OP_DISABLING          = 205
    OP_IDLE               = 206
    OP_ERROR              = 299
#################################################################################################

if __name__ == "__main__":
    pass

# EOF
