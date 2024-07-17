/**
 * @file
 * @ingroup server
 * @copyright ESO - European Southern Observatory
 * @author
 *
 * @brief main source file.
 */


#include <mal/Cii.hpp>
#include <mal/utility/LoadMal.hpp>

#include <rad/mal/replier.hpp>
#include <rad/mal/utils.hpp>
#include <rad/exceptions.hpp>
#include <rad/logger.hpp>
#include <rad/smAdapter.hpp>

#include <fcf/devmgr/common/dataContext.hpp>
#include <fcf/devmgr/common/actionMgr.hpp>
#include <fcf/devmgr/common/actionsStd.hpp>
#include <fcf/devmgr/common/stdCmdsImpl.hpp>
#include <fcf/devmgr/common/appCmdsImpl.hpp>
#include <fcf/devmgr/common/daqCmdsImpl.hpp>
#include <fcf/devmgr/common/config.hpp>

#include <fcf/devmgr/common/events.rad.hpp>

#include <ifw/core/protocol/base/iComm.hpp>
#include <ifw/core/protocol/base/commFactory.hpp>
#include <ifw/core/protocol/base/dispatcher.hpp>
#include <fcf/devmgr/common/deviceFacade.hpp>


#include <scxml4cpp/Log.h>
#include <scxml4cpp/Context.h>
#include <scxml4cpp/EventQueue.h>


#include <boost/asio.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <fmt/format.h>

#include <google/protobuf/stubs/common.h>

#include <memory>

#include "ifw/core/protocol/open62541/taskPoll.hpp"

using namespace fcf::devmgr::common;
using namespace protocol::base;
/**
 * Application main.
 *
 * @param[in] argc Number of command line options.
 * @param[in] argv Command line options.
 */
int main(int argc, char *argv[]) {

    try {
      elt::log::CiiLogManager::Shutdown();
      elt::log::CiiLogManager::SetApplicationName("fcsDevmgr");
      rad::LogInitializer log_initializer;
      protocol::base::Dispatcher<> failure_dispatcher;
      protocol::base::Dispatcher<> normal_dispatcher;
      protocol::opcua::TaskPoll  open62541_service(std::chrono::milliseconds(100));
      // Default logging settings
      elt::log::CiiLogManager::Configure(utils::bat::GetDefaultLogProperties());

        /* Read only configuration */
        fcf::devmgr::common::Config config;
        if (config.ParseOptions(argc, argv) == false) {
            // request for help
            return EXIT_SUCCESS;
        }

        config.LoadConfig();
        LOG4CPLUS_INFO(utils::bat::GetAppLogger(), "Application fcsDevmgr started.");


        // event loop
        boost::asio::io_service io_service;

        // load CII/MAL middleware
        //rad::cii::LoadMiddlewares({"zpb"});
        elt::mal::CiiFactory& factory = elt::mal::CiiFactory::getInstance();
        auto mal = elt::mal::loadMal("zpb", {});
        factory.registerMal("zpb", mal);

        /* Runtime data context */
        fcf::devmgr::common::DataContext data_ctx(config, io_service);


        // SM event queue and context
        scxml4cpp::EventQueue external_events;
        scxml4cpp::Context state_machine_ctx;

        // State Machine facade
        rad::SMAdapter state_machine(io_service,
                                     &state_machine_ctx,
                                     external_events);

        // Register Device Manager Devices
        fcf::devmgr::common::DeviceFacade device_facade(state_machine, config,
                                                  failure_dispatcher,
                                                  normal_dispatcher);

        // Use factory to create devices from the configuration.
        for (auto i = 0; i < config.GetNumDevices(); i++) {
            auto devname = config.GetDeviceName(i);
            auto devfile = config.GetDeviceFile(i);
            auto type = config.GetDeviceType(i);
            auto ifname = config.GetDeviceInterface(i);
            std::string resolved_filename;
            try {
              resolved_filename = rad::Helper::FindFile(devfile);
              auto device = DeviceFactory::Instance().Create(devfile, devname, type, data_ctx);
              auto interface = CommFactory::Instance().Create(resolved_filename, devname, ifname);
              device->CreateObjects();
              device->RegisterComm(interface, failure_dispatcher, normal_dispatcher);
              device_facade.AddDevice(device);
            } catch (std::exception& e) {
              RAD_LOG_ERROR() << "Problems creating device ["
                              << devname << "] using device configuration file: "
                              <<  devfile << " error: " << e.what();
              throw std::runtime_error ("Problems creating device ["
                                        + devname + "] using device configuration file: "
                                        + devfile +  " error: " + e.what());
            }
          }

        // actions and activities
        fcf::devmgr::common::ActionMgr action_mgr;
        action_mgr.CreateActions(io_service, *mal, state_machine, data_ctx, device_facade);
        action_mgr.CreateActivities(state_machine, data_ctx, device_facade);

        // Load SM model
        state_machine.Load(config.GetSmScxmlFilename(), &action_mgr.GetActions(),
                &action_mgr.GetActivities());


        // Register SM Status & Event notification callbacks
        fcf::devmgr::common::ActionsStd* actions_std =
                        dynamic_cast<fcf::devmgr::common::ActionsStd*>(
                                        action_mgr.FindActionGroup("ActionsStd"));
        RAD_ASSERTPTR(actions_std);

        state_machine.AddStatusListener(actions_std);
        state_machine.AddEventListener(actions_std);


        /*
         * Create CII/MAL replier
         */
        rad::cii::Replier mal_replier(elt::mal::Uri(config.GetMsgReplierEndpoint()));

        // Register application specific interface
        mal_replier.RegisterService<fcfif::AsyncAppCmds>("AppCmds",
                        std::make_shared<fcf::devmgr::common::AppCmdsImpl>(state_machine));

        // Register standard interface
        mal_replier.RegisterService<stdif::AsyncStdCmds>("StdCmds",
                                std::make_shared<fcf::devmgr::common::StdCmdsImpl>(state_machine));

        // Register metadaq interface
        mal_replier.RegisterService<metadaqif::AsyncMetaDaq>("MetaDaq",
                                std::make_shared<fcf::devmgr::common::MetaDaqImpl>(state_machine));

        open62541_service.StartThread();
        /*
         * Start event loop
         */
        state_machine.Start();
        

        io_service.run();
        open62541_service.StopThread();
        state_machine.Stop();
        
    } catch (rad::Exception& e) {
        LOG4CPLUS_ERROR(utils::bat::GetAppLogger(), e.what());
        return EXIT_FAILURE;
    } catch (...) {
        LOG4CPLUS_ERROR(utils::bat::GetAppLogger(), boost::current_exception_diagnostic_information());
        return EXIT_FAILURE;
    }

    // to avoid valgrind warnings on potential memory loss
    google::protobuf::ShutdownProtobufLibrary();

   LOG4CPLUS_INFO(utils::bat::GetAppLogger(), "Application terminated.");
    return EXIT_SUCCESS;
}
