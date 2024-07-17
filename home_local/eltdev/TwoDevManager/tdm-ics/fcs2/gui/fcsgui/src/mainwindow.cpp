/**
 * @file
 * @ingroup fcfgui
 * @copyright ESO - European Southern Observatory
 *
 * @brief FCFGUI mainWindow file.
 */

#include <iostream>
#include <boost/chrono.hpp>
#include <boost/chrono/chrono_io.hpp>
#include "ui_mainwindow.h"

#include "ifw/wdglib/ifwled.hpp"

#include <fcf/gui/wdglib/shutter.h>
#include <fcf/gui/wdglib/lamp.h>
#include <fcf/gui/wdglib/motor.h>
#include <fcf/gui/wdglib/actuator.h>
#include <fcf/gui/wdglib/piezo.h>
#include <fcf/gui/wdglib/iodev.h>
#include <fcf/gui/wdglib/drot.h>
#include <fcf/gui/wdglib/adc.h>


#include <fcf/gui/wdglib/cmdLog.h>

#include <fcf/gui/msglib/setupCmdWorker.h>
#include <fcf/gui/msglib/initCmdWorker.h>
#include <fcf/gui/msglib/enableCmdWorker.h>
#include <fcf/gui/msglib/recoverCmdWorker.h>

#include <fcs/gui/fcsgui/mainwindow.h>
#include <fcs/gui/wdglib/mos.h>

#include <fcf/gui/wdglib/placeholderdev.h>
#include <azmq/socket.hpp>
#include <azmq/message.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include <regex>

#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

#include <QTimer>
#include <QTime>
#include <QtWidgets>
#include <QMap>
#include <QSettings>

#include <fcf/devmgr/common/dataContext.hpp>

#include <rad/msgRequestor.hpp>
#include <rad/helper.hpp>

#include <string>
#include <QtDebug>

// trigger the MOC for this class
#if WAF
#include "fcs/gui/fcsgui/mainwindow.moc"
#endif

namespace fcs = fcf::devmgr::common;

//------------------------------------------------------------------------------
//  MainWindow - init static members
//------------------------------------------------------------------------------
const int MainWindow::update_ms {500};
const QString MainWindow::org_name {"ESO"};
const QString MainWindow::app_name {"fcsgui"};

//------------------------------------------------------------------------------
const QString MainWindow::lamp_type      {"Lamp"};
const QString MainWindow::shutter_type   {"Shutter"};
const QString MainWindow::motor_type     {"Motor"};
const QString MainWindow::iodev_type     {"IODev"};
const QString MainWindow::drot_type      {"Drot"};
const QString MainWindow::adc_type       {"Adc"};
const QString MainWindow::piezo_type     {"Piezo"};
const QString MainWindow::actuator_type  {"Actuator"};
const QString MainWindow::smaract_type   {"Smaract"};
const QString MainWindow::mos_type  {"Mos"};

//------------------------------------------------------------------------------
//  MainWindow
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent,
                       const QString uri,
                       int polling_interval)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  // Application UI configuration
  ApplUiSetup();

  // Create MAL interfaces: this need to be first
  CreateMalInterfaces(uri);

  // Obtain the configuration from the server
  GetServerConfiguration();

  // Create redis client
  CreateDbClient();

  // Finetuning of GUI
  CreateDeviceWidgets();

  // Deselect everything
  on_actionDeselect_All_triggered();

  // Connect buttons
  QObject::connect(ui->setupButton, &QPushButton::clicked,
                   this, &MainWindow::handleSetupButton);
  QObject::connect(ui->stopButton, &QPushButton::clicked,
                   this, &MainWindow::handleStopButton);

  // Read settings
  ReadSettings();

  // Start DB update timer
  RAD_LOG_INFO() << "DB Polling interval: " << polling_interval;
  m_timer = new QTimer(this);
  QObject::connect(m_timer, &QTimer::timeout, this, &MainWindow::UpdateFromDb);
  m_timer->start(polling_interval);

  UpdateFromDb();

}

//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  RAD_LOG_TRACE();

  WriteSettings();
  delete ui;
}

//------------------------------------------------------------------------------
void MainWindow::ApplUiSetup() {
    // Setup user interface
    ui->setupUi(this);

    m_server_connection_label = new QLabel(QString("NOT connected!"), this);
    m_led = new IfwLed(ui->statusBar);
    m_led->setColor(Qt::darkGreen);

    ui->statusBar->addPermanentWidget(m_server_connection_label);
    ui->statusBar->addPermanentWidget(m_led);

    ui->addrDockWidget->setTitleBarWidget(new QWidget(ui->addrDockWidget));
    ui->menuView->addAction(ui->attrDockWidget->toggleViewAction());
    ui->menuView->addAction(ui->refDockWidget->toggleViewAction());
    ui->menuView->addAction(ui->posDockWidget2->toggleViewAction());
    ui->menuView->addAction(ui->subDockWidget->toggleViewAction());
    ui->menuView->addAction(ui->logDockWidget_2->toggleViewAction());

    // This list is based on the stylesheets included in the application
    // resources.
    QStringList themes = { "Default", "Combinear", "Diffnes", "Takezo" };
    for (const auto& Theme : themes)
       {
           QAction* a = new QAction(Theme);
           ui->menuThemes->addAction(a);
           QObject::connect(a, &QAction::triggered, this, &MainWindow::onThemeActionTriggered);
       }

}

bool MainWindow::event(QEvent* ev)
{
    // This custom code is needed to allow opening a URL from a
    // Whatsthis information.
    if (ev->type() == QEvent::WhatsThisClicked)
        {
            ev->accept();
            QWhatsThisClickedEvent* whatsThisEvent = dynamic_cast<QWhatsThisClickedEvent*>(ev);
            assert(whatsThisEvent);
            QDesktopServices::openUrl(whatsThisEvent->href());
            return true;
        }
        return QMainWindow::event(ev);
}

//------------------------------------------------------------------------------
void MainWindow::CreateMalInterfaces(const QString uri) {
  // Mal Interfaces
  std::string service_uri = uri.toStdString() + "/StdCmds";
  RAD_LOG_INFO() << "Service URI (std commands): <" << service_uri << ">.";

  std::string service_uri2 = uri.toStdString() + "/AppCmds";
  RAD_LOG_INFO() << "Service URI (app commands):  <" << service_uri2 << ">.";

  std::string mal_type = "zpb";
  m_mal_instance = elt::mal::loadMal(mal_type, m_mal_properties);

  m_mal_uri = elt::mal::Uri(service_uri);
  m_mal_uri2 = elt::mal::Uri(service_uri2);
  std::string scheme = m_mal_uri.scheme().to_string();

  elt::mal::CiiFactory& factory = elt::mal::CiiFactory::getInstance();
  factory.registerMal(scheme, m_mal_instance);
  RAD_LOG_DEBUG() << "Client registered MAL type <" << mal_type << ">.";

  // This is a bit cryptic but is the CII way to specify a timeout
  m_mal_std = factory.getClient<::stdif::StdCmdsSync>(
        m_mal_uri,
  { std::make_shared<elt::mal::rr::qos::ReplyTime>(
    std::chrono::milliseconds(m_config_file.GetReqTout()*1000)) },
        m_mal_properties );

  m_mal_client = factory.getClient<::fcfif::AppCmdsSync>(
        m_mal_uri2,
  { std::make_shared<elt::mal::rr::qos::ReplyTime>(
    std::chrono::milliseconds(m_config_file.GetReqTout()*1000)) },
        m_mal_properties);

  m_connection_listener = m_mal_client->registerConnectionListener([this,uri](bool connected) {
      if (connected) {
          m_server_connection_label->setText(uri);
          m_led->setColor(Qt::darkGreen);
          m_led->setFlashing(false);
      } else {
          m_server_connection_label->setText(QString("NOT connected!"));
          m_led->setColor(Qt::red);
          m_led->setFlashing(true);
      }
  });

  auto connection_future = m_mal_client->asyncConnect();
  auto future_status = connection_future.wait_for(boost::chrono::milliseconds(1000));

  bool connected = (future_status == boost::future_status::ready);
  if (connected) {
      m_server_connection_label->setText(uri);
  }
}

//------------------------------------------------------------------------------
void MainWindow::GetServerConfiguration() {
  // Get config from the server
  // Warning: This is a blocking call.
  Q_CHECK_PTR(m_mal_client);
  auto config_as_string = m_mal_client->GetConfig();
  RAD_LOG_DEBUG() << "server config: " << config_as_string;
  try {
    m_config_file.SetConfigAsString(config_as_string);
  } catch (YAML::Exception& e) {
    RAD_LOG_ERROR() << e.what();
  } catch (elt::configng::CiiConfigBuildError& e) {
    RAD_LOG_ERROR() << e.what();
  } catch (std::exception& e) {
    RAD_LOG_ERROR() << e.what();
  }
}

//------------------------------------------------------------------------------
void MainWindow::CreateDbClient() {
  // Create redis client object

    m_oldb = std::make_unique<utils::bat::DbInterface>(m_config_file.GetOldbPrefix(),
                                                       std::chrono::seconds(m_config_file.GetDbConnTimeout()/1000),
                                                       std::chrono::milliseconds(m_config_file.GetDbTaskPeriod()),
                                                       false);
}

//------------------------------------------------------------------------------
void MainWindow::ReadSettings()
{
  RAD_LOG_TRACE();

  QSettings settings(MainWindow::org_name, MainWindow::app_name);

  settings.beginGroup("MainWindow");
  auto geometry = settings.value("geometry").toByteArray();
  restoreGeometry(geometry);
  auto windows_state = settings.value("windowState").toByteArray();
  restoreState(windows_state);
  auto size = settings.value("size").toSize();
  resize(size);
  auto pos = settings.value("pos").toPoint();
  move(pos);
  settings.endGroup();

}

//------------------------------------------------------------------------------
void MainWindow::WriteSettings() const
{
  RAD_LOG_TRACE();

  QSettings settings(MainWindow::org_name, MainWindow::app_name);

  settings.beginGroup("MainWindow");
  settings.setValue("size", size());
  settings.setValue("pos", pos());
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());
  settings.endGroup();
}

//------------------------------------------------------------------------------
void MainWindow::OnStateCurrentChanged(const StateEnum& state)
{
  // Set all buttons to false
  ui->stateLabel->setProperty("undefined", false);
  ui->substateLabel->setProperty("undefined", false);
  ui->stateLabel->setProperty("notoperational", false);
  ui->substateLabel->setProperty("notoperational", false);
  ui->stateLabel->setProperty("operational", false);
  ui->substateLabel->setProperty("operational", false);

  // Set one of the buttons to true
  if (state == StateEnum::ERROR) {
      ui->stateLabel->setProperty("undefined", true);
      ui->substateLabel->setProperty("undefined", true);
    } else if (state == StateEnum::WARNING) {
      ui->stateLabel->setProperty("notoperational", true);
      ui->substateLabel->setProperty("notoperational", true);
    } else if (state == StateEnum::OK) {
      ui->stateLabel->setProperty("operational", true);
      ui->substateLabel->setProperty("operational", true);
    }

  // Update buttons style
  ui->stateLabel->style()->unpolish(ui->stateLabel);
  ui->stateLabel->style()->polish(ui->stateLabel);
  ui->substateLabel->style()->polish(ui->substateLabel);
}


//------------------------------------------------------------------------------
void MainWindow::updateWindowState()
{
  // Adapt colors of labels to states
  // Colors must be defined in the QSS file

  if (m_state ==
      QString::fromStdString(fcs::SRV_STATE_OPERATIONAL_STR)) {
      if ((m_substate == QString::fromStdString(fcs::SRV_STATE_UNAVAILABLE_STR)) ||
          (m_substate == QString::fromStdString(fcs::SRV_SUBSTATE_ERROR_STR))) {
          OnStateCurrentChanged(StateEnum::ERROR);
        } else {
          OnStateCurrentChanged(StateEnum::OK);
        }
    }  else if (m_state == QString::fromStdString(fcs::SRV_STATE_NOT_OPERATIONAL_STR)) {
      if ((m_substate == QString::fromStdString(fcs::SRV_STATE_UNAVAILABLE_STR)) ||
          (m_substate == QString::fromStdString(fcs::SRV_SUBSTATE_NOTREADY_STR))) {
          OnStateCurrentChanged(StateEnum::ERROR);
        } else {
          OnStateCurrentChanged(StateEnum::WARNING);
        }
    }  else {
      OnStateCurrentChanged(StateEnum::ERROR);
    }


  // Write state names
  ui->stateLabel->setText(m_state);
  ui->substateLabel->setText(m_substate);
}

//------------------------------------------------------------------------------
void MainWindow::CreateDeviceWidgets()
{
  RAD_LOG_TRACE();

  long replyTimeout = m_config_file.GetReqTout();
  std::string req_endpoint = m_config_file.GetMsgReplierEndpoint();
  std::string server_id    = m_config_file.GetServerId();

  RAD_LOG_INFO() << "req_endpoint: " << req_endpoint;

  QVBoxLayout* devices_layout = nullptr;
  QVBoxLayout* sensors_layout = nullptr;
  QVBoxLayout* tracking_layout = nullptr;

  QWidget *devices_object = this->findChild<QWidget *>("devicesContainer");
  QWidget *sensors_object = this->findChild<QWidget *>("sensorsContainer");
  QWidget *tracking_object = this->findChild<QWidget *>("trackingContainer");
  Q_ASSERT_X(devices_object, "Could not find object","devicesContainer");
  Q_ASSERT_X(sensors_object, "Could not find object","sensorsContainer");
  Q_ASSERT_X(tracking_object, "Could not find object","trackingContainer");

  devices_layout =  qobject_cast<QVBoxLayout*>(devices_object->layout());
  sensors_layout =  qobject_cast<QVBoxLayout*>(sensors_object->layout());
  tracking_layout =  qobject_cast<QVBoxLayout*>(tracking_object->layout());
  Q_ASSERT_X(devices_layout, "Device object layout", "Devices Layout is nullptr");
  Q_ASSERT_X(sensors_layout, "Device object layout", "Devices Layout is nullptr");
  Q_ASSERT_X(tracking_layout, "Device object layout", "Devices Layout is nullptr");

  // Addd widgets or playeholders
  for (int i=0; i < m_config_file.GetNumDevices(); i++) {
      std::string devname = m_config_file.GetDeviceName(i);
      RAD_LOG_DEBUG() << "device name: " << devname;

      // Get layout to add the new device widget to
      QWidget* devWdg = nullptr;
      auto devtype = m_config_file.GetDeviceType(i);
      RAD_LOG_DEBUG() << "device type: " << devtype;
      if (devtype.empty()) {
          RAD_LOG_INFO() << "Could not get device type for " << devname
                         << " from config, insert placeholder";
          devWdg = new PlaceholderDevWdg(QString::fromStdString(devname),
                                         "Not found in configuration",
                                         nullptr);
          devWdg->setEnabled(false);
        } else {
          devWdg = AddDeviceWidget(devname,
                                   devtype,
                                   server_id,
                                   replyTimeout);
        }
      if ( !devWdg ) {
          RAD_LOG_ERROR() << "Could not create widget - skip device " << devname;
          continue;
        }

      // Append widget to layout
      // (Layout takes ownership of widget
      // so no need to worry about cleaning up)
      devWdg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
      QString devTypeStr = QString::fromStdString(devtype);
      if (QString::compare(devTypeStr, MainWindow::iodev_type, Qt::CaseInsensitive) == 0) {
          sensors_layout->addWidget(devWdg);
        } else if ((QString::compare(devTypeStr, MainWindow::drot_type, Qt::CaseInsensitive) == 0) ||
                   (QString::compare(devTypeStr, MainWindow::adc_type, Qt::CaseInsensitive) == 0)) {
          tracking_layout->addWidget(devWdg);
        } else {
           devices_layout->addWidget(devWdg);
        }

      devWdg->show();
      RAD_LOG_DEBUG() << "Added widget for device " << devname
                      << " to GUI (page = " << MainWindow::Page::Devices << ")";

      DeviceWdg* wdg = qobject_cast<DeviceWdg*>(devWdg);
      if (wdg) {
          m_wdgs.push_back(WdgInfo(devname, MainWindow::Page::Devices, wdg));
        }
    }

  // Append spacer items to layouts to shift all widgets to the top
  /*
  auto layout_devices = qobject_cast<QBoxLayout*>(myobject->layout());
  if (layout_devices)
    layout_devices->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
  */
qobject_cast<QBoxLayout*>(sensors_layout->layout())
    ->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
qobject_cast<QBoxLayout*>(tracking_layout->layout())
    ->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
qobject_cast<QBoxLayout*>(devices_layout->layout())
    ->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

}

//------------------------------------------------------------------------------
AbstractDevice *MainWindow::AddDeviceWidget(const std::string& devname,
                                            const std::string &devtype,
                                            const std::string &server_id,
                                            int cmd_timeout)
{
  RAD_LOG_TRACE();

  using namespace fcf::devmgr;

  AbstractDevice* wdg = nullptr;
  QString devTypeStr = QString::fromStdString(devtype);

  if (QString::compare(devTypeStr, MainWindow::shutter_type, Qt::CaseInsensitive) == 0) {
      std::string config_data = m_mal_client->DevConfig(devname);
      wdg = new ShutterWdg(this,
                           m_oldb,
                           devname,
                           config_data,
                           cmd_timeout);

    } else if (QString::compare(devTypeStr, MainWindow::actuator_type, Qt::CaseInsensitive) == 0) {
      std::string config_data = m_mal_client->DevConfig(devname);
      wdg = new ActuatorWdg(this,
                            m_oldb,
                            devname,
                            config_data,
                            cmd_timeout);
    } else if (QString::compare(devTypeStr, MainWindow::lamp_type, Qt::CaseInsensitive) == 0) {
      std::string config_data = m_mal_client->DevConfig(devname);
      wdg = new LampWdg(this,
                        m_oldb,
                        devname,
                        config_data,
                        cmd_timeout);
    } else if (QString::compare(devTypeStr, MainWindow::piezo_type, Qt::CaseInsensitive) == 0) {
      std::string config_data = m_mal_client->DevConfig(devname);
      wdg = new PiezoWdg(this,
                         m_oldb,
                         devname,
                         config_data,
                         cmd_timeout);
    } else if (QString::compare(devTypeStr, MainWindow::drot_type, Qt::CaseInsensitive) == 0) {
      std::string config_data = m_mal_client->DevConfig(devname);
      fcf::devmgr::motor::MotorConfig config(devname);
      config.InitFromString(config_data);
      config.Init();
      wdg = new DrotWdg(this,
                        m_oldb,
                        devname,
                        config,
                        server_id,
                        cmd_timeout);

    } else if (QString::compare(devTypeStr, MainWindow::adc_type, Qt::CaseInsensitive) == 0) {
      std::string config_data = m_mal_client->DevConfig(devname);
      fcf::devmgr::motor::MotorConfig config(devname);
      config.InitFromString(config_data);
      config.Init();
      wdg = new DrotWdg(this,
                        m_oldb,
                        devname,
                        config,
                        server_id,
                        cmd_timeout);

    } else if ((QString::compare(devTypeStr, MainWindow::motor_type, Qt::CaseInsensitive) == 0) ||
               (QString::compare(devTypeStr, MainWindow::smaract_type, Qt::CaseInsensitive) == 0)) {
          RAD_LOG_DEBUG() << "Getting motor configuration ";
         std::string config_data = m_mal_client->DevConfig(devname);
          RAD_LOG_DEBUG() << "Creating motor widget ";
         wdg = new MotorWdg(this,
                            m_oldb,
                            devname,
                            config_data,
                            server_id,
                            cmd_timeout);
  } else if (QString::compare(devTypeStr, MainWindow::iodev_type, Qt::CaseInsensitive) == 0) {
      std::string config_data = m_mal_client->DevConfig(devname);
      fcf::devmgr::iodev::IODevConfig config(devname);
      config.InitFromString(config_data);
      config.Init();
      wdg = new IODevWdg(this,
                         m_oldb,
                         devname,
                         config,
                         cmd_timeout);
  } else if (QString::compare(devTypeStr, MainWindow::mos_type, Qt::CaseInsensitive) == 0) {
      std::string config_data = m_mal_client->DevConfig(devname);
      wdg = new MosWdg(this, m_oldb, devname, config_data, cmd_timeout);
  }

  return wdg;
}

//------------------------------------------------------------------------------
std::vector<std::string> MainWindow::selectedDevices(MainWindow::Page page) const
{
  RAD_LOG_TRACE();

  std::vector<std::string> vec;

  for (const auto& tup: m_wdgs)
    {
      auto name = std::get<0>(tup);
      auto wdg_page = std::get<1>(tup);
      auto wdg = std::get<2>(tup);

      if ( page == MainWindow::All || page == wdg_page)
        {
          if ( wdg->IsSelected() )
            vec.push_back(name);
        }
    }
  return vec;
}

//------------------------------------------------------------------------------
void MainWindow::SetFcsLogLevel(const std::string& level)
{
  Q_UNUSED(level);

  RAD_LOG_TRACE();
  OverrideCursor cursor {Qt::WaitCursor};
  try
  {
    addLogEntry("Request", "SetLogLevel", QString::fromStdString(level));

    auto logInfo = m_mal_instance->createDataEntity<::stdif::LogInfo>();
    logInfo->setLevel(level);
    logInfo->setLogger("app");
    auto reply = m_mal_std->SetLogLevel(logInfo);

    addLogEntry("Reply", "SetLogLevel", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "SetLogLevel", QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "SetLogLevel", QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "SetLogLevel", QString::fromStdString("Exception occurred: unknown error"));
  }
}

//------------------------------------------------------------------------------
void MainWindow::UpdateFromDb()
{
  //----
  // Read data from OLDB and update widgets
  for (auto tup: m_wdgs)
    {
      AbstractDevice* wdg = std::get<2>(tup);
       wdg->UpdateData();
    }

  //----
  // Get state info from OLDB
  std::string db_state;
  std::string db_substate;

  m_oldb->Get(std::string("stat/states/state"), db_state);
  m_state =   QString::fromStdString(db_state);
  m_oldb->Get(std::string("stat/states/substate"), db_substate);
  m_substate = QString::fromStdString(db_substate);

  // Update GUI according to state info
  updateWindowState();
}

//------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
  Q_UNUSED(event)

}

//------------------------------------------------------------------------------
void MainWindow::on_actionInit_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  InitCmdWorker* worker = new InitCmdWorker(m_mal_uri,
                                            m_config_file.GetReqTout(),
                                            m_mal_properties);
  StartInThread(worker);
}

//------------------------------------------------------------------------------
void MainWindow::on_actionExit_triggered()
{
  RAD_LOG_INFO() << "Action Exit !";

  QApplication::quit();
}

//------------------------------------------------------------------------------
void MainWindow::on_actionEnable_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  EnableCmdWorker* worker = new EnableCmdWorker(m_mal_uri,
                                                m_config_file.GetReqTout(),
                                                m_mal_properties);
  StartInThread(worker);
}

//------------------------------------------------------------------------------
void MainWindow::on_actionDisable_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  try
  {
    addLogEntry("Request", "DISABLE", QString::fromStdString(""));
    auto reply = m_mal_std->Disable();
    addLogEntry("Reply", "DISABLE", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "DISABLE", QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "DISABLE", QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "DISABLE", QString::fromStdString("Exception occurred: unknown error"));
  }
}

//------------------------------------------------------------------------------
void MainWindow::on_actionRecover_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  RecoverCmdWorker* worker = new RecoverCmdWorker(m_mal_uri2,
                                                  m_config_file.GetReqTout(),
                                                  m_mal_properties);
  StartInThread(worker);
}

//------------------------------------------------------------------------------
void MainWindow::on_actionReset_triggered()
{
  RAD_LOG_TRACE();
  try
  {
    addLogEntry("Request", "RESET", QString::fromStdString(""));
    auto reply = m_mal_std->Reset();
    addLogEntry("Reply", "RESET", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "RESET", QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "RESET", QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "RESET", QString::fromStdString("Exception occurred: unknown error"));
  }
}

//------------------------------------------------------------------------------
void MainWindow::on_actionSelect_All_triggered()
{
  RAD_LOG_INFO() << "Select all:" << m_wdgs.size()  << " devices";
  for (auto tup : m_wdgs)
    {
      AbstractDevice* wdg = std::get<2>(tup);
      wdg->Select();
    }
}

//------------------------------------------------------------------------------
void MainWindow::on_actionDeselect_All_triggered() {
  RAD_LOG_INFO() << "De-select all:" << m_wdgs.size()  << " devices";
  for (auto tup : m_wdgs)
    {
      AbstractDevice* wdg = std::get<2>(tup);
      wdg->Deselect();
    }
}


//------------------------------------------------------------------------------
void MainWindow::on_actionDevReset_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  std::vector<std::string> selected = selectedDevices(MainWindow::All);
  try
  {
    addLogEntry("Request", "HWRESET", QString::fromStdString(""));
    auto reply = m_mal_client->HwReset(selected);
    addLogEntry("Reply", "HWRESET", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "HWRESET", QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "HWRESET", QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "HWRESET",
                QString::fromStdString("Exception occurred: unknown error"));
  }
}

//-----------------
void MainWindow::on_actionDevInit_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  std::vector<std::string> selected = selectedDevices(MainWindow::All);
  try
  {
    addLogEntry("Request", "HWINIT", QString::fromStdString(""));
    auto reply = m_mal_client->HwInit(selected);
    addLogEntry("Reply", "HWINIT", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "HWINIT", QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "HWINIT", QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "HWINIT",
                QString::fromStdString("Exception occurred: unknown error"));
  }
}

//-----------------
void MainWindow::on_actionDevEnable_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  std::vector<std::string> selected = selectedDevices(MainWindow::All);
  try
  {
    addLogEntry("Request", "HWENABLE", QString::fromStdString(""));
    auto reply = m_mal_client->HwEnable(selected);
    addLogEntry("Reply", "HWENABLE", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "HWENABLE", QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "HWENABLE", QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "HWENABLE",
                QString::fromStdString("Exception occurred: unknown error"));
  }
}

//-----------------
void MainWindow::on_actionDevDisable_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  std::vector<std::string> selected = selectedDevices(MainWindow::All);
  try
  {
    addLogEntry("Request", "HWDISABLE", QString::fromStdString(""));
    auto reply = m_mal_client->HwDisable(selected);
    addLogEntry("Reply", "HWDISABLE", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "HWDISABLE", QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "HWDISABLE", QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "HWDISABLE",
                QString::fromStdString("Exception occurred: unknown error"));
  }
}

//------------------------------------------------------------------------------
void MainWindow::on_actionDevSimulate_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  std::vector<std::string> selected = selectedDevices(MainWindow::All);
  try
  {
    addLogEntry("Request", "SIMULATE", QString());
    auto reply = m_mal_client->Simulate(selected);
    addLogEntry("Reply", "SIMULATE", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "SIMULATE", QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "SIMULATE", QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "SIMULATE",
                QString::fromStdString("Exception occurred: unknown error"));
  }
}

//------------------------------------------------------------------------------
void MainWindow::on_actionDevStopSim_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  std::vector<std::string> selected = selectedDevices(MainWindow::All);
  try
  {
    addLogEntry("Request", "STOPSIM", QString());
    auto reply = m_mal_client->StopSim(selected);
    addLogEntry("Reply", "STOPSIM", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "STOPSIM",
                QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "STOPSIM",
                QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "STOPSIM",
                QString::fromStdString("Exception occurred: unknown error"));
  }
}

//------------------------------------------------------------------------------
void MainWindow::on_actionDevIgnore_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  std::vector<std::string> selected = selectedDevices(MainWindow::All);
  try
  {
    addLogEntry("Request", "IGNORE", QString());
    auto reply = m_mal_client->Ignore(selected);
    addLogEntry("Reply", "IGNORE", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "IGNORE",
                QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "IGNORE",
                QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "IGNORE",
                QString::fromStdString("Exception occurred: unknown error"));
  }
}

//------------------------------------------------------------------------------
void MainWindow::on_actionDevStopIgn_triggered()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  std::vector<std::string> selected = selectedDevices(MainWindow::All);
  try
  {
    addLogEntry("Request", "STOPIGN", QString());
    auto reply = m_mal_client->StopIgn(selected);
    addLogEntry("Reply", "STOPIGN", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "STOPIGN",
                QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "STOPIGN",
                QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "STOPIGN",
                QString::fromStdString("Exception occurred: unknown error"));
  }
}

//------------------------------------------------------------------------------
void MainWindow::on_actionFCF_CLI_triggered()
{
    RAD_LOG_TRACE();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString command{"xfce4-terminal"};
    QString endpoint = QString::fromStdString(m_config_file.GetMsgReplierEndpoint());
    QProcess process;
    process.setProgram(command);

    QStringList params {"-e",
                        "fcfcli --uri " + endpoint
                        + " --timeout " +
                        QString::fromStdString(std::to_string(m_config_file.GetReqTout()))};

    process.setArguments(params);
    RAD_LOG_INFO() << "Command: " << command.toStdString() <<
                      " " << params.join(" ").toStdString();


    qint64 pid;
    process.startDetached(&pid);
    process.waitForStarted();

    QApplication::restoreOverrideCursor();

}

//------------------------------------------------------------------------------
void MainWindow::on_actionWhatsthis_triggered()
{
  RAD_LOG_TRACE();
 QWhatsThis::enterWhatsThisMode();
}

//------------------------------------------------------------------------------
void MainWindow::on_actionAbout_triggered()
{
  RAD_LOG_TRACE();
  QMessageBox messageBox;

  messageBox.setIconPixmap(QPixmap(":/images/eso-logo-color.png"));


  QString line2 = tr("<b> Function Control System (FCS)</b><br>Version: %1").
                   arg(QString::fromStdString(m_config_file.GetVersion()));

  messageBox.setText(line2);

  messageBox.setWindowTitle("FCS GUI About Dialog");
  messageBox.exec();


}
//------------------------------------------------------------------------------
void MainWindow::handleSetupButton()
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  try
  {
    auto mal = m_mal_client->getMal();

    // Fill setup_buffer with info from selected devices
    std::vector<std::shared_ptr<::fcfif::SetupElem>> setup_buffer;
    for (auto tup : m_wdgs)
      {
        AbstractDevice* wdg = std::get<2>(tup);
        wdg->GetMessage(mal, setup_buffer);
      }

    // Trigger the setup in separate thread
    SetupCmdWorker* worker = new SetupCmdWorker(m_mal_uri2,
                                                m_config_file.GetReqTout(),
                                                m_mal_properties,
                                                setup_buffer);
    StartInThread(worker);
  }
  catch (std::exception& e)
  {
    RAD_LOG_ERROR() << "Exception occurred:"  << e.what();
  }
  catch (...)
  {
    RAD_LOG_ERROR() << "Exception occurred:";
  }
}

//------------------------------------------------------------------------------
void MainWindow::handleDeviceSetup(QString name)
{
  RAD_LOG_TRACE();

  OverrideCursor cursor {Qt::WaitCursor};
  try
  {
    // Search widget for device
    auto it = find_if( m_wdgs.begin(), m_wdgs.end(),
                       [&name](const auto& tup) {return QString::fromStdString(std::get<0>(tup)) == name;} );
    if  (it == m_wdgs.end())
      {
        RAD_LOG_WARNING() << "Did not find widget for device " << name.toStdString();
        return;
      }

    // Fill setup_buffer
    auto mal = m_mal_client->getMal();
    std::vector<std::shared_ptr<::fcfif::SetupElem>> setup_buffer;

    AbstractDevice* wdg = std::get<2>(*it);
    wdg->GetMessage(mal, setup_buffer, true);

    // Trigger setup in separate thread
    SetupCmdWorker* worker = new SetupCmdWorker(m_mal_uri2,
                                                m_config_file.GetReqTout(),
                                                m_mal_properties,
                                                setup_buffer);// In the future we might have an handler which subscribes to
    // alarm notifications. It could then call this method
    // to make the alarm info available in the GUI
    StartInThread(worker);
  }
  catch (std::exception& e)
  {
    RAD_LOG_ERROR() << "Exception occurred:"  << e.what();
  }
  catch (...)
  {
    RAD_LOG_ERROR() << "Exception occurred" ;
  }
}

//------------------------------------------------------------------------------
void MainWindow::handleStopButton()
{
  RAD_LOG_TRACE();

  for (auto tup : m_wdgs)
    {
      AbstractDevice* wdg = std::get<2>(tup);
      wdg->Stop();
    }

  try
  {
    addLogEntry("Request", "STOP", QString());
    auto reply = m_mal_std->Stop();
    addLogEntry("Reply", "STOP", QString::fromStdString(reply));
  }
  catch (const fcfif::ExceptionErr& e)
  {
    addLogEntry("Reply", "STOP", QString::fromStdString(e.getDesc()));
  }
  catch (const std::exception& e)
  {
    addLogEntry("Reply", "STOP", QString::fromStdString(e.what()));
  }
  catch (...)
  {
    addLogEntry("Reply", "STOP", QString::fromStdString("Exception occurred: unknown error"));
  }
}

//------------------------------------------------------------------------------
void MainWindow::addLogEntry(const QString& type,
                             const QString& cmd,
                             const QString& text)
{
  ui->cmdLog->addEntry("", type, cmd, text);
}

void MainWindow::onThemeActionTriggered() {
    auto Action = qobject_cast<QAction*>(sender());

    QString style;
    QString fileName;

    RAD_LOG_INFO() << "theme: " << Action->text().toStdString() ;
    fileName = QString(":/styles/%1/%2.qss").arg(Action->text(), Action->text());


    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)) {
        //parser.showHelp();
    } else {
        style = QLatin1String(file.readAll());
    }
    file.close();
    qApp->setStyleSheet( style );
}
