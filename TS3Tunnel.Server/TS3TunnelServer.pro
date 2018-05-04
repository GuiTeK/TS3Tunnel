QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

LIBS += -lpcap

SOURCES += \
        main.cpp \
    Server.cpp \
    Sniffer.cpp

HEADERS += \
    Server.h \
    Sniffer.h
