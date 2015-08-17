#-------------------------------------------------
#
# Project created by QtCreator 2015-07-27T10:39:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = BTA-service
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        bta_service.cpp \
    pf_table.cpp \
    qfitsheadereditor.cpp \
    focussing_widget.cpp \
    qcustomplot.cpp \
    plot_region_widget.cpp \
    psf_dialog.cpp \
    psf_fitting.cpp

HEADERS  += bta_service.h \
    pf_table.h \
    qfitsheadereditor.h \
    focussing_widget.h \
    qcustomplot.h \
    plot_region_widget.h \
    psf_dialog.h \
    psf_defs.h

unix:CONFIG(release,release|debug) LIBS += -L$$PWD/../build-fits_viewer-QT5-Release/ -lfits_viewer
unix:CONFIG(debug,release|debug) LIBS += -L$$PWD/../build-fits_viewer-QT5-Debug/ -lfits_viewer

INCLUDEPATH += $$PWD/../fits_viewer
DEPENDPATH += $$PWD/../fits_viewer

unix:!macx|win32: LIBS += -lcfitsio

unix:!macx: LIBS += -llevmar

unix:!macx: LIBS += -lclapack
