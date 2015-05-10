#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T15:55:04
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mail
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mail.cpp

HEADERS  += mainwindow.h \
    mail.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoNet

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoNetd

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoNetSSL

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoNetSSLd

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoFoundation

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoFoundationd

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoUtil

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoUtild

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoCrypto

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lPocoCryptod

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
