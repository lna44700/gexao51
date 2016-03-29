#ifndef CAPTEUR_H
#define CAPTEUR_H

class Capteur;

#include <QString>
#include <QPixmap>
#include "../../TypeCommun.h"

class Capteur
{
public:
    Capteur(QString NomCapteur, QString NomBroche);  //Constructeur
    inline QPixmap getPhoto()   { return Photo; }   //Accesseur de la photo

private:
    QString Titre;
    QString Commande;
    QString Inconnue;
    QString Expression;
    QString Unit;
    QString NomBroche;
    QPixmap Photo;

};

#endif // CAPTEUR_H
