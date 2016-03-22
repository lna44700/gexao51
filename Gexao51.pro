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
    ConfigurationMaquette/ListeCapteurVue.cpp \
    ConfigurationMaquette/ScenePlanDeCablage.cpp \
    ConfigurationMaquette/ItemBroche.cpp \
    ConfigurationMaquette/Photo.cpp \
    ConfigurationMaquette/Capteur.cpp \


HEADERS  += F_Principale.h \
    Arduino.h \
    F_Sonde.h \
    F_ChoisirSonde.h \
    ConfigurationMaquette/ListeCapteurVue.h \
    ConfigurationMaquette/ScenePlanDeCablage.h \
    ConfigurationMaquette/ItemBroche.h \
    ConfigurationMaquette/Photo.h \
    ConfigurationMaquette/Capteur.h \
    TypeCommun.h \

FORMS    += \
    F_Principale.ui \
    F_Sonde.ui \
    F_ChoisirSonde.ui

DISTFILES +=

RESOURCES += \
    icones.qrc
