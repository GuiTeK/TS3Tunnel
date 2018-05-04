#pragma once

#include <QBuffer>
#include <QIODevice>
#include <QMutex>

class PlaybackAudioGenerator : public QIODevice
{
public:
	PlaybackAudioGenerator();

	virtual qint64 readData(char *data, qint64 maxSize) override;
	virtual qint64 writeData(const char *data, qint64 maxSize) override;


private:
	QMutex m_mutex;
	QBuffer m_buffer;
};
