#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T15:55:04
#
#-------------------------------------------------

QT       += core gui
QT       += sql

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mail
TEMPLATE = app

QTPLUGIN += QMYSQL

SOURCES += main.cpp\
        mainwindow.cpp \
    mail.cpp \
    filemimetype.cpp \
    fileexplorerdialog.cpp \
    myparthandler.cpp \
    mystring.cpp \
    clientinfodialog.cpp \
    emailsenddialog.cpp \
    emailsendfaildialog.cpp \
    addcontactdialog.cpp

HEADERS  += mainwindow.h \
    mail.h \
    filemimetype.h \
    fileexplorerdialog.h \
    myparthandler.h \
    mystring.h \
    utf8/checked.h \
    utf8/core.h \
    utf8/unchecked.h \
    utf8.h \
    clientinfodialog.h \
    emailsenddialog.h \
    emailsendfaildialog.h \
    addcontactdialog.h

FORMS    += mainwindow.ui \
    fileexplorerdialog.ui \
    clientinfodialog.ui \
    emailsenddialog.ui \
    emailsendfaildialog.ui \
    addcontactdialog.ui

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

unix:!macx: LIBS += -L$$PWD/lib/ -lmagic

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
