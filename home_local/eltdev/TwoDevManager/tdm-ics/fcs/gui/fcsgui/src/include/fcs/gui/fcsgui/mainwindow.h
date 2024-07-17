/**
 * @file
 * @ingroup fcfgui
 * @copyright ESO - European Southern Observatory
 * @brief MainWindow include file for FCF GUI.
 */

#ifndef FCF_GUI_MAINWINDOW_HPP_
#define FCF_GUI_MAINWINDOW_HPP_

#include <any>
#include <QMainWindow>
#include <QTimer>
#include <QMap>
#include <QPair>
#include "fcf/gui/msglib/subscribeCmdWorker.h"
#include "fcf/devmgr/common/config.hpp"
#include "fcf/gui/wdglib/abstractDevice.h"
#include "fcf/gui/wdglib/device.h"
#include <Fcfif.hpp>
#include <Stdif.hpp>

#include <mal/Cii.hpp>
#include <mal/Mal.hpp>
#include <mal/utility/LoadMal.hpp>
#include <mal/rr/qos/ReplyTime.hpp>


#include <yaml-cpp/yaml.h>

#include <utils/bat/dbInterface.hpp>


#include <QCloseEvent>
#include <QApplication>
#include <QThread>

class QGroupBox;
class QLabel;
class QTextEdit;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QListWidget;
class QString;
class IfwLed;

namespace Ui
{
    class MainWindow;
}

namespace fcf::devmgr::common
{
    class DeviceConfig;
}

//---------------------------------------------------------------------------------
//  MainWindow
//---------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:

        // Enumaration for pages in tab widget
        enum Page
        {
            Devices,
            Sensors,
            All
        };

        // Enumaration for dynamic state style
        enum StateEnum
        {
            ERROR,
            WARNING,
            OK,
        };

        // Helper class
        struct OverrideCursor
        {
            OverrideCursor(Qt::CursorShape shape = Qt::WaitCursor)
            {
                QApplication::setOverrideCursor(shape);
            }

            ~OverrideCursor()
            {
                QApplication::restoreOverrideCursor();
            }
        };

    public:
        MainWindow(QWidget *parent,
                   const QString uri,
                   int polling_interval);
        virtual ~MainWindow() override;

        void closeEvent(QCloseEvent *event) override;
        bool event(QEvent* ev) override;

    private slots:
        // NOTE:
        // This is to explain the naming conventions of the slots methods.
        // They do not follow ESO's naming convention to enable the automatic
        // connection done by Qt for action signals following Qt naming convention.

        // Updates triggered by timer
        void UpdateFromDb();

        // File menu
        void on_actionExit_triggered();

        // FCS menu
        void on_actionInit_triggered();
        void on_actionEnable_triggered();
        void on_actionDisable_triggered();
        void on_actionRecover_triggered();
        void on_actionReset_triggered();
        void on_actionLogError_triggered()  { SetFcsLogLevel("ERROR"); };
        void on_actionLogInfo_triggered()   { SetFcsLogLevel("INFO"); };
        void on_actionLogDebug_triggered()  { SetFcsLogLevel("DEBUG"); };
        void on_actionLogTrace_triggered()  { SetFcsLogLevel("TRACE"); };

        // Device menu
        void on_actionSelect_All_triggered();
        void on_actionDeselect_All_triggered();
        void on_actionDevReset_triggered();
        void on_actionDevInit_triggered();
        void on_actionDevEnable_triggered();
        void on_actionDevDisable_triggered();
        void on_actionDevSimulate_triggered();
        void on_actionDevStopSim_triggered();
        void on_actionDevIgnore_triggered();
        void on_actionDevStopIgn_triggered();

        // Tools menu
        void on_actionFCF_CLI_triggered();

        // Them menu
        void onThemeActionTriggered();

        // Help menu
        void on_actionAbout_triggered();
        void on_actionWhatsthis_triggered();

        // Other
        void handleSetupButton();
        void handleDeviceSetup(QString name);
        void handleStopButton();
        void addLogEntry(const QString& type,
                         const QString& cmd,
                         const QString& text);
        void updateWindowState();

    private:
        void ApplUiSetup();
        void CreateMalInterfaces(const QString uri);
        void GetServerConfiguration();
        void CreateDbClient();
        void CreateDeviceWidgets();
        AbstractDevice* AddDeviceWidget(const std::string &devname,
                                     const std::string& devtype,
                                     const std::string& server_id,
                                     int cmd_timeout);
        std::vector<std::string> selectedDevices(MainWindow::Page page = MainWindow::All) const;
        void SetFcsLogLevel(const std::string &level);

        template <class WorkerType>
        void StartInThread(WorkerType* worker)
        {
            QThread* thread = new QThread;
            worker->moveToThread(thread);
            QObject::connect(thread, &QThread::started, worker, &WorkerType::process);
            QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            QObject::connect(worker, &WorkerType::finished, thread, &QThread::quit);
            QObject::connect(worker, &WorkerType::finished, worker, &WorkerType::deleteLater);
            QObject::connect(worker, &WorkerType::LogReplyOut,
                             this, &MainWindow::addLogEntry,
                             Qt::QueuedConnection);
            thread->start();
        }

        void ReadSettings();
        void WriteSettings() const;

         // Dynamic style change
        void OnStateCurrentChanged(const StateEnum& state);

    private:
        static const QString org_name;
        static const QString app_name;
        static const int update_ms;
        static const QString lamp_type;
        static const QString shutter_type;
        static const QString motor_type;
        static const QString iodev_type;
        static const QString drot_type;
        static const QString adc_type;
        static const QString piezo_type;
        static const QString actuator_type;
        static const QString smaract_type;
        static const QString mos_type;

        using WdgInfo = std::tuple<std::string, MainWindow::Page, AbstractDevice*>; // <devname, page, Pointer to widget>
        using WdgInfoVec = std::vector<WdgInfo>;
        WdgInfoVec m_wdgs;

        Ui::MainWindow*                        ui;
        QLabel*                                m_server_connection_label;
        IfwLed*                                m_led;
        QTimer*                                m_timer;
        QString                                m_state;
        QString                                m_substate;
        fcf::devmgr::common::Config            m_config_file;
        std::unique_ptr<utils::bat::DbInterface> m_oldb;
        std::shared_ptr<::elt::mal::Mal>       m_mal_instance;
        std::shared_ptr<::stdif::StdCmdsSync>  m_mal_std;
        std::shared_ptr<::fcfif::AppCmdsSync>  m_mal_client;
        elt::mal::Mal::Properties              m_mal_properties;
        elt::mal::Uri                          m_mal_uri;
        elt::mal::Uri                          m_mal_uri2;
        elt::mal::rr::ListenerRegistration     m_connection_listener;
};

#endif // FCF_GUI_MAINWINDOW_HPP_
