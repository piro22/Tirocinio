/**
 * @ingroup devices
 * @copyright ESO - European Southern Observator
 *
 * @brief MosRpcErrors header file.
 */

#ifndef FCS_MOS_RPC_ERRORS_H
#define FCS_MOS_RPC_ERRORS_H

#include <unordered_map>

namespace fcs::mos {

  enum {
    RPC_SUCCESS = 0,
    RPC_ERR_NOT_OP               = -1,
    RPC_ERR_NOT_OP_READY    = -2,
    RPC_ERR_NOT_OP_NOTREADY = -3
  };

  const std::unordered_map<short, std::string> rpc_errors = {
    {RPC_ERR_NOT_OP,               "Cannot disable Mos. Not in OP state"},
    {RPC_ERR_NOT_OP_READY,    "Cannot enable Mos. Not in NOT_OP_READY."},
    {RPC_ERR_NOT_OP_NOTREADY, "Cannot init Mos. Not in NOT_OP_NOTREADY or FAILURE."}
  };
}

#endif // FCS_MOS_RPC_ERRORS_H
