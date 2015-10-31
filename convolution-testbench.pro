QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = convolution-testbench
TEMPLATE = app
CONFIG += c++11

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/convolution.cpp

HEADERS  += src/mainwindow.h \
    src/convolution.h

FORMS    += src/mainwindow.ui
