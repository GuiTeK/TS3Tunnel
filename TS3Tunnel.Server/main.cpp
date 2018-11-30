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
