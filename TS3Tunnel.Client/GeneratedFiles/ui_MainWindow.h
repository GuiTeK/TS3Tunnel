/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QGroupBox *StatisticsGroupBox;
    QLabel *DecodedVoicePacketsLabel;
    QLabel *DecodingErrorsLabel;
    QLabel *DecodedVoicePacketsNbLabel;
    QLabel *DecodingErrorsNbLabel;
    QLabel *DecodedVoicePacketsBytesNbLabel;
    QLabel *DecodedVoicePacketsBytesLabel;
    QGroupBox *SettingsGroupBox;
    QPushButton *ConnectPushButton;
    QLineEdit *ServerIPAddressLineEdit;
    QLabel *TS3EServerPortColonLabel;
    QLabel *ServerLabel;
    QSpinBox *ServerPortSpinBox;
    QLabel *PasswordLabel;
    QLineEdit *PasswordLineEdit;
    QLabel *StorageFolderLabel;
    QLineEdit *StorageFolderLineEdit;
    QPushButton *StorageFolderBrowsePushButton;
    QTableWidget *VoiceSessionsTableWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(604, 328);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindowClass->sizePolicy().hasHeightForWidth());
        MainWindowClass->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        StatisticsGroupBox = new QGroupBox(centralWidget);
        StatisticsGroupBox->setObjectName(QStringLiteral("StatisticsGroupBox"));
        StatisticsGroupBox->setGeometry(QRect(10, 180, 301, 91));
        DecodedVoicePacketsLabel = new QLabel(StatisticsGroupBox);
        DecodedVoicePacketsLabel->setObjectName(QStringLiteral("DecodedVoicePacketsLabel"));
        DecodedVoicePacketsLabel->setGeometry(QRect(10, 20, 121, 16));
        DecodingErrorsLabel = new QLabel(StatisticsGroupBox);
        DecodingErrorsLabel->setObjectName(QStringLiteral("DecodingErrorsLabel"));
        DecodingErrorsLabel->setGeometry(QRect(10, 60, 121, 16));
        DecodedVoicePacketsNbLabel = new QLabel(StatisticsGroupBox);
        DecodedVoicePacketsNbLabel->setObjectName(QStringLiteral("DecodedVoicePacketsNbLabel"));
        DecodedVoicePacketsNbLabel->setGeometry(QRect(180, 20, 101, 16));
        DecodingErrorsNbLabel = new QLabel(StatisticsGroupBox);
        DecodingErrorsNbLabel->setObjectName(QStringLiteral("DecodingErrorsNbLabel"));
        DecodingErrorsNbLabel->setGeometry(QRect(180, 60, 101, 16));
        DecodedVoicePacketsBytesNbLabel = new QLabel(StatisticsGroupBox);
        DecodedVoicePacketsBytesNbLabel->setObjectName(QStringLiteral("DecodedVoicePacketsBytesNbLabel"));
        DecodedVoicePacketsBytesNbLabel->setGeometry(QRect(180, 40, 101, 16));
        DecodedVoicePacketsBytesLabel = new QLabel(StatisticsGroupBox);
        DecodedVoicePacketsBytesLabel->setObjectName(QStringLiteral("DecodedVoicePacketsBytesLabel"));
        DecodedVoicePacketsBytesLabel->setGeometry(QRect(10, 40, 161, 16));
        SettingsGroupBox = new QGroupBox(centralWidget);
        SettingsGroupBox->setObjectName(QStringLiteral("SettingsGroupBox"));
        SettingsGroupBox->setGeometry(QRect(10, 10, 301, 161));
        ConnectPushButton = new QPushButton(SettingsGroupBox);
        ConnectPushButton->setObjectName(QStringLiteral("ConnectPushButton"));
        ConnectPushButton->setGeometry(QRect(110, 120, 75, 23));
        ServerIPAddressLineEdit = new QLineEdit(SettingsGroupBox);
        ServerIPAddressLineEdit->setObjectName(QStringLiteral("ServerIPAddressLineEdit"));
        ServerIPAddressLineEdit->setGeometry(QRect(100, 20, 113, 22));
        TS3EServerPortColonLabel = new QLabel(SettingsGroupBox);
        TS3EServerPortColonLabel->setObjectName(QStringLiteral("TS3EServerPortColonLabel"));
        TS3EServerPortColonLabel->setGeometry(QRect(220, 20, 16, 16));
        ServerLabel = new QLabel(SettingsGroupBox);
        ServerLabel->setObjectName(QStringLiteral("ServerLabel"));
        ServerLabel->setGeometry(QRect(10, 20, 71, 16));
        ServerPortSpinBox = new QSpinBox(SettingsGroupBox);
        ServerPortSpinBox->setObjectName(QStringLiteral("ServerPortSpinBox"));
        ServerPortSpinBox->setGeometry(QRect(230, 20, 61, 22));
        ServerPortSpinBox->setMaximum(65535);
        ServerPortSpinBox->setValue(42317);
        PasswordLabel = new QLabel(SettingsGroupBox);
        PasswordLabel->setObjectName(QStringLiteral("PasswordLabel"));
        PasswordLabel->setGeometry(QRect(10, 50, 71, 16));
        PasswordLineEdit = new QLineEdit(SettingsGroupBox);
        PasswordLineEdit->setObjectName(QStringLiteral("PasswordLineEdit"));
        PasswordLineEdit->setGeometry(QRect(100, 50, 113, 22));
        PasswordLineEdit->setEchoMode(QLineEdit::Password);
        StorageFolderLabel = new QLabel(SettingsGroupBox);
        StorageFolderLabel->setObjectName(QStringLiteral("StorageFolderLabel"));
        StorageFolderLabel->setGeometry(QRect(10, 80, 81, 16));
        StorageFolderLineEdit = new QLineEdit(SettingsGroupBox);
        StorageFolderLineEdit->setObjectName(QStringLiteral("StorageFolderLineEdit"));
        StorageFolderLineEdit->setGeometry(QRect(100, 80, 113, 22));
        StorageFolderLineEdit->setEchoMode(QLineEdit::Normal);
        StorageFolderBrowsePushButton = new QPushButton(SettingsGroupBox);
        StorageFolderBrowsePushButton->setObjectName(QStringLiteral("StorageFolderBrowsePushButton"));
        StorageFolderBrowsePushButton->setGeometry(QRect(220, 80, 75, 23));
        VoiceSessionsTableWidget = new QTableWidget(centralWidget);
        VoiceSessionsTableWidget->setObjectName(QStringLiteral("VoiceSessionsTableWidget"));
        VoiceSessionsTableWidget->setGeometry(QRect(320, 15, 271, 251));
        VoiceSessionsTableWidget->setColumnCount(0);
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 604, 21));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "TS3Tunnel Client", nullptr));
        StatisticsGroupBox->setTitle(QApplication::translate("MainWindowClass", "Statistics", nullptr));
        DecodedVoicePacketsLabel->setText(QApplication::translate("MainWindowClass", "Decoded voice packets :", nullptr));
        DecodingErrorsLabel->setText(QApplication::translate("MainWindowClass", "Decoding error(s) :", nullptr));
        DecodedVoicePacketsNbLabel->setText(QApplication::translate("MainWindowClass", "N/A", nullptr));
        DecodingErrorsNbLabel->setText(QApplication::translate("MainWindowClass", "N/A", nullptr));
        DecodedVoicePacketsBytesNbLabel->setText(QApplication::translate("MainWindowClass", "N/A", nullptr));
        DecodedVoicePacketsBytesLabel->setText(QApplication::translate("MainWindowClass", "Decoded voice packets (bytes) :", nullptr));
        SettingsGroupBox->setTitle(QApplication::translate("MainWindowClass", "Settings", nullptr));
        ConnectPushButton->setText(QApplication::translate("MainWindowClass", "Connect", nullptr));
        TS3EServerPortColonLabel->setText(QApplication::translate("MainWindowClass", ":", nullptr));
        ServerLabel->setText(QApplication::translate("MainWindowClass", "Server :", nullptr));
        PasswordLabel->setText(QApplication::translate("MainWindowClass", "Password :", nullptr));
        StorageFolderLabel->setText(QApplication::translate("MainWindowClass", "Storage folder :", nullptr));
        StorageFolderBrowsePushButton->setText(QApplication::translate("MainWindowClass", "Browse...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
