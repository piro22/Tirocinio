# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'ui_taurusmos.ui'
##
## Created by: Qt User Interface Compiler version 5.15.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *

from taurus.qt.qtgui.display import TaurusLabel


class Ui_TaurusMos(object):
    def setupUi(self, TaurusMos):
        if not TaurusMos.objectName():
            TaurusMos.setObjectName(u"TaurusMos")
        TaurusMos.resize(690, 86)
        sizePolicy = QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(TaurusMos.sizePolicy().hasHeightForWidth())
        TaurusMos.setSizePolicy(sizePolicy)
        self.verticalLayout = QVBoxLayout(TaurusMos)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(3, 3, 3, 3)
        self.devFrame = QFrame(TaurusMos)
        self.devFrame.setObjectName(u"devFrame")
        sizePolicy1 = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Preferred)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.devFrame.sizePolicy().hasHeightForWidth())
        self.devFrame.setSizePolicy(sizePolicy1)
        self.devFrame.setFrameShape(QFrame.StyledPanel)
        self.devFrame.setFrameShadow(QFrame.Raised)
        self.devFrame.setLineWidth(1)
        self.horizontalLayout = QHBoxLayout(self.devFrame)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(3, 3, 3, 3)
        self.gridLayout = QGridLayout()
        self.gridLayout.setObjectName(u"gridLayout")
        self.label = QLabel(self.devFrame)
        self.label.setObjectName(u"label")

        self.gridLayout.addWidget(self.label, 2, 5, 1, 1)

        self.label_3 = QLabel(self.devFrame)
        self.label_3.setObjectName(u"label_3")

        self.gridLayout.addWidget(self.label_3, 2, 7, 1, 1)

        self.nameLabel = QLabel(self.devFrame)
        self.nameLabel.setObjectName(u"nameLabel")
        sizePolicy2 = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Fixed)
        sizePolicy2.setHorizontalStretch(1)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.nameLabel.sizePolicy().hasHeightForWidth())
        self.nameLabel.setSizePolicy(sizePolicy2)
        self.nameLabel.setMinimumSize(QSize(0, 0))
        font = QFont()
        font.setBold(True)
        font.setWeight(75)
        self.nameLabel.setFont(font)
        self.nameLabel.setFrameShape(QFrame.StyledPanel)
        self.nameLabel.setFrameShadow(QFrame.Raised)
        self.nameLabel.setScaledContents(True)

        self.gridLayout.addWidget(self.nameLabel, 0, 2, 1, 1)

        self.piston = QDoubleSpinBox(self.devFrame)
        self.piston.setObjectName(u"piston")

        self.gridLayout.addWidget(self.piston, 2, 8, 1, 1)

        self.simLed = QPushButton(self.devFrame)
        self.simLed.setObjectName(u"simLed")
        self.simLed.setEnabled(False)
        sizePolicy3 = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.simLed.sizePolicy().hasHeightForWidth())
        self.simLed.setSizePolicy(sizePolicy3)
        self.simLed.setMinimumSize(QSize(0, 0))
        self.simLed.setMaximumSize(QSize(20, 20))

        self.gridLayout.addWidget(self.simLed, 0, 1, 1, 1)

        self.enableCheckBox = QCheckBox(self.devFrame)
        self.enableCheckBox.setObjectName(u"enableCheckBox")
        self.enableCheckBox.setEnabled(True)
        sizePolicy3.setHeightForWidth(self.enableCheckBox.sizePolicy().hasHeightForWidth())
        self.enableCheckBox.setSizePolicy(sizePolicy3)
        self.enableCheckBox.setAutoFillBackground(False)
        self.enableCheckBox.setCheckable(True)

        self.gridLayout.addWidget(self.enableCheckBox, 0, 0, 1, 1)

        self.tip = QDoubleSpinBox(self.devFrame)
        self.tip.setObjectName(u"tip")

        self.gridLayout.addWidget(self.tip, 2, 6, 1, 1)

        self.substateLabel = QLabel(self.devFrame)
        self.substateLabel.setObjectName(u"substateLabel")
        sizePolicy4 = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Fixed)
        sizePolicy4.setHorizontalStretch(2)
        sizePolicy4.setVerticalStretch(0)
        sizePolicy4.setHeightForWidth(self.substateLabel.sizePolicy().hasHeightForWidth())
        self.substateLabel.setSizePolicy(sizePolicy4)
        self.substateLabel.setSizeIncrement(QSize(0, 0))
        self.substateLabel.setAutoFillBackground(False)
        self.substateLabel.setFrameShape(QFrame.StyledPanel)
        self.substateLabel.setFrameShadow(QFrame.Sunken)

        self.gridLayout.addWidget(self.substateLabel, 2, 4, 1, 1)

        self.localLed = QPushButton(self.devFrame)
        self.localLed.setObjectName(u"localLed")
        self.localLed.setEnabled(False)
        sizePolicy3.setHeightForWidth(self.localLed.sizePolicy().hasHeightForWidth())
        self.localLed.setSizePolicy(sizePolicy3)
        self.localLed.setMaximumSize(QSize(20, 20))

        self.gridLayout.addWidget(self.localLed, 2, 1, 1, 1)

        self.setupButton = QPushButton(self.devFrame)
        self.setupButton.setObjectName(u"setupButton")

        self.gridLayout.addWidget(self.setupButton, 0, 10, 1, 1)

        self.optionsComboBox = QComboBox(self.devFrame)
        self.optionsComboBox.addItem("")
        self.optionsComboBox.setObjectName(u"optionsComboBox")
        self.optionsComboBox.setModelColumn(0)

        self.gridLayout.addWidget(self.optionsComboBox, 0, 8, 1, 1)

        self.label_2 = QLabel(self.devFrame)
        self.label_2.setObjectName(u"label_2")

        self.gridLayout.addWidget(self.label_2, 0, 5, 1, 1)

        self.stateLabel = TaurusLabel(self.devFrame)
        self.stateLabel.setObjectName(u"stateLabel")
        sizePolicy4.setHeightForWidth(self.stateLabel.sizePolicy().hasHeightForWidth())
        self.stateLabel.setSizePolicy(sizePolicy4)
        self.stateLabel.setAutoFillBackground(False)
        self.stateLabel.setFrameShape(QFrame.StyledPanel)
        self.stateLabel.setFrameShadow(QFrame.Sunken)
        self.stateLabel.setLineWidth(2)
        self.stateLabel.setScaledContents(True)
        self.stateLabel.setProperty("autoTrim", False)

        self.gridLayout.addWidget(self.stateLabel, 0, 4, 1, 1)

        self.tilt = QDoubleSpinBox(self.devFrame)
        self.tilt.setObjectName(u"tilt")

        self.gridLayout.addWidget(self.tilt, 0, 6, 1, 1)

        self.errorLed = QPushButton(self.devFrame)
        self.errorLed.setObjectName(u"errorLed")
        self.errorLed.setEnabled(False)
        self.errorLed.setMaximumSize(QSize(20, 20))

        self.gridLayout.addWidget(self.errorLed, 0, 3, 1, 1)


        self.horizontalLayout.addLayout(self.gridLayout)


        self.verticalLayout.addWidget(self.devFrame)


        self.retranslateUi(TaurusMos)

        QMetaObject.connectSlotsByName(TaurusMos)
    # setupUi

    def retranslateUi(self, TaurusMos):
        TaurusMos.setWindowTitle(QCoreApplication.translate("TaurusMos", u"Form", None))
        self.label.setText(QCoreApplication.translate("TaurusMos", u"tip", None))
        self.label_3.setText(QCoreApplication.translate("TaurusMos", u"piston", None))
        self.nameLabel.setText(QCoreApplication.translate("TaurusMos", u"name", None))
#if QT_CONFIG(tooltip)
        self.simLed.setToolTip(QCoreApplication.translate("TaurusMos", u"<html><head/><body><p>When highlighted in orange, it indicates that device is in simulation mode.</p></body></html>", None))
#endif // QT_CONFIG(tooltip)
        self.simLed.setText(QCoreApplication.translate("TaurusMos", u"S", None))
#if QT_CONFIG(tooltip)
        self.enableCheckBox.setToolTip(QCoreApplication.translate("TaurusMos", u"By enabling the device, it makes it accountable when pressing the Setup button at the bottom of the GUI. It is also applicable for the actions in the Device menu.", None))
#endif // QT_CONFIG(tooltip)
        self.enableCheckBox.setText("")
        self.substateLabel.setText(QCoreApplication.translate("TaurusMos", u"substate", None))
#if QT_CONFIG(tooltip)
        self.localLed.setToolTip(QCoreApplication.translate("TaurusMos", u"<html><head/><body><p>When highlighted in yellow, it indicates that device is in local mode.</p></body></html>", None))
#endif // QT_CONFIG(tooltip)
        self.localLed.setText(QCoreApplication.translate("TaurusMos", u"L", None))
        self.setupButton.setText(QCoreApplication.translate("TaurusMos", u"Setup", None))
        self.optionsComboBox.setItemText(0, QCoreApplication.translate("TaurusMos", u"MOVE", None))

        self.label_2.setText(QCoreApplication.translate("TaurusMos", u"tilt", None))
        self.stateLabel.setText(QCoreApplication.translate("TaurusMos", u"state", None))
        self.stateLabel.setProperty("model", QCoreApplication.translate("TaurusMos", u"lcs/state", None))
        self.stateLabel.setProperty("bgRole", "")
#if QT_CONFIG(tooltip)
        self.errorLed.setToolTip(QCoreApplication.translate("TaurusMos", u"<html><head/><body><p>When highlighted in red, it indicates that device got an error. You can see the error by hovering the mouse over the button.</p></body></html>", None))
#endif // QT_CONFIG(tooltip)
        self.errorLed.setText(QCoreApplication.translate("TaurusMos", u"E", None))
    # retranslateUi

