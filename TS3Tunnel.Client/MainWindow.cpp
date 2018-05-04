#include "MainWindow.h"

#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow{ parent },
m_client{ nullptr },
m_refreshUiStatsTimer{}
{
	ui.setupUi(this);

	ui.VoiceSessionsTableWidget->setColumnCount(2);
	ui.VoiceSessionsTableWidget->setHorizontalHeaderLabels(QStringList{ "Session ID", "Listen" });
	ui.VoiceSessionsTableWidget->setColumnWidth(0, 150);
	ui.VoiceSessionsTableWidget->setColumnWidth(1, 50);

	this->setMinimumSize(this->size());
	this->setMaximumSize(this->size());

	m_refreshUiStatsTimer.setInterval(500);
	m_refreshUiStatsTimer.setSingleShot(false);

	this->connect(&m_refreshUiStatsTimer, SIGNAL(timeout()), this, SLOT(on_refreshUiStatsTimer_timeout()));
	m_refreshUiStatsTimer.start();
}


void MainWindow::on_ConnectPushButton_clicked(bool checked)
{
	this->ui.statusBar->showMessage("Connecting...");
	m_client = new Client{ QHostAddress{ this->ui.ServerIPAddressLineEdit->text() }, static_cast<quint16>(this->ui.ServerPortSpinBox->value()), this->ui.PasswordLineEdit->text(), this };

	this->connect(m_client, SIGNAL(newVoiceSession(quint64)), this, SLOT(on_ts3eClient_newVoiceSession(quint64)));

	if (m_client->setupAudioPlayback())
	{
		if (m_client->registerToServer())
		{
			this->ui.statusBar->showMessage("Success: connected to server");
		}

		else
		{
			this->ui.statusBar->showMessage("Error: could not connect to server");
		}
	}

	else
	{
		this->ui.statusBar->showMessage("Error: could not setup playback device");
	}
}

void MainWindow::on_refreshUiStatsTimer_timeout()
{
	if (m_client != nullptr)
	{
		ui.DecodedVoicePacketsNbLabel->setText(QString::number(m_client->getDecodedVoicePacketsNb()));
		ui.DecodedVoicePacketsBytesNbLabel->setText(QString::number(m_client->getDecodedVoicePacketsBytesNb()));
		ui.DecodingErrorsNbLabel->setText(QString::number(m_client->getDecodingErrorsNb()));
	}
}

void MainWindow::on_ts3eClient_newVoiceSession(quint64 sessionId)
{
	QCheckBox *voiceSessionListenCheckbox = new QCheckBox{};
	voiceSessionListenCheckbox->setProperty("VoiceSessionId", sessionId);

	ui.VoiceSessionsTableWidget->insertRow(ui.VoiceSessionsTableWidget->rowCount());
	ui.VoiceSessionsTableWidget->setItem(ui.VoiceSessionsTableWidget->rowCount() - 1, 0, new QTableWidgetItem{ QString::number(sessionId) });
	ui.VoiceSessionsTableWidget->setCellWidget(ui.VoiceSessionsTableWidget->rowCount() - 1, 1, voiceSessionListenCheckbox);

	this->connect(voiceSessionListenCheckbox, SIGNAL(stateChanged(int)), this, SLOT(on_voiceSessionListenCheckBox_stateChanged(int)));
}

void MainWindow::on_voiceSessionListenCheckBox_stateChanged(int state)
{
	quint64 sessionId = QObject::sender()->property("VoiceSessionId").toULongLong();

	m_client->setVoiceSessionState(sessionId, state == Qt::CheckState::Checked);
}
