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
