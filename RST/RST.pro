#-------------------------------------------------
#
# Project created by QtCreator 2013-08-28T15:50:55
#
#-------------------------------------------------

QT       += core gui\
            printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RST
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    algorithm/PointsGenerator.cpp \
    algorithm/SMST.cpp \
    algorithm/ZMST.cpp \
    mst.cpp \
    algorithm/LMST.cpp \
    data.cpp \
    donewdialog.cpp \
    paintarea.cpp

HEADERS  += mainwindow.h \
    algorithm/data_format.h \
    algorithm/overlap.h \
    algorithm/PointsGenerator.h \
    algorithm/SMST.h \
    algorithm/ZMST.h \
    mst.h \
    algorithm/LMST.h \
    data.h \
    donewdialog.h \
    paintarea.h

FORMS    += mainwindow.ui \
    donewdialog.ui

RESOURCES += \
    icons.qrc
