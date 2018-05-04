#include "Server.h"

#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

#include "Sniffer.h"


const QString Server::PING_STR = "Ping";


Server::Server(const QString &ts3InetName, const QString &ts3VoicePort, const QString &password, quint16 port, QObject *parent) : QObject{ parent },
    m_password{ password },
    m_port{ port },
    m_udpSocket{ new QUdpSocket{ this } },
    m_clients{},
    m_mutex{},
    m_clientDisconnectionTimer{},
    m_snifferThread{},
    m_sniffer{ new Sniffer{ ts3InetName, ts3VoicePort, m_udpSocket, &m_clients, &m_mutex } }
{
    m_clientDisconnectionTimer.setInterval(CLIENT_DISCONNECTION_TIMER_INTERVAL_SEC * 1000);
    m_clientDisconnectionTimer.setSingleShot(false);

    this->connect(&m_clientDisconnectionTimer, SIGNAL(timeout()), this, SLOT(clientDisconnectionTimer_timeout()));
}


void Server::run()
{
    qInfo() << "Running TS3 Tunnel Server on port" << m_port;

    m_udpSocket->bind(QHostAddress::AnyIPv4, m_port);
    this->connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(udpSocket_readyRead()));

    m_clientDisconnectionTimer.start();

    m_sniffer->moveToThread(&m_snifferThread);
    this->connect(&m_snifferThread, SIGNAL(finished()), m_sniffer, SLOT(deleteLater()));
    m_snifferThread.start();
    QTimer::singleShot(0, m_sniffer, SLOT(run()));
}


void Server::udpSocket_readyRead()
{
    QByteArray dataBuffer{};
    QHostAddress senderAddress{};
    quint16 senderPort = 0;

    m_mutex.lock();

    dataBuffer.resize(m_udpSocket->pendingDatagramSize());
    m_udpSocket->readDatagram(dataBuffer.data(), dataBuffer.size(), &senderAddress, &senderPort);

    if (dataBuffer.size() == PING_STR.size() && QString::fromLatin1(dataBuffer.data(), dataBuffer.size()) == PING_STR)
    {
        for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
        {
            if (it->Address == senderAddress && it->Port == senderPort)
            {
                it->LastPing = QDateTime::currentDateTime().toTime_t();
                break;
            }
        }

        qDebug().nospace() << "Client ping from " << senderAddress << ":" << senderPort;
    }

    else
    {
        if (dataBuffer.size() == m_password.size() && QString::fromLatin1(dataBuffer.data(), dataBuffer.size()) == m_password)
        {
            ClientInfo clientInfo{};

            clientInfo.Address = senderAddress;
            clientInfo.Port = senderPort;
            clientInfo.LastPing = QDateTime::currentDateTime().toTime_t();

            m_clients.append(clientInfo);
            qInfo().nospace() << "Client connected from " << senderAddress << ":" << senderPort;
        }

        else
        {
            qInfo().nospace() << "Bad password from " << senderAddress << ":" << senderPort;
        }
    }

    m_mutex.unlock();
}

void Server::clientDisconnectionTimer_timeout()
{
    QList<ClientInfo>::iterator it{};

    m_mutex.lock();
    it = m_clients.begin();

    while (it != m_clients.end())
    {
        qint64 currentTime = QDateTime::currentDateTime().toTime_t();

        if (currentTime > it->LastPing + CLIENT_DISCONNECTION_TIMER_INTERVAL_SEC)
        {
            qInfo() << "Client disconnected from " << it->Address << ":" << it->Port;

            it = m_clients.erase(it);
        }

        else
        {
            ++it;
        }
    }

    m_mutex.unlock();
}
