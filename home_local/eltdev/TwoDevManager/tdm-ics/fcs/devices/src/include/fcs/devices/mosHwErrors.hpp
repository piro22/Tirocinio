/**
 * @ingroup devices
 * @copyright ESO - European Southern Observatory
 *
 * @brief MosHwErrors header file.
 */

#ifndef FCS_MOS_HW_ERRORS_HPP
#define FCS_MOS_HW_ERRORS_HPP

#include <unordered_map>

namespace fcs::mos {

    enum {
      HW_SUCCESS               = 0,
      HW_ERR_HW_NOT_OP         = 1,
      HW_ERR_WRONG_CMD         = 2,
      HW_ERR_INCONSISTENT      = 3,
      HW_ERR_INIT_FAILURE      = 4,
      HW_ERR_ENABLE_FAILURE    = 5,
      HW_ERR_DISABLE_FAILURE   = 6
    };

    const std::unordered_map<short, std::string> hw_errors = {
      {HW_ERR_HW_NOT_OP,       "Mos no in operational state"},
      {HW_ERR_WRONG_CMD,       "Mos unknown command"},
      {HW_ERR_INCONSISTENT,    "Mos inconsistent hardware signals"},
      {HW_ERR_INIT_FAILURE,    "Mos init failure"},
      {HW_ERR_ENABLE_FAILURE,  "Mos enable failure"},
      {HW_ERR_DISABLE_FAILURE, "Mos disable failure"},
    };
}

#endif // FCS_MOS_HW_ERRORS_HPP
