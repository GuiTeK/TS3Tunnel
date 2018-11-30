/*
* MIT License
*
* Copyright (c) 2018 Guillaume Truchot - guillaume.truchot@outlook.com
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "MainWindow.h"

#include <QCheckBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow{ parent },
m_client{ nullptr },
m_refreshUiStatsTimer{}
{
	ui.setupUi(this);

	ui.VoiceSessionsTableWidget->setColumnCount(3);
	ui.VoiceSessionsTableWidget->setHorizontalHeaderLabels(QStringList{ "Session ID", "Listen", "Save" });
	ui.VoiceSessionsTableWidget->setColumnWidth(0, 150);
	ui.VoiceSessionsTableWidget->setColumnWidth(1, 50);
	ui.VoiceSessionsTableWidget->setColumnWidth(2, 50);

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

	m_client->setAudioSavePath(this->ui.StorageFolderLineEdit->text());
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

void MainWindow::on_StorageFolderBrowsePushButton_clicked(bool checked)
{
	QString storageFolder = QFileDialog::getExistingDirectory(this, "TS3Tunnel - Audio Storage Folder", QString{}, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	this->ui.StorageFolderLineEdit->setText(storageFolder);
}

void MainWindow::on_StorageFolderLineEdit_textChanged(const QString &text)
{
	if (m_client)
	{
		m_client->setAudioSavePath(this->ui.StorageFolderLineEdit->text());
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
	QCheckBox *voiceSessionSaveCheckbox = new QCheckBox{};

	voiceSessionListenCheckbox->setProperty("VoiceSessionId", sessionId);
	voiceSessionListenCheckbox->setProperty("VoiceSessionCapability", static_cast<int>(Client::VoiceSessionCapability::Listen));

	voiceSessionSaveCheckbox->setProperty("VoiceSessionId", sessionId);
	voiceSessionSaveCheckbox->setProperty("VoiceSessionCapability", static_cast<int>(Client::VoiceSessionCapability::Save));

	ui.VoiceSessionsTableWidget->insertRow(ui.VoiceSessionsTableWidget->rowCount());
	ui.VoiceSessionsTableWidget->setItem(ui.VoiceSessionsTableWidget->rowCount() - 1, 0, new QTableWidgetItem{ QString::number(sessionId) });
	ui.VoiceSessionsTableWidget->setCellWidget(ui.VoiceSessionsTableWidget->rowCount() - 1, 1, voiceSessionListenCheckbox);
	ui.VoiceSessionsTableWidget->setCellWidget(ui.VoiceSessionsTableWidget->rowCount() - 1, 2, voiceSessionSaveCheckbox);

	this->connect(voiceSessionListenCheckbox, SIGNAL(stateChanged(int)), this, SLOT(on_voiceSessionCapabilityCheckBox_stateChanged(int)));
	this->connect(voiceSessionSaveCheckbox, SIGNAL(stateChanged(int)), this, SLOT(on_voiceSessionCapabilityCheckBox_stateChanged(int)));
}

void MainWindow::on_voiceSessionCapabilityCheckBox_stateChanged(int state)
{
	quint64 sessionId = QObject::sender()->property("VoiceSessionId").toULongLong();
	Client::VoiceSessionCapability capability = static_cast<Client::VoiceSessionCapability>(QObject::sender()->property("VoiceSessionCapability").toInt());

	m_client->setVoiceSessionCapability(sessionId, capability, state == Qt::CheckState::Checked);
}
