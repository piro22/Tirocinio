/**
 * @file
 * @ingroup gui_wdglib
 * @copyright ESO - European Southern Observatory
 *
 * @brief WDGLIB MosWdg class implementation file.
 */

#include <iostream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>

#include <QtWidgets>
#include "ui_mosWdg.h"
#include "fcs/devices/mosConfig.hpp"
#include "fcs/gui/wdglib/mos.h"
#include "fcf/devmgr/devices/shutterConfig.hpp"
#include <rad/logger.hpp>

#if WAF
#include "fcs/gui/wdglib/mos.moc"
#endif

MosWdg::MosWdg(QWidget *parent,
                       std::unique_ptr<utils::bat::DbInterface>& dbinterface,
                       const std::string name,
                       const std::string& config,
                       const int& cmd_timeout):
  DeviceWdg(parent, dbinterface, name, cmd_timeout),
  ui(new Ui::MosWdg) {
  ui->setupUi(this);
  m_ui.SetName(ui->nameLabel);
  m_ui.SetEnable(ui->enableCheckBox);
  m_ui.SetSpinner(ui->activitySpinner);
  m_ui.SetState(ui->stateLabel);
  m_ui.SetSubstate(ui->substateLabel);
  m_ui.SetSim(ui->simLed);
  m_ui.SetLocal(ui->localLed);
  m_ui.SetError(ui->errorLed);

  DeviceWdg::SetupCommonUi();

  auto config_node = YAML::Load(config);
  std::string  overview("");
  std::string  description("");

  if (config_node[name][fcf::devmgr::common::CI_OVERVIEW])
      overview = config_node[name][fcf::devmgr::common::CI_OVERVIEW].as<std::string>();
  if (config_node[name][fcf::devmgr::common::CI_DESCRIPTION])
      description = config_node[name][fcf::devmgr::common::CI_DESCRIPTION].as<std::string>();

  DeviceWdg::SetupInfoUi(overview, description);

  connect( ui->optionsComboBox, SIGNAL( currentIndexChanged(int) ),
           this, SLOT( SetActive(int)) );

  // Connect button signal with class method that will emit a
  // new signal for the parent class.
  connect(ui->setupButton, SIGNAL (released()), this,
          SLOT (HandleSetupButton()));

  // Used to trigger the setup command by the parent class
  connect(this, SIGNAL (TriggerSetup(QString)), parent,
          SLOT (handleDeviceSetup(QString)));

  m_error_tooltip = m_ui.GetError()->toolTip();

  GetDbAttributes();

}

MosWdg::~MosWdg() {
  delete ui;
}

void MosWdg::SetEnable(bool flag)
{
  DeviceWdg::SetEnable(flag);

  SetProperty(ui->optionsComboBox,std::string("disabled"), !flag);
  SetProperty(ui->setupButton,std::string("disabled"), !flag);
  ui->optionsComboBox->setEnabled( flag );
  ui->setupButton->setEnabled( flag );


}

void MosWdg::GetMessage(std::shared_ptr<::elt::mal::Mal> mal,
                            std::vector<std::shared_ptr<::fcfif::SetupElem>>& setup_buffer,
                            bool ignoreChecked) {
  if (ignoreChecked || ui->enableCheckBox->isChecked())
    {

      auto custom = mal->createDataEntity<::fcfif::CustomDevice>();

      const std::string params = "{\"tip\": " +
          std::to_string(ui->tip->value()) + "," +
          "\"tilt\": " +
          std::to_string(ui->tilt->value()) + "," +
          "\"piston\": " +
          std::to_string(ui->piston->value()) + "," +
          "\"action\": \"" + ui->optionsComboBox->currentText().toStdString() + "\"}";

      custom->setParameters(params);
      auto container = mal->createDataEntity<::fcfif::SetupElem>();
      auto device = container->getDevice();
      container->setId(GetName());
      device->setCustom(custom);
      setup_buffer.push_back(container);

    }
}

void MosWdg::UpdateData() {
    // Update the common UI in the parent class first
    DeviceWdg::UpdateData();

    for (unsigned i=0; i<m_dbattribute_vector.size() ; i++) {
        std::vector<std::string> path_attr;
        boost::split(path_attr, m_dbattribute_vector[i], boost::is_any_of("/"), boost::token_compress_on);

        if (path_attr.size() > 0) {
            std::string attr = path_attr[path_attr.size()-1];
            RAD_LOG_DEBUG() << "attr: " <<  m_dbattribute_vector[i];
            if (attr == fcf::devmgr::common::CI_STAT_SUBSTATE) {
                int substate=0;
                m_dbinterface->Get(m_dbattribute_vector[i], substate);
                UpdateSubstate(substate);
                UpdateSpinner(substate);
            }
        }
    }

}

void MosWdg::UpdateSubstate(int substate) {
  bool updated = DeviceWdg::UpdateSubstate(substate);
  if (! updated) {
      switch (substate) {
        case fcs::mos::SUBSTATE_OP_IDLE:
          ui->substateLabel->setText(QString::fromStdString(fcs::mos::SUBSTATE_OP_IDLE_STR));
          break;
        default:
          ui->substateLabel->setText(QString::fromStdString(fcf::devmgr::common::STATE_UNAVAILABLE_STR));
          break;
        }
    }
}

void MosWdg::UpdateSpinner(int substate) {
  DeviceWdg::UpdateSpinner(substate);

  switch (substate) {
    case fcs::mos::SUBSTATE_OP_IDLE:
      ui->activitySpinner->stop();
      break;
    }
}

 void MosWdg::GetDbAttributes() {


   DeviceWdg::GetDbAttributes();


 }



 void MosWdg::StartSubscription() {

 }


