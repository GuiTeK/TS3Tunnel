#pragma once

#include <QObject>

#include <QList>
#include <QMutex>
#include <QTimer>
#include <QThread>
#include <QtNetwork/QUdpSocket>

class Sniffer;


class Server : public QObject
{
    Q_OBJECT

public:
    Server(const QString &ts3InetName, const QString &ts3VoicePort, const QString &password, quint16 port, QObject *parent = nullptr);


    struct ClientInfo
    {
        QHostAddress Address;
        quint16 Port;
        qint64 LastPing;
    };


public slots:
    void run();


private slots:
    void udpSocket_readyRead();
    void clientDisconnectionTimer_timeout();


private:
    static const int CLIENT_DISCONNECTION_TIMER_INTERVAL_SEC = 5;
    static const QString PING_STR;

    QString m_password;
    quint16 m_port;
    QUdpSocket *m_udpSocket;
    QList<ClientInfo> m_clients;
    QMutex m_mutex;
    QTimer m_clientDisconnectionTimer;
    QThread m_snifferThread;
    Sniffer *m_sniffer;
};
