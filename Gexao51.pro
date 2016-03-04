#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T23:57:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gexao51
TEMPLATE = app

include(librairie\qextserialport\qextserialport.pri)

SOURCES += main.cpp\
        F_Principale.cpp \
    Arduino.cpp \
    F_Sonde.cpp \
    F_ChoisirSonde.cpp \
    EnfantSonde.cpp

HEADERS  += F_Principale.h \
    Arduino.h \
    F_Sonde.h \
    F_ChoisirSonde.h \
    EnfantSonde.h

FORMS    += \
    F_Principale.ui \
    F_Sonde.ui \
    F_ChoisirSonde.ui
