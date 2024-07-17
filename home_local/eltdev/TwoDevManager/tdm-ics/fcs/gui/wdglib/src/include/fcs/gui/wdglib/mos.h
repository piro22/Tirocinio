/**
 * @file
 * @ingroup gui_wdglib
 * @copyright ESO - European Southern Observatory
 *
 * @brief WDGLIB MosWdg class declaration file.
 */

#ifndef Mos_WDG_H
#define Mos_WDG_H

#include "fcf/gui/wdglib/device.h"

#include <QWidget>
#include <QObject>
#include <azmq/socket.hpp>
#include <azmq/message.hpp>
#include <rad/mal/subscriber.hpp>
#include <rad/mal/utils.hpp>

class QLabel;
class QCheckBox;
class QComboBox;
class QSpinBox;
class QPalette;


namespace Ui {
    class MosWdg;
}

class MosWdg : public DeviceWdg
{
  Q_OBJECT

public:
  explicit MosWdg(QWidget *parent,
                      std::unique_ptr<utils::bat::DbInterface>& dbinterface,
                      const std::string name,
                      const std::string& config,
                      const int& cmd_timeout = 60000);
  virtual ~MosWdg();

  virtual void StartSubscription();

  void GetMessage(std::shared_ptr<::elt::mal::Mal> mal,
                  std::vector<std::shared_ptr<::fcfif::SetupElem>>& setup_buffer,
                  bool ignoreChecked = false);
  void SetMessage(const azmq::message_vector& msg);
  virtual void UpdateData();
  virtual void GetDbAttributes();
  void SetEnable(bool flag);

signals:


public slots:
  void UpdateSubstate(int substate);
  void UpdateSpinner(int substate);



private:
  Ui::MosWdg   *ui;


};

#endif // Mos_WDG_H
