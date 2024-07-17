/**
 * @ingroup devices
 * @copyright ESO - European Southern Observatory
 *
 * @brief MosLcsIf class source file.
 */

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <exception>
#include <boost/any.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>

#include <rad/assert.hpp>
#include <rad/exceptions.hpp>
#include <rad/helper.hpp>

#include "fcs/devices/mosLcsIf.hpp"

using namespace fcf::devmgr::common;

namespace fcs::mos {

    MosControllerData::MosControllerData():
      fcf::devmgr::common::DeviceControllerData() {
    }

    MosControllerData::~MosControllerData() {
    }

    void MosControllerData::SetData(const MosControllerData& data) {
      fcf::devmgr::common::DeviceControllerData::SetData(data);
    }

    MosLcsIf::MosLcsIf(std::shared_ptr<fcf::devmgr::common::IDeviceConfig> config,
                                     fcf::devmgr::common::DataContext& data_ctx):
        DeviceLcsIf(config, data_ctx),
        m_logger(log4cplus::Logger::getInstance(LOGGER_NAME)) {

        LOG4CPLUS_TRACE_METHOD(m_logger,__PRETTY_FUNCTION__);

        // JKN/2023-02-08: Commented out the following lines. They are contained in the 
        // <device>LcsIf::InitObject() in the other FCF Devices.
        // if (!IsConfigLoaded()) {
        //     throw std::runtime_error ("Mos map configuration not loaded");
        //     return;
        //   }

        // // fill up the base container with specific device errors.
        // m_hw_errors.insert(hw_errors.begin(), hw_errors.end());
      }

    void MosLcsIf::Ping() {
        LOG4CPLUS_TRACE_METHOD(m_logger,__PRETTY_FUNCTION__);

        LOG4CPLUS_INFO(m_logger, "[" << m_config->GetName() << "] "
                       << "Executing RPC_PING ...");
        protocol::base::VectorVariant attr_list;
        std::string obj;
        std::string proc;
        proc = m_config->GetProcId(GetMapValue(fcf::devmgr::common::CAT_RPC,
                                               RPC_PING));
        obj = m_config->GetObjId();
        try {
          ExecuteRpc(obj, proc, attr_list);
          LOG4CPLUS_INFO(m_logger, "[" << m_config->GetName() << "] "
                         << "Successfull call of Mos ping: ");
        }
        catch (std::exception& e) {
          const std::string msg = "[" + m_config->GetName()
              + "] Ping failure: " + e.what();
          LOG4CPLUS_ERROR(m_logger, msg);
          throw std::runtime_error(msg);
        }

      }

    std::string MosLcsIf::GetRpcError(const short error_code) const {
        switch (error_code) {
          case RPC_ERR_NOT_OP:
          case RPC_ERR_NOT_OP_READY:
          case RPC_ERR_NOT_OP_NOTREADY: {
              auto it = rpc_errors.find(error_code);
              std::string err_msg;
              if (it != rpc_errors.end()) {
                  err_msg = "RPC error: " + it->second;
                }
              else {
                  err_msg = "unregistered RPC error code: " + std::to_string(error_code);
                }

              return err_msg;
            }
            break;
          default: {
              const std::string err_msg = "unknown error message for RPC code: " +
                  std::to_string(error_code);
              return err_msg;
            }
            break;
          }
        return "";
      }

}
