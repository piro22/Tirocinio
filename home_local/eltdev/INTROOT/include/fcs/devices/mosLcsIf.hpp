/**
 * @ingroup devices
 * @copyright ESO - European Southern Observatory
 *
 * @brief mosLcsIf class header file.
 */

#ifndef FCS_MOS_IF_HPP
#define FCS_MOS_IF_HPP

#include <unordered_map>

#include <yaml-cpp/yaml.h>
#include "fcf/devmgr/common/deviceLcsIf.hpp"
#include "fcs/devices/mosConfig.hpp"
#include "fcf/devmgr/common/dataContext.hpp"

#include <rad/mal/publisher.hpp>

namespace fcs::mos {


    /**
     * @brief The mosControllerData struct
     *
     * This structure is used store the status information about the mos.
     * The mosLcsIf class will read the mos status information from the
     * controller and it will store it in this structure.
     */
    struct MosControllerData : fcf::devmgr::common::DeviceControllerData {
    public:
      MosControllerData();
      virtual ~MosControllerData();

      inline std::string GetErrorText() const;
      void SetData(const MosControllerData& data);

      inline virtual std::string GetSubstateStr() const;

    protected:
      
    };


    /**
     * @brief Mos Local Control System (LCS) Interface (IF) class
     *
     * This class manages the Mos connection to the LCS encapsulating
     * all basic communication with the controller or the simulator.
     *
     * Each RPC call is synchronous but it only starts the action in the
     * controller. The controller will return successfully if the action can
     * be started, not that the action was completed.
     *
     * The completion of the action must be monitored by reading the status
     * of the controller but this is out of the scope of this class.
     */

    class MosLcsIf : public fcf::devmgr::common::DeviceLcsIf
    {

    public:
      /**
       * @brief mosLcsIf constructor.
       * @param[inout] config    Shared pointer to the device configuration.
       * @param[inout] data_ctx  Reference to the data context object.
       */
       MosLcsIf(std::shared_ptr<fcf::devmgr::common::IDeviceConfig> config,
                         fcf::devmgr::common::DataContext& data_ctx);


      /**
       * @brief Mos destructor
       */
      virtual ~MosLcsIf() {};

     
      /**
       * @brief Executes the Ping RPC
       * @throws an exception if an error ocurrs.
       *
       *
       */
      void Ping();


      /**
       *
       * @param error_code Error code
       * @return Associated error text to the passed error code.
       */
      virtual std::string GetRpcError(const short error_code) const;





    private:
      log4cplus::Logger  m_logger;

    };

}

#include "mosLcsIf.ipp"

#endif //FCS_MOS_IF_HPP
