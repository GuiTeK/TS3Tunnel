#include "PlaybackAudioGenerator.h"

#include <cstring>

PlaybackAudioGenerator::PlaybackAudioGenerator() : QIODevice{}
{
	this->open(QIODevice::ReadWrite);
	m_buffer.open(QIODevice::ReadWrite);
}

qint64 PlaybackAudioGenerator::readData(char *data, qint64 maxSize)
{
	QMutexLocker locker{ &m_mutex };
	qint64 bytesAvailable = m_buffer.bytesAvailable();

	bytesAvailable = bytesAvailable >= maxSize ? maxSize : bytesAvailable;

	m_buffer.read(data, bytesAvailable);
	std::memset(data + bytesAvailable, 0, maxSize - bytesAvailable);

	return maxSize;
}

qint64 PlaybackAudioGenerator::writeData(const char *data, qint64 maxSize)
{
	QMutexLocker locker{ &m_mutex };
	qint64 currentPos = m_buffer.pos();
	qint64 writeResult = 0;

	m_buffer.seek(m_buffer.size());
	writeResult = m_buffer.write(data, maxSize);
	m_buffer.seek(currentPos);

	return writeResult;
}
