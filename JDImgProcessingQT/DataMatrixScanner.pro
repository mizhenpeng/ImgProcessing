#-------------------------------------------------
#
# Project created by QtCreator 2016-02-11T22:34:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zBarTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    binarize.cpp \
    util.cpp \
    Processing.cpp \
    Scanner.cpp \
    Decoder.cpp \
    DataBank.cpp


HEADERS  += mainwindow.h \
    binarize.h \
    util.h \
    ui_mainwindow.h \
    Processing.h \
    globaldefine.h \
    Scanner.h \
    Decoder.h \
    DataBank.h



FORMS    += mainwindow.ui
