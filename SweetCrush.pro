QT -= gui
QT += core

CONFIG += console cmdline
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    bits.h \
    tablero.h

SOURCES += \
    main.cpp \
    bits.cpp \
    tablero.cpp
