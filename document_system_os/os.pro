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
    getnewuser.cpp \
    deleteolduser.cpp \
    file_system.cpp \
    file.cpp

HEADERS  += mainwindow.h \
    define.h \
    user.h \
    admin.h \
    getnewuser.h \
    deleteolduser.h \
    file_system.h

FORMS    += mainwindow.ui \
    admin.ui \
    getnewuser.ui \
    deleteolduser.ui \
    file_system.ui
