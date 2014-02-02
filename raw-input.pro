#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T15:34:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RawInputData
TEMPLATE = app

*-g++* {
QMAKE_CXXFLAGS += -std=gnu++11
}


SOURCES += main.cpp \
    win_specific.cpp

#HEADERS  += \
#    tray_icon.h \
#    activity_driver.h

#RESOURCES += \
#    resources.qrc

HEADERS += \
    raw_input.h \
    input_catcher.h