QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = convolution-testbench
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    convolute.cpp \
    convolutionkerneldialog.cpp

HEADERS  += mainwindow.h \
    convolute.h \
    convolutionkerneldialog.h

FORMS    += mainwindow.ui \
    convolutionkerneldialog.ui
