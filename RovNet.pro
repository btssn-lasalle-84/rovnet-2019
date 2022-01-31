#-------------------------------------------------
#
# Project created by QtCreator 2019-03-13T11:45:45
#
#-------------------------------------------------

QT       += core gui gamepad multimedia multimediawidgets serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RovNet
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        ihmrov.cpp \
    controlecamera.cpp \
    communicationrov.cpp \
    rov.cpp \
    archives.cpp \
    camera.cpp \
    manette.cpp \
    mesures.cpp \
    controlerov.cpp \
    bras.cpp \
    deplacement.cpp \
    basededonnees.cpp

HEADERS += \
        ihmrov.h \
    controlecamera.h \
    communicationrov.h \
    rov.h \
    archives.h \
    camera.h \
    mesures.h \
    manette.h \
    controlerov.h \
    bras.h \
    deplacement.h \
    basededonnees.h

RESOURCES += \
    ressources.qrc

DISTFILES += \
    rovnet.sqlite

# copie la base de données dans le dossier build où se trouve l'exécutable
CONFIG += file_copies
COPIES += bd
bd.files = rovnet.sqlite
bd.path = $$OUT_PWD/
bd.base = $$PWD/

INCLUDEPATH += /usr/include/qwt

LIBS += -lqwt-qt5
