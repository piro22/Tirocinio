/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <fcf/gui/wdglib/cmdLog.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionInit;
    QAction *actionEnable;
    QAction *actionDisable;
    QAction *actionSelect_All;
    QAction *actionDeselect_All;
    QAction *actionRecover;
    QAction *actionDevReset;
    QAction *actionDevInit;
    QAction *actionDevEnable;
    QAction *actionDevDisable;
    QAction *actionReset;
    QAction *actionDevSimulate;
    QAction *actionDevStopSim;
    QAction *actionDevIgnore;
    QAction *actionDevStopIgn;
    QAction *actionLogError;
    QAction *actionLogInfo;
    QAction *actionLogDebug;
    QAction *actionLogDebug2;
    QAction *actionLogDebug3;
    QAction *actionLogTrace;
    QAction *actionSelectAllCurrent;
    QAction *actionDeselectAllCurrent;
    QAction *actionLoad_Layout;
    QAction *actionSave_Layout;
    QAction *actionReset_Layout;
    QAction *actionFCF_CLI;
    QAction *actionAbout;
    QAction *actionWhatsthis;
    QWidget *centralWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuFCS;
    QMenu *menuSet_log_level;
    QMenu *menuDevices;
    QMenu *menuHelp;
    QMenu *menuTools;
    QMenu *menuView;
    QMenu *menuThemes;
    QStatusBar *statusBar;
    QDockWidget *attrDockWidget;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    QScrollArea *devicesScrollArea;
    QWidget *devicesContainer;
    QVBoxLayout *verticalLayout_3;
    QDockWidget *addrDockWidget;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout;
    QPushButton *setupButton;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *serverLabel;
    QLabel *stateLabel;
    QLabel *substateLabel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *stopButton;
    QDockWidget *subDockWidget;
    QWidget *dockWidgetContents_3;
    QGridLayout *gridLayout_3;
    QScrollArea *scrollArea_2;
    QWidget *sensorsContainer;
    QVBoxLayout *verticalLayout_4;
    QDockWidget *refDockWidget;
    QWidget *dockWidgetContents_4;
    QGridLayout *AxesLayout_2;
    QScrollArea *scrollArea_3;
    QWidget *trackingContainer;
    QVBoxLayout *verticalLayout_5;
    QDockWidget *logDockWidget_2;
    QWidget *dockWidgetContents_7;
    QGridLayout *gridLayout_6;
    QVBoxLayout *verticalLayout;
    CmdLogWdg *cmdLog;
    QDockWidget *posDockWidget2;
    QWidget *posDockWidgetContents;
    QGridLayout *gridLayout_9;
    QScrollArea *scrollArea_4;
    QWidget *deviceContainer4;
    QVBoxLayout *verticalLayout_6;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(960, 656);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("application-exit");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionExit->setIcon(icon);
        actionInit = new QAction(MainWindow);
        actionInit->setObjectName(QString::fromUtf8("actionInit"));
        actionEnable = new QAction(MainWindow);
        actionEnable->setObjectName(QString::fromUtf8("actionEnable"));
        actionDisable = new QAction(MainWindow);
        actionDisable->setObjectName(QString::fromUtf8("actionDisable"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        QFont font;
        actionSelect_All->setFont(font);
        actionDeselect_All = new QAction(MainWindow);
        actionDeselect_All->setObjectName(QString::fromUtf8("actionDeselect_All"));
        actionRecover = new QAction(MainWindow);
        actionRecover->setObjectName(QString::fromUtf8("actionRecover"));
        actionDevReset = new QAction(MainWindow);
        actionDevReset->setObjectName(QString::fromUtf8("actionDevReset"));
        actionDevInit = new QAction(MainWindow);
        actionDevInit->setObjectName(QString::fromUtf8("actionDevInit"));
        actionDevEnable = new QAction(MainWindow);
        actionDevEnable->setObjectName(QString::fromUtf8("actionDevEnable"));
        actionDevDisable = new QAction(MainWindow);
        actionDevDisable->setObjectName(QString::fromUtf8("actionDevDisable"));
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        actionDevSimulate = new QAction(MainWindow);
        actionDevSimulate->setObjectName(QString::fromUtf8("actionDevSimulate"));
        actionDevStopSim = new QAction(MainWindow);
        actionDevStopSim->setObjectName(QString::fromUtf8("actionDevStopSim"));
        actionDevIgnore = new QAction(MainWindow);
        actionDevIgnore->setObjectName(QString::fromUtf8("actionDevIgnore"));
        actionDevStopIgn = new QAction(MainWindow);
        actionDevStopIgn->setObjectName(QString::fromUtf8("actionDevStopIgn"));
        actionLogError = new QAction(MainWindow);
        actionLogError->setObjectName(QString::fromUtf8("actionLogError"));
        actionLogInfo = new QAction(MainWindow);
        actionLogInfo->setObjectName(QString::fromUtf8("actionLogInfo"));
        actionLogDebug = new QAction(MainWindow);
        actionLogDebug->setObjectName(QString::fromUtf8("actionLogDebug"));
        actionLogDebug2 = new QAction(MainWindow);
        actionLogDebug2->setObjectName(QString::fromUtf8("actionLogDebug2"));
        actionLogDebug3 = new QAction(MainWindow);
        actionLogDebug3->setObjectName(QString::fromUtf8("actionLogDebug3"));
        actionLogTrace = new QAction(MainWindow);
        actionLogTrace->setObjectName(QString::fromUtf8("actionLogTrace"));
        actionSelectAllCurrent = new QAction(MainWindow);
        actionSelectAllCurrent->setObjectName(QString::fromUtf8("actionSelectAllCurrent"));
        actionDeselectAllCurrent = new QAction(MainWindow);
        actionDeselectAllCurrent->setObjectName(QString::fromUtf8("actionDeselectAllCurrent"));
        actionLoad_Layout = new QAction(MainWindow);
        actionLoad_Layout->setObjectName(QString::fromUtf8("actionLoad_Layout"));
        actionSave_Layout = new QAction(MainWindow);
        actionSave_Layout->setObjectName(QString::fromUtf8("actionSave_Layout"));
        actionReset_Layout = new QAction(MainWindow);
        actionReset_Layout->setObjectName(QString::fromUtf8("actionReset_Layout"));
        actionFCF_CLI = new QAction(MainWindow);
        actionFCF_CLI->setObjectName(QString::fromUtf8("actionFCF_CLI"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionWhatsthis = new QAction(MainWindow);
        actionWhatsthis->setObjectName(QString::fromUtf8("actionWhatsthis"));
        QIcon icon1;
        iconThemeName = QString::fromUtf8("help-faq");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionWhatsthis->setIcon(icon1);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        MainWindow->setCentralWidget(centralWidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 960, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFCS = new QMenu(menubar);
        menuFCS->setObjectName(QString::fromUtf8("menuFCS"));
        menuSet_log_level = new QMenu(menuFCS);
        menuSet_log_level->setObjectName(QString::fromUtf8("menuSet_log_level"));
        menuDevices = new QMenu(menubar);
        menuDevices->setObjectName(QString::fromUtf8("menuDevices"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuHelp->setContextMenuPolicy(Qt::PreventContextMenu);
        menuHelp->setLayoutDirection(Qt::LeftToRight);
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuThemes = new QMenu(menubar);
        menuThemes->setObjectName(QString::fromUtf8("menuThemes"));
        MainWindow->setMenuBar(menubar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        attrDockWidget = new QDockWidget(MainWindow);
        attrDockWidget->setObjectName(QString::fromUtf8("attrDockWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(attrDockWidget->sizePolicy().hasHeightForWidth());
        attrDockWidget->setSizePolicy(sizePolicy1);
        attrDockWidget->setMinimumSize(QSize(400, 107));
        attrDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dockWidgetContents->sizePolicy().hasHeightForWidth());
        dockWidgetContents->setSizePolicy(sizePolicy2);
        dockWidgetContents->setMinimumSize(QSize(100, 0));
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(5, 0, 5, 0);
        devicesScrollArea = new QScrollArea(dockWidgetContents);
        devicesScrollArea->setObjectName(QString::fromUtf8("devicesScrollArea"));
        devicesScrollArea->setWidgetResizable(true);
        devicesContainer = new QWidget();
        devicesContainer->setObjectName(QString::fromUtf8("devicesContainer"));
        devicesContainer->setGeometry(QRect(0, 0, 388, 225));
        verticalLayout_3 = new QVBoxLayout(devicesContainer);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        devicesScrollArea->setWidget(devicesContainer);

        gridLayout_2->addWidget(devicesScrollArea, 0, 0, 1, 1);

        attrDockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, attrDockWidget);
        addrDockWidget = new QDockWidget(MainWindow);
        addrDockWidget->setObjectName(QString::fromUtf8("addrDockWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(addrDockWidget->sizePolicy().hasHeightForWidth());
        addrDockWidget->setSizePolicy(sizePolicy3);
        addrDockWidget->setStyleSheet(QString::fromUtf8("QDockWidget > QWidget {\n"
"    border: 1px solid white;\n"
"}\n"
"\n"
"QDockWidget {\n"
"    border: 1px solid darkgrey;\n"
"    titlebar-close-icon: url(close.png);\n"
"    titlebar-normal-icon: url(undock.png);\n"
"}\n"
"\n"
"QDockWidget::title {\n"
"    text-align: left; /* align the text to the left */\n"
"    background: lightgray;\n"
"    padding-left: 5px;\n"
"}\n"
"\n"
"\n"
"\n"
""));
        addrDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        addrDockWidget->setAllowedAreas(Qt::TopDockWidgetArea);
        addrDockWidget->setWindowTitle(QString::fromUtf8(""));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        sizePolicy2.setHeightForWidth(dockWidgetContents_2->sizePolicy().hasHeightForWidth());
        dockWidgetContents_2->setSizePolicy(sizePolicy2);
        gridLayout = new QGridLayout(dockWidgetContents_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(5, 0, 5, 0);
        setupButton = new QPushButton(dockWidgetContents_2);
        setupButton->setObjectName(QString::fromUtf8("setupButton"));
        setupButton->setMinimumSize(QSize(0, 0));
        QFont font1;
        font1.setPointSize(12);
        setupButton->setFont(font1);

        gridLayout->addWidget(setupButton, 2, 11, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        serverLabel = new QLabel(dockWidgetContents_2);
        serverLabel->setObjectName(QString::fromUtf8("serverLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(serverLabel->sizePolicy().hasHeightForWidth());
        serverLabel->setSizePolicy(sizePolicy4);
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        serverLabel->setFont(font2);

        horizontalLayout->addWidget(serverLabel);

        stateLabel = new QLabel(dockWidgetContents_2);
        stateLabel->setObjectName(QString::fromUtf8("stateLabel"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(stateLabel->sizePolicy().hasHeightForWidth());
        stateLabel->setSizePolicy(sizePolicy5);
        stateLabel->setMinimumSize(QSize(0, 0));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setItalic(false);
        font3.setWeight(75);
        stateLabel->setFont(font3);
        stateLabel->setFrameShape(QFrame::StyledPanel);
        stateLabel->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(stateLabel);

        substateLabel = new QLabel(dockWidgetContents_2);
        substateLabel->setObjectName(QString::fromUtf8("substateLabel"));
        sizePolicy5.setHeightForWidth(substateLabel->sizePolicy().hasHeightForWidth());
        substateLabel->setSizePolicy(sizePolicy5);
        substateLabel->setMinimumSize(QSize(0, 0));
        substateLabel->setFont(font3);
        substateLabel->setFrameShape(QFrame::StyledPanel);
        substateLabel->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(substateLabel);


        verticalLayout_2->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout_2, 2, 7, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 9, 1, 1);

        stopButton = new QPushButton(dockWidgetContents_2);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setMinimumSize(QSize(0, 0));
        stopButton->setFont(font1);

        gridLayout->addWidget(stopButton, 2, 10, 1, 1);

        addrDockWidget->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(Qt::TopDockWidgetArea, addrDockWidget);
        subDockWidget = new QDockWidget(MainWindow);
        subDockWidget->setObjectName(QString::fromUtf8("subDockWidget"));
        sizePolicy2.setHeightForWidth(subDockWidget->sizePolicy().hasHeightForWidth());
        subDockWidget->setSizePolicy(sizePolicy2);
        subDockWidget->setFloating(false);
        subDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        sizePolicy2.setHeightForWidth(dockWidgetContents_3->sizePolicy().hasHeightForWidth());
        dockWidgetContents_3->setSizePolicy(sizePolicy2);
        gridLayout_3 = new QGridLayout(dockWidgetContents_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(5, 0, 5, 0);
        scrollArea_2 = new QScrollArea(dockWidgetContents_3);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        sensorsContainer = new QWidget();
        sensorsContainer->setObjectName(QString::fromUtf8("sensorsContainer"));
        sensorsContainer->setGeometry(QRect(0, 0, 68, 218));
        verticalLayout_4 = new QVBoxLayout(sensorsContainer);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        scrollArea_2->setWidget(sensorsContainer);

        gridLayout_3->addWidget(scrollArea_2, 0, 0, 1, 1);

        subDockWidget->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, subDockWidget);
        refDockWidget = new QDockWidget(MainWindow);
        refDockWidget->setObjectName(QString::fromUtf8("refDockWidget"));
        sizePolicy2.setHeightForWidth(refDockWidget->sizePolicy().hasHeightForWidth());
        refDockWidget->setSizePolicy(sizePolicy2);
        refDockWidget->setMinimumSize(QSize(100, 107));
        refDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        sizePolicy2.setHeightForWidth(dockWidgetContents_4->sizePolicy().hasHeightForWidth());
        dockWidgetContents_4->setSizePolicy(sizePolicy2);
        AxesLayout_2 = new QGridLayout(dockWidgetContents_4);
        AxesLayout_2->setSpacing(6);
        AxesLayout_2->setContentsMargins(11, 11, 11, 11);
        AxesLayout_2->setObjectName(QString::fromUtf8("AxesLayout_2"));
        AxesLayout_2->setContentsMargins(5, 0, 5, 0);
        scrollArea_3 = new QScrollArea(dockWidgetContents_4);
        scrollArea_3->setObjectName(QString::fromUtf8("scrollArea_3"));
        scrollArea_3->setWidgetResizable(true);
        trackingContainer = new QWidget();
        trackingContainer->setObjectName(QString::fromUtf8("trackingContainer"));
        trackingContainer->setGeometry(QRect(0, 0, 388, 224));
        verticalLayout_5 = new QVBoxLayout(trackingContainer);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        scrollArea_3->setWidget(trackingContainer);

        AxesLayout_2->addWidget(scrollArea_3, 0, 0, 1, 1);

        refDockWidget->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, refDockWidget);
        logDockWidget_2 = new QDockWidget(MainWindow);
        logDockWidget_2->setObjectName(QString::fromUtf8("logDockWidget_2"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(logDockWidget_2->sizePolicy().hasHeightForWidth());
        logDockWidget_2->setSizePolicy(sizePolicy6);
        logDockWidget_2->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);
        logDockWidget_2->setAllowedAreas(Qt::BottomDockWidgetArea);
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QString::fromUtf8("dockWidgetContents_7"));
        sizePolicy6.setHeightForWidth(dockWidgetContents_7->sizePolicy().hasHeightForWidth());
        dockWidgetContents_7->setSizePolicy(sizePolicy6);
        gridLayout_6 = new QGridLayout(dockWidgetContents_7);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(5, 0, 5, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        cmdLog = new CmdLogWdg(dockWidgetContents_7);
        cmdLog->setObjectName(QString::fromUtf8("cmdLog"));
        QSizePolicy sizePolicy7(QSizePolicy::Expanding, QSizePolicy::Ignored);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(1);
        sizePolicy7.setHeightForWidth(cmdLog->sizePolicy().hasHeightForWidth());
        cmdLog->setSizePolicy(sizePolicy7);
        cmdLog->setBaseSize(QSize(0, 0));

        verticalLayout->addWidget(cmdLog);


        gridLayout_6->addLayout(verticalLayout, 1, 0, 1, 1);

        logDockWidget_2->setWidget(dockWidgetContents_7);
        MainWindow->addDockWidget(Qt::BottomDockWidgetArea, logDockWidget_2);
        posDockWidget2 = new QDockWidget(MainWindow);
        posDockWidget2->setObjectName(QString::fromUtf8("posDockWidget2"));
        sizePolicy2.setHeightForWidth(posDockWidget2->sizePolicy().hasHeightForWidth());
        posDockWidget2->setSizePolicy(sizePolicy2);
        posDockWidget2->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        posDockWidgetContents = new QWidget();
        posDockWidgetContents->setObjectName(QString::fromUtf8("posDockWidgetContents"));
        gridLayout_9 = new QGridLayout(posDockWidgetContents);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setContentsMargins(5, 0, 0, 5);
        scrollArea_4 = new QScrollArea(posDockWidgetContents);
        scrollArea_4->setObjectName(QString::fromUtf8("scrollArea_4"));
        scrollArea_4->setWidgetResizable(true);
        deviceContainer4 = new QWidget();
        deviceContainer4->setObjectName(QString::fromUtf8("deviceContainer4"));
        deviceContainer4->setGeometry(QRect(0, 0, 73, 226));
        verticalLayout_6 = new QVBoxLayout(deviceContainer4);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        scrollArea_4->setWidget(deviceContainer4);

        gridLayout_9->addWidget(scrollArea_4, 1, 0, 1, 1);

        posDockWidget2->setWidget(posDockWidgetContents);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, posDockWidget2);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuFCS->menuAction());
        menubar->addAction(menuDevices->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuThemes->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuFCS->addAction(actionInit);
        menuFCS->addAction(actionEnable);
        menuFCS->addAction(actionDisable);
        menuFCS->addSeparator();
        menuFCS->addAction(actionRecover);
        menuFCS->addSeparator();
        menuFCS->addAction(actionReset);
        menuFCS->addSeparator();
        menuFCS->addAction(menuSet_log_level->menuAction());
        menuSet_log_level->addAction(actionLogError);
        menuSet_log_level->addAction(actionLogInfo);
        menuSet_log_level->addAction(actionLogDebug);
        menuSet_log_level->addSeparator();
        menuSet_log_level->addAction(actionLogTrace);
        menuDevices->addAction(actionSelect_All);
        menuDevices->addAction(actionDeselect_All);
        menuDevices->addSeparator();
        menuDevices->addSeparator();
        menuDevices->addSeparator();
        menuDevices->addAction(actionDevReset);
        menuDevices->addAction(actionDevInit);
        menuDevices->addAction(actionDevEnable);
        menuDevices->addAction(actionDevDisable);
        menuDevices->addSeparator();
        menuDevices->addAction(actionDevSimulate);
        menuDevices->addAction(actionDevStopSim);
        menuDevices->addSeparator();
        menuDevices->addAction(actionDevIgnore);
        menuDevices->addAction(actionDevStopIgn);
        menuHelp->addAction(actionAbout);
        menuTools->addAction(actionFCF_CLI);
        toolBar->addAction(actionExit);
        toolBar->addAction(actionWhatsthis);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "FreeOpcUa Client", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
#if QT_CONFIG(tooltip)
        actionExit->setToolTip(QCoreApplication::translate("MainWindow", "Quit FCF GUI", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionInit->setText(QCoreApplication::translate("MainWindow", "Init", nullptr));
        actionEnable->setText(QCoreApplication::translate("MainWindow", "Enable", nullptr));
        actionDisable->setText(QCoreApplication::translate("MainWindow", "Disable", nullptr));
        actionSelect_All->setText(QCoreApplication::translate("MainWindow", "Select all", nullptr));
#if QT_CONFIG(shortcut)
        actionSelect_All->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDeselect_All->setText(QCoreApplication::translate("MainWindow", "Deselect all", nullptr));
#if QT_CONFIG(shortcut)
        actionDeselect_All->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRecover->setText(QCoreApplication::translate("MainWindow", "Recover", nullptr));
        actionDevReset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
#if QT_CONFIG(shortcut)
        actionDevReset->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDevInit->setText(QCoreApplication::translate("MainWindow", "Init", nullptr));
#if QT_CONFIG(shortcut)
        actionDevInit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+I", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDevEnable->setText(QCoreApplication::translate("MainWindow", "Enable", nullptr));
#if QT_CONFIG(shortcut)
        actionDevEnable->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDevDisable->setText(QCoreApplication::translate("MainWindow", "Disable", nullptr));
#if QT_CONFIG(shortcut)
        actionDevDisable->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+D", nullptr));
#endif // QT_CONFIG(shortcut)
        actionReset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        actionDevSimulate->setText(QCoreApplication::translate("MainWindow", "Sim HW", nullptr));
        actionDevStopSim->setText(QCoreApplication::translate("MainWindow", "Stop Sim HW", nullptr));
        actionDevIgnore->setText(QCoreApplication::translate("MainWindow", "Ignore HW", nullptr));
        actionDevStopIgn->setText(QCoreApplication::translate("MainWindow", "Stop Ignore HW", nullptr));
        actionLogError->setText(QCoreApplication::translate("MainWindow", "ERROR", nullptr));
        actionLogInfo->setText(QCoreApplication::translate("MainWindow", "INFO", nullptr));
        actionLogDebug->setText(QCoreApplication::translate("MainWindow", "DEBUG", nullptr));
        actionLogDebug2->setText(QCoreApplication::translate("MainWindow", "DEBUG2", nullptr));
        actionLogDebug3->setText(QCoreApplication::translate("MainWindow", "DEBUG3", nullptr));
        actionLogTrace->setText(QCoreApplication::translate("MainWindow", "TRACE", nullptr));
        actionSelectAllCurrent->setText(QCoreApplication::translate("MainWindow", "Select all (current page)", nullptr));
        actionDeselectAllCurrent->setText(QCoreApplication::translate("MainWindow", "Deselect all (current page)", nullptr));
        actionLoad_Layout->setText(QCoreApplication::translate("MainWindow", "Load Layout", nullptr));
        actionSave_Layout->setText(QCoreApplication::translate("MainWindow", "Save Layout", nullptr));
        actionReset_Layout->setText(QCoreApplication::translate("MainWindow", "Reset Layout", nullptr));
        actionFCF_CLI->setText(QCoreApplication::translate("MainWindow", "FCF CLI", nullptr));
#if QT_CONFIG(shortcut)
        actionFCF_CLI->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About ...", nullptr));
        actionWhatsthis->setText(QCoreApplication::translate("MainWindow", "Whatsthis", nullptr));
#if QT_CONFIG(tooltip)
        actionWhatsthis->setToolTip(QCoreApplication::translate("MainWindow", "Activate Whatsthis", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionWhatsthis->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+H", nullptr));
#endif // QT_CONFIG(shortcut)
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuFCS->setTitle(QCoreApplication::translate("MainWindow", "FCS", nullptr));
        menuSet_log_level->setTitle(QCoreApplication::translate("MainWindow", "Set log level", nullptr));
        menuDevices->setTitle(QCoreApplication::translate("MainWindow", "Devices", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
        menuThemes->setTitle(QCoreApplication::translate("MainWindow", "Themes", nullptr));
        attrDockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "Devices", nullptr));
        setupButton->setText(QCoreApplication::translate("MainWindow", "SETUP", nullptr));
        serverLabel->setText(QCoreApplication::translate("MainWindow", "State:", nullptr));
#if QT_CONFIG(tooltip)
        stateLabel->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        stateLabel->setText(QCoreApplication::translate("MainWindow", "state", nullptr));
        substateLabel->setText(QCoreApplication::translate("MainWindow", "substate", nullptr));
        stopButton->setText(QCoreApplication::translate("MainWindow", "STOP", nullptr));
        subDockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "Sensors", nullptr));
        refDockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "Tracking", nullptr));
        logDockWidget_2->setWindowTitle(QCoreApplication::translate("MainWindow", "Command Log", nullptr));
        posDockWidget2->setWindowTitle(QCoreApplication::translate("MainWindow", "Cryogenic", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
