#include <QSettings>
#include "CapteurActionneur.h"
#include <QDebug>

CapteurActionneur::CapteurActionneur(QString NomCapteur, QString NomBroche) :
    Photo   ("./IOCard/" + NomCapteur + "/photo.png")
{
    qDebug()<<NomCapteur;
    qDebug()<<NomBroche;

    //Broche
    this->NomBroche = NomBroche;

    QSettings   Config("./IOCard/" + NomCapteur + "/config.ini", QSettings::IniFormat);

    //Le Titre
    this->Titre = Config.value("TITRE/NOM").toString();

    //Commande
    this->Commande = Config.value("SUP/Commande").toString();

    //Inconnue
    this->Inconnue = Config.value("SUP/Inconnue").toString();

    //Expression
    this->Expression = Config.value("SUP/Expression").toString();

    //Unit
    this->Unit = Config.value("SUP/Unit").toString();

}
