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
