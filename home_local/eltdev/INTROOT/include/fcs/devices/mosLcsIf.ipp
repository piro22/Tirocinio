/**
 * @ingroup devices
 * @copyright ESO - European Southern Observatory
 *
 * @brief mosLcsIf class header file.
 */

#ifndef FCS_MOS_IF_ICC
#define FCS_MOS_IF_ICC

#include <boost/any.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>



namespace fcs::mos {

inline std::string MosControllerData::GetErrorText() const {
  std::string error_text("unknown");
  auto errItd = fcs::mos::hw_errors.find(GetErrorCode());
  if (errItd != fcs::mos::hw_errors.end()) {
      error_text= errItd->second;
    }
  return error_text;
}

inline std::string MosControllerData::GetSubstateStr() const {
  auto stateItd = fcs::mos::SubstateMap.find(GetSubstate());
  std::string stateStr("Undefined");

  if (stateItd != fcs::mos::SubstateMap.end()) {
      stateStr = stateItd->second;
    }
  return stateStr;
}

}

#endif //FCS_MOS_IF_ICC


