#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include <QTimer>

#include "Client.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);


private slots:
	void on_ConnectPushButton_clicked(bool checked = false);
	void on_StorageFolderBrowsePushButton_clicked(bool checked = false);
	void on_StorageFolderLineEdit_textChanged(const QString &text);
	void on_refreshUiStatsTimer_timeout();
	void on_ts3eClient_newVoiceSession(quint64 sessionId);
	void on_voiceSessionCapabilityCheckBox_stateChanged(int state);


private:
	Ui::MainWindowClass ui;
	Client *m_client;
	QTimer m_refreshUiStatsTimer;
};
