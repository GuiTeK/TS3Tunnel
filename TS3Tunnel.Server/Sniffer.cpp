#include "Sniffer.h"

#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include <pcap.h>

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QList>

#include "Server.h"

void packetSniffed(u_char *userData, const struct pcap_pkthdr *pcapHeader, const u_char *packet)
{
    Sniffer::SnifferData *snifferData = reinterpret_cast<Sniffer::SnifferData*>(userData);
    QList<Server::ClientInfo> *clients = reinterpret_cast<QList<Server::ClientInfo>*>(snifferData->Clients);
    const ether_header *ethernetHeader = (ether_header*)packet;
    const ip *ipHeader = (const ip*)((const u_char*)ethernetHeader + sizeof(ether_header));
    const udphdr *udpHeader = (const udphdr*)((const u_char*)ipHeader + sizeof(ip));
    const u_char *payload = (const u_char*)udpHeader + sizeof(udphdr);
    const u_char *voiceData = payload + sizeof(Sniffer::Ts3VoicePacketHeader);
    int32_t ethernetPaddingLength = pcapHeader->len - htons(ipHeader->ip_len) - sizeof(ether_header);
    int32_t payloadLength = pcapHeader->len - sizeof(ether_header) - sizeof(ip) - sizeof(udphdr) - ethernetPaddingLength;
    int32_t voiceDataLength = payloadLength >= sizeof(Sniffer::Ts3VoicePacketHeader) ? payloadLength - sizeof(Sniffer::Ts3VoicePacketHeader) : 0;

    if (payloadLength >= sizeof(Sniffer::Ts3VoicePacketHeader))
    {
        const Sniffer::Ts3VoicePacketHeader *voiceHeader = reinterpret_cast<const Sniffer::Ts3VoicePacketHeader*>(payload);

        if (voiceHeader->PacketType == Sniffer::Ts3VoicePacketType::Voice1 ||
                voiceHeader->PacketType == Sniffer::Ts3VoicePacketType::Voice2)
        {
            snifferData->Nb += 1;

            qDebug() << "==============================";
            qDebug().nospace() << "Packet length: " << pcapHeader->len << ", IP total length: " << htons(ipHeader->ip_len) << ", payload length: " << payloadLength << " (" << pcapHeader->len << " - " << sizeof(ether_header) << " - " << sizeof(ip) << " - " << sizeof(udphdr) << " - " << ethernetPaddingLength << "), voice data length: " << voiceDataLength << " (packet #" << snifferData->Nb << ")";
            qDebug() << "SessionId:" << voiceHeader->SessionId;
            qDebug() << "ControlPacketId:" << htons(voiceHeader->ControlPacketId);
            qDebug() << "ErrorCode:" << htons(voiceHeader->ErrorCode);
            qDebug() << "PacketType:" << static_cast<uint32_t>(voiceHeader->PacketType);
            qDebug() << "VoicePacketId:" << htons(voiceHeader->VoicePacketId);
            qDebug() << "CodecType:" << static_cast<uint32_t>(voiceHeader->CodecType);
            qDebug() << "==============================";

            snifferData->Mutex->lock();
            for (auto it = clients->begin(); it != clients->end(); ++it)
            {
                QByteArray voiceDataBuffer{};
                QDataStream voiceDataStream{ &voiceDataBuffer, QIODevice::WriteOnly };

                voiceDataStream << static_cast<quint16>(voiceDataLength);
                voiceDataStream << static_cast<quint64>(voiceHeader->SessionId);
                voiceDataStream.writeRawData(reinterpret_cast<const char*>(voiceData), voiceDataLength);

                snifferData->Socket->writeDatagram(voiceDataBuffer, it->Address, it->Port);
            }
            snifferData->Mutex->unlock();
        }
    }
}


Sniffer::Sniffer(const QString &interfaceName, const QString &ts3VoicePort, QUdpSocket *socket, QList<Server::ClientInfo> *clients, QMutex *mutex, QObject *parent) : QObject{ parent },
    m_interfaceName{ interfaceName },
    m_ts3VoicePort{ ts3VoicePort },
    m_snifferData{}
{
    m_snifferData.Nb = 0;
    m_snifferData.Socket = socket;
    m_snifferData.Clients = clients;
    m_snifferData.Mutex = mutex;
}


void Sniffer::run()
{
    const char *devName = m_interfaceName.toStdString().c_str();
    const int PACKET_BUFFER_TIMEOUT_MS = 200;
    char errorBuffer[PCAP_ERRBUF_SIZE];
    pcap_t *devHandle;
    struct bpf_program filterProgram;
    int result;

    devHandle = pcap_open_live(devName, BUFSIZ, false, PACKET_BUFFER_TIMEOUT_MS, errorBuffer);

    if (devHandle != nullptr)
    {
        QString filterProgramStr = "udp port ";

        filterProgramStr += m_ts3VoicePort;
        qInfo() << "Successfully opened device" << devName;

        result = pcap_compile(devHandle, &filterProgram, filterProgramStr.toStdString().c_str(), true, PCAP_NETMASK_UNKNOWN);

        if (result == 0)
        {
            qDebug() << "Successfully compiled filter program";

            result = pcap_setfilter(devHandle, &filterProgram);

            if (result == 0)
            {
                qDebug() << "Successfully set filter program";

                result = pcap_loop(devHandle, -1, packetSniffed, reinterpret_cast<u_char *>(&m_snifferData));

                if (result == -1)
                {
                    pcap_perror(devHandle, "pcap_loop failed: ");
                }

                pcap_close(devHandle);
            }

            else
            {
                pcap_perror(devHandle, "Error while setting filter program: ");
            }
        }

        else
        {
            pcap_perror(devHandle, "Error while compiling filter program: ");
        }
    }

    else
    {
        qCritical() << "Failed to open device" << devName;
    }
}
