#-------------------------------------------------
#
# Project created by QtCreator 2016-04-21T21:24:09
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
    ConfigurationMaquette/ListeCapteurVue.cpp \
    ConfigurationMaquette/ScenePlanDeCablage.cpp \
    ConfigurationMaquette/ItemBroche.cpp \
    ConfigurationMaquette/Photo.cpp \
    ConfigurationMaquette/Capteur.cpp \
    F_Calibrage.cpp \
    F_ListeCalibrage.cpp


HEADERS  += F_Principale.h \
    Arduino.h \
    F_ChoisirSonde.h \
    F_Sonde.h \
    ConfigurationMaquette/ListeCapteurVue.h \
    ConfigurationMaquette/ScenePlanDeCablage.h \
    ConfigurationMaquette/ItemBroche.h \
    ConfigurationMaquette/Photo.h \
    ConfigurationMaquette/Capteur.h \
    F_Calibrage.h \
    F_ListeCalibrage.h

FORMS    += \
    F_Principale.ui \
    F_ChoisirSonde.ui \
    F_Sonde.ui \
    F_Calibrage.ui \
    F_ListeCalibrage.ui

DISTFILES +=

RESOURCES += \
    icones.qrc
