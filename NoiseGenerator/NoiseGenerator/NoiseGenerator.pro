TEMPLATE = app
TARGET = NoiseGenerator

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    window.cpp \
    noisemap.cpp

HEADERS += \
    allinc.h \
    window.h \
    noisemap.h \
    defines.h
