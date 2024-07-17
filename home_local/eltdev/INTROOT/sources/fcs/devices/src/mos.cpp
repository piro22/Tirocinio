/**
 * @ingroup devices
 * @copyright ESO - European Southern Observatory
 *
 * @brief Mos class source file.
 */

#include <rad/exceptions.hpp>
#include <rad/helper.hpp>
#include <rad/assert.hpp>


#include "fcs/devices/mos.hpp"
#include "fcs/devices/mosConfig.hpp"
#include "fcs/devices/mosEncDec.hpp"

#include <Fcfif.hpp>

using namespace fcf::devmgr::common;

namespace fcs::mos {

  REGISTER_DEVICE(Mos);

Mos::Mos(const std::string& filename,
  const std::string& name,
  fcf::devmgr::common::DataContext& data_ctx):
  Device(filename, name, data_ctx),
  m_logger(log4cplus::Logger::getInstance(LOGGER_NAME)) {
  LOG4CPLUS_TRACE_METHOD(m_logger,__PRETTY_FUNCTION__);
}

void Mos::CreateObjects(std::shared_ptr<fcf::devmgr::common::IDeviceConfig>  config) {
  LOG4CPLUS_TRACE_METHOD(m_logger,__PRETTY_FUNCTION__);

  if (config) {
      // This branch is used only by the unit tests
      m_config = std::static_pointer_cast<fcf::devmgr::common::DeviceConfig>(config);
    }
  else {
      m_config = std::make_shared<fcf::devmgr::common::DeviceConfig>(m_filename, m_name);
    }

  m_lcs_if = std::make_shared<MosLcsIf>(m_config, m_data_ctx);
  m_lcs_if->InitObject();

  // Writes initial device configuration into the OLDB
  this->WriteConfig();
}

void Mos::Setup(const std::any& payload) {
  LOG4CPLUS_TRACE_METHOD(m_logger,__PRETTY_FUNCTION__);

  LOG4CPLUS_INFO(m_logger, "Processing Setup command ...");
  if (!m_config->GetIgnored()) {
    try {
      auto fcf_vector = std::any_cast<std::vector<std::shared_ptr<fcfif::SetupElem>>>(&payload);
      for (auto it = fcf_vector->begin(); it != fcf_vector->end(); it++) {
           auto setup_elem = *it;
           LOG4CPLUS_INFO(m_logger, "ID: " << setup_elem->getId());
           // ignore other shutters
           if (IsMsgForMe(setup_elem->getId()) != true) {
               continue;
             }
           auto lcs_if = std::static_pointer_cast<MosLcsIf>(m_lcs_if);
           auto fcs_union = setup_elem->getDevice();
           if (fcs_union->getDiscriminator() != ::fcfif::DeviceType::CUSTOM) {
               LOG4CPLUS_ERROR(m_logger, "[" << m_config->GetName() << "] "
                               << "Setup is for me but device type is not correct?");
               continue;
             }

           auto custom = fcs_union->getCustom();
          LOG4CPLUS_DEBUG(m_logger, "[" << m_config->GetName() << "] "
                          << "Setup ID: " << setup_elem->getId());

          std::string params = boost::replace_all_copy(custom->getParameters(), "'", "\"");
          LOG4CPLUS_INFO(m_logger, "[" << m_config->GetName() << "] " 
                                    << "Setup buffer: " << params);
          //@TODO: Add handling of setup parameters

          // Parsing of JSON payload
          EncDec data = nlohmann::json::parse(params);
          if (data.has_piston()) {
              LOG4CPLUS_INFO(m_logger, "[" << m_config->GetName() << "] " 
                                      << "Piston: " << data.get_piston());
            }
          LOG4CPLUS_INFO(m_logger, "[" << m_config->GetName() << "] " 
                                    << "Tip: " << data.get_tip());
          LOG4CPLUS_INFO(m_logger, "[" << m_config->GetName() << "] "
                                    << "Tilt: " << data.get_tilt());
          if (data.get_action() == "MOVE") {
              lcs_if->Ping();
            }
        }
      } catch(const std::bad_any_cast& e) {
        LOG4CPLUS_ERROR(m_logger, "CII payload is not recognized !");
        throw;
      }
    }
}

bool Mos::IsSetupActive(const std::any& payload) const
{
  LOG4CPLUS_TRACE_METHOD(m_logger,__PRETTY_FUNCTION__);

  if (!m_config->GetIgnored()) {
    try {
      auto fcf_vector = std::any_cast<std::vector<std::shared_ptr<fcfif::SetupElem>>>(&payload);

      for (auto it = fcf_vector->begin(); it != fcf_vector->end(); it++) {
           auto setup_elem = *it;
           // ignore other shutters
           if (IsMsgForMe(setup_elem->getId()) != true) {
               continue;
             }
           auto lcs_if = std::static_pointer_cast<MosLcsIf>(m_lcs_if);
           auto fcs_union = setup_elem->getDevice();
           if (fcs_union->getDiscriminator() != ::fcfif::DeviceType::CUSTOM) {
               LOG4CPLUS_ERROR(m_logger, "[" << m_config->GetName() << "] "
                               << "Setup is for me but device type is not correct?");
               continue;
             }
           auto custom = fcs_union->getCustom();

          std::string params = boost::replace_all_copy(custom->getParameters(), "'", "\"");
          EncDec data = nlohmann::json::parse(params);        
          if (data.get_action() == "MOVE") {
              //@TODO: Add handling of setup acceptance.
              // For the example we are saying the command is immediately ready (not active).

              return false;
            }
        }
      } catch(const std::bad_any_cast& e) {
        LOG4CPLUS_ERROR(m_logger, "payload is not recognized !");
        throw;
      }
    }
  return false;
}

void Mos::UpdateStatus() {
  LOG4CPLUS_TRACE_METHOD(m_logger,__PRETTY_FUNCTION__);
  if (!m_config->GetIgnored()) {
      RAD_ASSERT(m_lcs_if != nullptr);

      // Read status from the controller using the LCS interface.
      if (m_lcs_if->IsConnected()) {
          m_lcs_if->ReadStatus(m_controller_status);
        } else {
          // If not connected force unavailable state/substate
          m_controller_status.SetState(0);
          m_controller_status.SetSubstate(0);
        }

      LOG4CPLUS_DEBUG(m_logger, "State: "     << m_controller_status.GetState());
      LOG4CPLUS_DEBUG(m_logger, "Substate: "  << m_controller_status.GetSubstate());
      LOG4CPLUS_DEBUG(m_logger, "Local: "     << m_controller_status.GetLocalFlag());
      LOG4CPLUS_DEBUG(m_logger, "Error: "     << m_controller_status.GetErrorCode());

      // Build the list for updating the OLDB
      // T// Build the list for updating the OLDB
      // The vector is a key value list.
      utils::bat::DbVector status_vec;
      // The reserved number should be big enough to store all attributes
      status_vec.reserve(10);

      Device::GetStatusList(status_vec,m_controller_status);

      // Write status into the OLDB
      UpdateDb(status_vec);
    }
}

void Mos::Status(const std::any& payload, std::string& buffer)
{
  LOG4CPLUS_TRACE_METHOD(m_logger,__PRETTY_FUNCTION__);
  // Check whether the payload contains an specific device definition.
  if (CheckNameParam(payload)) {
      // Add state to the buffer
      std::string device_status("");

      if (!m_config->GetIgnored()) {

          // Read status from LCS
          UpdateStatus();

          // Prefix for attributes coming from the LCS
          std::string stat_prefix = m_config->GetName() + "." + fcf::devmgr::common::DB_CONTROLLER_PREFIX;

          // A// Add common device attributes
          Device::GetStatusBuffer(stat_prefix, &m_controller_status, device_status);

          // Add substate to the buffer
          device_status.append(stat_prefix + "." + fcf::devmgr::common::CI_STAT_SUBSTATE
                               + " = " + m_controller_status.GetSubstateStr() + "\n");

        } else {
          device_status.append(m_config->GetName()
                               + "." + fcf::devmgr::common::CI_IGNORED + " = " +
                               "true" + "\n");
        }
      // Update original buffer
      buffer.append(device_status);
      LOG4CPLUS_INFO(m_logger, "buffer:" << buffer);
    }
}

void Mos::Status(bool end_acq,
                                                       const core::dit::did::Did& dictionary,
                                                       std::shared_ptr<CCfits::FITS>& fits_handle) {
  LOG4CPLUS_TRACE_METHOD(m_logger,__PRETTY_FUNCTION__);
  if (! end_acq) {
      return;
    }

  RAD_ASSERT(fits_handle.get() != nullptr);

  Config& config = m_data_ctx.GetConfig();
  const std::string prefix = config.GetFitsPrefix() + " " + m_config->GetFitsPrefix() + " ";

  // Add basic device information
  Device::GetStatusHeader(prefix, dictionary, fits_handle);

  if (!m_config->GetIgnored()) {
      // Read status from LCS
      UpdateStatus();

      core::dit::fits::AddKey(fits_handle, dictionary, prefix + FITS_PARAM_STAT,
                              m_controller_status.GetSubstateStr(), 1);

    } else {
      core::dit::fits::AddKey(fits_handle, dictionary, prefix + FITS_PARAM_DEVIGN,
                              true, 1);
    }

}
}
