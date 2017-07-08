#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T09:54:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = os
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    admin.cpp \
    getnewuser.cpp

HEADERS  += mainwindow.h \
    login.h \
    define.h \
    user.h \
    admin.h \
    getnewuser.h

FORMS    += mainwindow.ui \
    admin.ui \
    getnewuser.ui
