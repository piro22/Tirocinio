/**
 * @ingroup devices
 * @copyright ESO - European Southern Observatory
 *
 * @brief MosConfig class header file.
 */

#ifndef FCS_MOS_CONFIG_HPP
#define FCS_MOS_CONFIG_HPP

// System headers
#include <string>

// Third party headers
#include <yaml-cpp/yaml.h>

#include <utils/bat/logger.hpp>
#include "fcf/devmgr/common/deviceConfig.hpp"
#include "fcf/devmgr/common/dataContext.hpp"
#include "fcs/devices/mosRpcErrors.hpp"
#include "fcs/devices/mosHwErrors.hpp"

namespace fcs::mos {

    constexpr auto LOGGER_NAME = "mos";
    // Constants for internal mapping
    // Add constants here:

    // Specifics Mos status values

    // Specific Mos RPC
    // Add RPCs here:
    constexpr auto RPC_PING                   = "rpcPing";


    // Specific constant values for Mos device state/substate
    constexpr int SUBSTATE_OP_IDLE               =  206;

    constexpr auto SUBSTATE_OP_IDLE_STR          =  "Idle";

    const std::unordered_map<short, std::string>  SubstateMap = {
      {fcf::devmgr::common::SUBSTATE_NOTREADY,       fcf::devmgr::common::SUBSTATE_NOTREADY_STR},
      {fcf::devmgr::common::SUBSTATE_INITIALISING,   fcf::devmgr::common::SUBSTATE_INITIALISING_STR},
      {fcf::devmgr::common::SUBSTATE_READY,       fcf::devmgr::common::SUBSTATE_READY_STR},
      {SUBSTATE_OP_IDLE,           SUBSTATE_OP_IDLE_STR}
    };



}


#endif //FCS_MOS_CONFIG_H
