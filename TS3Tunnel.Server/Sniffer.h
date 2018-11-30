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
#pragma once

#include <QObject>

#include <cstdint>

#include <QMutex>
#include <QString>
#include <QtNetwork/QUdpSocket>

#include "Server.h"


class Sniffer : public QObject
{
    Q_OBJECT

public:
    Sniffer(const QString &interfaceName, const QString &ts3VoicePort, QUdpSocket *socket, QList<Server::ClientInfo> *clients, QMutex *mutex, QObject *parent = nullptr);


    enum class Ts3VoicePacketType : uint8_t
    {
        ControlShort = 0x84,
        ControlLong = 0x85,
        Voice1 = 0x80,
        Voice2 = 0xc0
    };

    enum class Ts3VoiceCodecType : uint8_t
    {
        SpeexNarrowband = 0x00,
        SpeexWideband = 0x01,
        SpeexUltraWideband = 0x02,
        CeltMono = 0x03,
        OpusVoice = 0x04,
        OpusMusic = 0x05
    };

    #pragma pack(push, 1)
    struct Ts3VoicePacketHeader
    {
        uint64_t			SessionId;
        uint16_t			ControlPacketId;
        uint16_t			ErrorCode;
        Ts3VoicePacketType	PacketType;
        uint16_t			VoicePacketId;
        Ts3VoiceCodecType	CodecType;
    };
    #pragma pack(pop)

    struct SnifferData
    {
        int Nb;
        QUdpSocket *Socket;
        QList<Server::ClientInfo> *Clients;
        QMutex *Mutex;
    };


public slots:
    void run();


private:
    QString m_interfaceName;
    QString m_ts3VoicePort;
    SnifferData m_snifferData;
};
