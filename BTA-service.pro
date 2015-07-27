#-------------------------------------------------
#
# Project created by QtCreator 2015-07-27T10:39:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BTA-service
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        bta_service.cpp \
    pf_table.cpp

HEADERS  += bta_service.h \
    pf_table.h
