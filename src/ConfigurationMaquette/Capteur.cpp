#include <QSettings>
#include "Capteur.h"
#include <QDebug>

Capteur::Capteur(QString NomCapteur, QString NomBroche) :
    Photo   ("./IOCard/" + NomCapteur + "/photo.png"),
    NomCapteur(NomCapteur),
    NomBroche(NomBroche)
{

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
    this->Unite = Config.value("SUP/Unit").toString();

    if(this->NomBroche[0] == 'J')
    {
        this->NomBroche = this->NomBroche.remove(0,3);
    }

    if(this->NomBroche[0] == 'A')
    {
            this->NomBroche = this->NomBroche.remove(0,1);
    }
}
