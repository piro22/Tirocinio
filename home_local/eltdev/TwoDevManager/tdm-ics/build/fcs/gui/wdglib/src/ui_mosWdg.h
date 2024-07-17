/********************************************************************************
** Form generated from reading UI file 'mosWdg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOSWDG_H
#define UI_MOSWDG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <fcf/gui/wdglib/spinner.h>

QT_BEGIN_NAMESPACE

class Ui_MosWdg
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *devFrame;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_3;
    QLabel *nameLabel;
    QDoubleSpinBox *piston;
    QPushButton *simLed;
    QCheckBox *enableCheckBox;
    QDoubleSpinBox *tip;
    QLabel *substateLabel;
    QPushButton *localLed;
    QPushButton *setupButton;
    QComboBox *optionsComboBox;
    QLabel *label_2;
    QLabel *stateLabel;
    QDoubleSpinBox *tilt;
    QPushButton *errorLed;
    Spinner *activitySpinner;

    void setupUi(QWidget *MosWdg)
    {
        if (MosWdg->objectName().isEmpty())
            MosWdg->setObjectName(QString::fromUtf8("MosWdg"));
        MosWdg->resize(690, 86);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MosWdg->sizePolicy().hasHeightForWidth());
        MosWdg->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(MosWdg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        devFrame = new QFrame(MosWdg);
        devFrame->setObjectName(QString::fromUtf8("devFrame"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(devFrame->sizePolicy().hasHeightForWidth());
        devFrame->setSizePolicy(sizePolicy1);
        devFrame->setFrameShape(QFrame::StyledPanel);
        devFrame->setFrameShadow(QFrame::Raised);
        devFrame->setLineWidth(1);
        horizontalLayout = new QHBoxLayout(devFrame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(devFrame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 5, 1, 1);

        label_3 = new QLabel(devFrame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 7, 1, 1);

        nameLabel = new QLabel(devFrame);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(nameLabel->sizePolicy().hasHeightForWidth());
        nameLabel->setSizePolicy(sizePolicy2);
        nameLabel->setMinimumSize(QSize(0, 0));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        nameLabel->setFont(font);
        nameLabel->setFrameShape(QFrame::StyledPanel);
        nameLabel->setFrameShadow(QFrame::Raised);
        nameLabel->setScaledContents(true);

        gridLayout->addWidget(nameLabel, 0, 2, 1, 1);

        piston = new QDoubleSpinBox(devFrame);
        piston->setObjectName(QString::fromUtf8("piston"));

        gridLayout->addWidget(piston, 2, 8, 1, 1);

        simLed = new QPushButton(devFrame);
        simLed->setObjectName(QString::fromUtf8("simLed"));
        simLed->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(simLed->sizePolicy().hasHeightForWidth());
        simLed->setSizePolicy(sizePolicy3);
        simLed->setMinimumSize(QSize(0, 0));
        simLed->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(simLed, 0, 1, 1, 1);

        enableCheckBox = new QCheckBox(devFrame);
        enableCheckBox->setObjectName(QString::fromUtf8("enableCheckBox"));
        enableCheckBox->setEnabled(true);
        sizePolicy3.setHeightForWidth(enableCheckBox->sizePolicy().hasHeightForWidth());
        enableCheckBox->setSizePolicy(sizePolicy3);
        enableCheckBox->setAutoFillBackground(false);
        enableCheckBox->setCheckable(true);

        gridLayout->addWidget(enableCheckBox, 0, 0, 1, 1);

        tip = new QDoubleSpinBox(devFrame);
        tip->setObjectName(QString::fromUtf8("tip"));

        gridLayout->addWidget(tip, 2, 6, 1, 1);

        substateLabel = new QLabel(devFrame);
        substateLabel->setObjectName(QString::fromUtf8("substateLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(2);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(substateLabel->sizePolicy().hasHeightForWidth());
        substateLabel->setSizePolicy(sizePolicy4);
        substateLabel->setSizeIncrement(QSize(0, 0));
        substateLabel->setAutoFillBackground(false);
        substateLabel->setFrameShape(QFrame::StyledPanel);
        substateLabel->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(substateLabel, 2, 4, 1, 1);

        localLed = new QPushButton(devFrame);
        localLed->setObjectName(QString::fromUtf8("localLed"));
        localLed->setEnabled(false);
        sizePolicy3.setHeightForWidth(localLed->sizePolicy().hasHeightForWidth());
        localLed->setSizePolicy(sizePolicy3);
        localLed->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(localLed, 2, 1, 1, 1);

        setupButton = new QPushButton(devFrame);
        setupButton->setObjectName(QString::fromUtf8("setupButton"));

        gridLayout->addWidget(setupButton, 0, 10, 1, 1);

        optionsComboBox = new QComboBox(devFrame);
        optionsComboBox->addItem(QString());
        optionsComboBox->setObjectName(QString::fromUtf8("optionsComboBox"));
        optionsComboBox->setModelColumn(0);

        gridLayout->addWidget(optionsComboBox, 0, 8, 1, 1);

        label_2 = new QLabel(devFrame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 5, 1, 1);

        stateLabel = new QLabel(devFrame);
        stateLabel->setObjectName(QString::fromUtf8("stateLabel"));
        sizePolicy4.setHeightForWidth(stateLabel->sizePolicy().hasHeightForWidth());
        stateLabel->setSizePolicy(sizePolicy4);
        stateLabel->setAutoFillBackground(false);
        stateLabel->setFrameShape(QFrame::StyledPanel);
        stateLabel->setFrameShadow(QFrame::Raised);
        stateLabel->setScaledContents(true);

        gridLayout->addWidget(stateLabel, 0, 4, 1, 1);

        tilt = new QDoubleSpinBox(devFrame);
        tilt->setObjectName(QString::fromUtf8("tilt"));

        gridLayout->addWidget(tilt, 0, 6, 1, 1);

        errorLed = new QPushButton(devFrame);
        errorLed->setObjectName(QString::fromUtf8("errorLed"));
        errorLed->setEnabled(false);
        errorLed->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(errorLed, 0, 3, 1, 1);


        horizontalLayout->addLayout(gridLayout);

        activitySpinner = new Spinner(devFrame);
        activitySpinner->setObjectName(QString::fromUtf8("activitySpinner"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(activitySpinner->sizePolicy().hasHeightForWidth());
        activitySpinner->setSizePolicy(sizePolicy5);
        activitySpinner->setMinimumSize(QSize(40, 40));
        activitySpinner->setBaseSize(QSize(40, 40));

        horizontalLayout->addWidget(activitySpinner);


        verticalLayout->addWidget(devFrame);


        retranslateUi(MosWdg);

        QMetaObject::connectSlotsByName(MosWdg);
    } // setupUi

    void retranslateUi(QWidget *MosWdg)
    {
        MosWdg->setWindowTitle(QCoreApplication::translate("MosWdg", "Form", nullptr));
        label->setText(QCoreApplication::translate("MosWdg", "tip", nullptr));
        label_3->setText(QCoreApplication::translate("MosWdg", "piston", nullptr));
        nameLabel->setText(QCoreApplication::translate("MosWdg", "name", nullptr));
        simLed->setText(QCoreApplication::translate("MosWdg", "S", nullptr));
#if QT_CONFIG(tooltip)
        enableCheckBox->setToolTip(QCoreApplication::translate("MosWdg", "By enabling the device, it makes it accountable when pressing the Setup button at the bottom of the GUI. It is also applicable for the actions in the Device menu.", nullptr));
#endif // QT_CONFIG(tooltip)
        enableCheckBox->setText(QString());
        substateLabel->setText(QCoreApplication::translate("MosWdg", "substate", nullptr));
        localLed->setText(QCoreApplication::translate("MosWdg", "L", nullptr));
        setupButton->setText(QCoreApplication::translate("MosWdg", "Setup", nullptr));
        optionsComboBox->setItemText(0, QCoreApplication::translate("MosWdg", "MOVE", nullptr));

        label_2->setText(QCoreApplication::translate("MosWdg", "tilt", nullptr));
        stateLabel->setText(QCoreApplication::translate("MosWdg", "state", nullptr));
        errorLed->setText(QCoreApplication::translate("MosWdg", "E", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MosWdg: public Ui_MosWdg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOSWDG_H
