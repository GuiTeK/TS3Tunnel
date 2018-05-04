#include "Client.h"

#include <QDebug>

#include "PlaybackAudioGenerator.h"

const QString Client::PING_STR = "Ping";

/*int myPaStreamCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	PlaybackAudioGenerator *playbackAudioGenerator = reinterpret_cast<PlaybackAudioGenerator*>(userData);
	unsigned long audioBufferSize = frameCount * sizeof(opus_int16);

	playbackAudioGenerator->read(reinterpret_cast<char*>(output), audioBufferSize);

	return 0;
}*/

Client::Client(const QHostAddress &serverAddress, quint16 serverPort, const QString &serverPassword, QObject *parent) : QObject{ parent },
m_serverAddress{ serverAddress },
m_serverPort{ serverPort },
m_serverPassword{ serverPassword },
m_udpSocket{ new QUdpSocket{ this } },
m_serverPingTimer{},
m_audioOutput{ nullptr },
m_playbackDevice{ nullptr },
m_opusDecoder{ nullptr },
m_decodedVoicePacketsNb{ 0 },
m_decodedVoicePacketsBytesNb{ 0 },
m_decodingErrorsNb{ 0 },
m_voiceSessions{}/*,
m_paStream{ nullptr }*/
{
	this->connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(udpSocket_readyRead()));

	m_serverPingTimer.setInterval(SERVER_PING_TIMER_INTERVAL_SEC * 1000);
	m_serverPingTimer.setSingleShot(false);
	this->connect(&m_serverPingTimer, SIGNAL(timeout()), this, SLOT(serverPingTimer_timeout()));
}

Client::~Client()
{
	if (m_opusDecoder != nullptr)
	{
		opus_decoder_destroy(m_opusDecoder);
	}
}


int Client::getDecodedVoicePacketsNb() const
{
	return m_decodedVoicePacketsNb;
}

int Client::getDecodedVoicePacketsBytesNb() const
{
	return m_decodedVoicePacketsBytesNb;
}

int Client::getDecodingErrorsNb() const
{
	return m_decodingErrorsNb;
}


bool Client::setupAudioPlayback()
{
	QAudioFormat format{};
	int opusResult = 0;

	format.setSampleRate(OPUS_SAMPLE_RATE);
	format.setChannelCount(OPUS_CHANNEL_COUNT);
	format.setSampleSize(OPUS_SAMPLE_SIZE);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SampleType::SignedInt);

	m_audioOutput = new QAudioOutput(format, this);
	m_audioOutput->setBufferSize(PLAYBACK_AUDIO_BUFFER_SIZE);
	//Pa_Initialize();

	m_playbackDevice = new PlaybackAudioGenerator{};
	//Pa_OpenDefaultStream(&m_paStream, 0, OPUS_CHANNEL_COUNT, paInt16, OPUS_SAMPLE_RATE, 256, myPaStreamCallback, m_playbackDevice);

	m_audioOutput->start(m_playbackDevice);
	//Pa_StartStream(m_paStream);

	qDebug().nospace() << "Using " << m_audioOutput->bufferSize() << "-byte audio buffer, period size is " << m_audioOutput->periodSize() << " bytes, notify interval is " << m_audioOutput->notifyInterval() << " ms";

	m_opusDecoder = opus_decoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNEL_COUNT, &opusResult);

	return m_playbackDevice != nullptr && m_opusDecoder != nullptr;
}

bool Client::registerToServer()
{
	QByteArray registrationData{ m_serverPassword.toStdString().c_str() };
	bool udpSocketBindResult = m_udpSocket->bind(QHostAddress::AnyIPv4);
	qint64 registrationResult = m_udpSocket->writeDatagram(registrationData, m_serverAddress, m_serverPort);

	m_serverPingTimer.start();

	qDebug().nospace() << "Local UDP socket bound on " << m_udpSocket->localAddress() << ":" << m_udpSocket->localPort();

	return udpSocketBindResult == true && registrationResult == registrationData.size();
}


void Client::setVoiceSessionState(quint64 sessionId, bool enabled)
{
	auto it = m_voiceSessions.find(sessionId);

	if (it != m_voiceSessions.end())
	{
		it->Enabled = enabled;
	}
}


void Client::udpSocket_readyRead()
{
	QByteArray voiceDataBuffer{};
	QDataStream voiceDataStream{ &voiceDataBuffer, QIODevice::ReadOnly };
	QHostAddress senderAddress;
	quint16 senderPort;
	char *voicePacketBuffer = new char[m_udpSocket->pendingDatagramSize()];
	int opusResult = 0;

	voiceDataBuffer.resize(m_udpSocket->pendingDatagramSize());
	m_udpSocket->readDatagram(voiceDataBuffer.data(), voiceDataBuffer.size(), &senderAddress, &senderPort);

	this->decodeVoiceDataStream(voiceDataStream, m_playbackDevice, voiceDataBuffer.size());
}

void Client::serverPingTimer_timeout()
{
	m_udpSocket->writeDatagram(PING_STR.toStdString().c_str(), PING_STR.length(), m_serverAddress, m_serverPort);
}


void Client::decodeVoiceDataStream(QDataStream &voiceDataStream, QIODevice *outputDevice, qint64 voicePacketBufferReserveSize)
{
	char *voicePacketBuffer = new char[voicePacketBufferReserveSize];

	while (!voiceDataStream.atEnd())
	{
		quint16 voicePacketLength = 0;
		quint64 voiceSessionId;
		int opusResult = 0;
		opus_int16 pcm[OPUS_FRAME_SIZE * OPUS_CHANNEL_COUNT * sizeof(opus_int16)];

		voiceDataStream >> voicePacketLength;
		voiceDataStream >> voiceSessionId;
		voiceDataStream.readRawData(voicePacketBuffer, voicePacketLength);

		if (this->updateVoiceSessionList(voiceSessionId))
		{
			opusResult = opus_decode(m_opusDecoder, reinterpret_cast<unsigned char*>(voicePacketBuffer), voicePacketLength, pcm, OPUS_FRAME_SIZE, 0);

			if (opusResult <= 0)
			{
				m_decodingErrorsNb += 1;
			}

			else
			{
				qint64 decodedBytes = opusResult * OPUS_CHANNEL_COUNT * sizeof(opus_int16);

				m_decodedVoicePacketsNb += 1;
				m_decodedVoicePacketsBytesNb += decodedBytes;

				outputDevice->write(reinterpret_cast<char*>(pcm), decodedBytes);
			}
		}
	}
}

bool Client::updateVoiceSessionList(quint64 sessionId)
{
	auto it = m_voiceSessions.find(sessionId);

	if (it == m_voiceSessions.end())
	{
		VoiceSession voiceSession{};

		voiceSession.Id = sessionId;
		voiceSession.Enabled = false;

		m_voiceSessions.insert(sessionId, voiceSession);

		emit this->newVoiceSession(sessionId);

		return voiceSession.Enabled;
	}

	return it->Enabled;
}
