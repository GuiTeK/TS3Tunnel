#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a{argc, argv};

    if (argc != 5)
    {
        qCritical() << "Usage: ./TS3TunnelServer TS3_INET_NAME TS3_VOICE_PORT PASSWORD PORT";
        return EXIT_FAILURE;
    }

    else
    {
        Server server{ QString::fromLatin1(argv[1]), QString::fromLatin1(argv[2]), QString::fromLatin1(argv[3]), QString{ argv[4] }.toUShort() };

        QTimer::singleShot(0, &server, SLOT(run()));

        return a.exec();
    }
}
