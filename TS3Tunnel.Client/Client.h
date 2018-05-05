#pragma once

#include <QObject>

#include <opus/opus.h>

#include <portaudio.h>

#include <QBuffer>
#include <QDataStream>
#include <QHash>
#include <QIODevice>
#include <QFile>
#include <QTimer>
#include <QUdpSocket>
#include <QtMultimedia/QAudioOutput>

#include "PlaybackAudioGenerator.h"

class MainWindow;


class Client : public QObject
{
	Q_OBJECT

public:
	enum class VoiceSessionCapability : int
	{
		Listen,
		Save
	};

	struct VoiceSession
	{
		quint64 Id;
		OpusDecoder *AudioDecoder;
		PlaybackAudioGenerator *AudioGenerator;
		PaStream *AudioStream;
		QFile *AudioSaveFile;
		bool ListenEnabled;
		bool SaveEnabled;
	};


	Client(const QHostAddress &serverAddress, quint16 serverPort, const QString &serverPassword, QObject *parent);
	~Client();

	int getDecodedVoicePacketsNb() const;
	int getDecodedVoicePacketsBytesNb() const;
	int getDecodingErrorsNb() const;

	bool setupAudioPlayback();
	bool registerToServer();

	void setAudioSavePath(const QString &path);
	void setVoiceSessionCapability(quint64 sessionId, VoiceSessionCapability capability, bool enabled);


signals:
	void newVoiceSession(quint64 sessionId);


private slots:
	void udpSocket_readyRead();
	void serverPingTimer_timeout();


private:
	void decodeVoiceDataStream(QDataStream &voiceDataStream, qint64 voicePacketBufferReserveSize);
	VoiceSession *updateVoiceSessionList(quint64 sessionId);

	static const int SERVER_PING_TIMER_INTERVAL_SEC = 2;
	static const QString PING_STR;

	static const int OPUS_CHANNEL_COUNT = 1;
	static const std::size_t OPUS_SAMPLE_SIZE = sizeof(opus_int16) * 8;
	static const opus_int32 OPUS_SAMPLE_RATE = 48000;
	static const int AUDIO_FRAME_SIZE = 960;


	QHostAddress m_serverAddress;
	quint16 m_serverPort;
	QString m_serverPassword;
	QUdpSocket *m_udpSocket;
	QTimer m_serverPingTimer;
	int m_decodedVoicePacketsNb;
	int m_decodedVoicePacketsBytesNb;
	int m_decodingErrorsNb;
	QHash<quint64, VoiceSession*> m_voiceSessions;
	QString m_audioSavePath;
};
