/**
 * @ingroup devices
 * @copyright ESO - European Southern Observatory
 *
 *
 * @brief Mos class header file.
 */

#ifndef FCS_MOS_HPP
#define FCS_MOS_HPP

#include "fcf/devmgr/common/device.hpp"
#include "fcs/devices/mosConfig.hpp"
#include "fcs/devices/mosLcsIf.hpp"
#include "fcf/devmgr/common/dataContext.hpp"


namespace fcs::mos {

    /**
     * @brief The Mos class
     *
     * This class handles the behaviour of a generic Mos device.
     *
     * The Mos class have two auxiliary objects. One for managing the device
     * configuration and the other one for managing the connection to the device
     * controller running in the Local Control System (LCS). These two objects
     * are created by a dedicated method. These objects inherits basic
     * behaviours from their parent classes.
     *
     * @see MosConfig
     * @see MosLcsIf
     * @see Device
     * @see DeviceConfig
     * @see DeviceLcsIf
     *
     */
    class Mos : public fcf::devmgr::common::Device {

    public:
      /**
       * @brief Mos constructor
       * @param[in] filename filename of Mos configuration file
       * @param[in] name     Mos identifier
       * @param[in] data_ctx reference to data context object
       */
      Mos(const std::string& filename,
           const std::string& name,
           fcf::devmgr::common::DataContext& data_ctx);

      /**
      * @brief Mos destructor
      */
      virtual ~Mos() = default;

      /**
       * @brief Create object instances
       * @param config Shared pointer to a device config object
       *
       * This method creates the instances of the device configuration and
       * LCS interface objects that are needed by the device. If can override the
       * type of the configuration by passing a parameter otherwise it will use the
       * default mosConfig.
       *
       *
       */
      void CreateObjects(std::shared_ptr<fcf::devmgr::common::IDeviceConfig>  config = nullptr) override;

      /**
       * @brief Executes a setup of the mos device
       * @param[in] setupMsg   contains the payload of the setup message.
       *
       * @return true if command is successfully executed, false otherwise.
       * @throws an exception if an error ocurrs.
       */
      virtual void Setup(const std::any& payload) override;

      /**
       * @brief Check if last setup message is still active
       * @param[in] setupMsg setup message associated with the action
       *
       * @return true is action is still ongoing, false otherwise.
       *
       * In case an error with the hardware while waiting for the
       * setup to finish, the method will catch the exception and
       * rethrow a new one with more context information.
       *
       * @throws std::runtime_error
       */
      virtual bool IsSetupActive(const std::any& payload) const override;


      /**
       * @brief Obtain the status of the device
       * @param statusMsg    Status message object
       * @param buffer       Buffer where the status will be written
       */
      virtual void Status(const std::any& payload, std::string& buffer) override;

       /**
       * @brief Obtain the status of the device
       * @param end_acq      Flag to indicate is the end of the acquisition
       * @param dictionary   Reference to the dictionary object
       * @param fits_handle  Pointer to the CCFITS object
       *
       * This method will obtain the data from each device and add the corresponding
       * meta-data to the CCfits object. If the end_acq is not true it will just
       * register the start of the acquisition.
       */
      virtual void Status(bool end_acq,
                          const core::dit::did::Did& dictionary,
                          std::shared_ptr<CCfits::FITS>& fits_handle) override;

      /**
       * @brief Update the status of the Mos device in the OLDB
       *
       * This method reads the status from the controller and updates
       * the database accordingly.
       */
      virtual void UpdateStatus() override;




    protected:
      //!< object containing the status of the controller
      MosControllerData           m_controller_status;

    private:
      log4cplus::Logger  m_logger;
    };
}


#endif //FCS_MOS_HPP
