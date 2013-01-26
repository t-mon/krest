#-------------------------------------------------
#
# Project created by QtCreator 2012-07-24T22:58:32
#
#-------------------------------------------------

QT       += core gui network

TARGET = krest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    requestitem.cpp \
    requestitemmodel.cpp \
    bookmarkmodel.cpp \
    core.cpp

HEADERS  += mainwindow.h \
    requestitem.h \
    requestitemmodel.h \
    bookmarkmodel.h \
    core.h

FORMS    += mainwindow.ui
