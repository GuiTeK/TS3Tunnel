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
