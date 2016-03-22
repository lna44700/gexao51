//CapteurActionneur.h   1.0     14/12/14    N.Jarnoux
#ifndef CAPTEURACTIONNEUR_H
#define CAPTEURACTIONNEUR_H

class CapteurActionneur;

//=== Headers standards ===
#include <QString>
#include <QPixmap>

//==== Headers  persos ====
#include "../../TypeCommun.h"

class CapteurActionneur
{
public:
    CapteurActionneur(QString NomCapteur, QString NomBroche);  //Constructeur
    inline QPixmap getPhoto()   { return Photo; }   //Accesseur de la photo
    //inline QString getNom()     { return Nom; }     //Accesseur du nom
    //inline QString getTitre()   { return Titre; }   //Accesseur du titre
    //inline QString getBroche()  { return Broche; }  //Accesseur de la broche
    //inline QString getType()    { return Type; }    //Accesseur du type
    //inline QString getCarte()   { return Carte; }   //Accesseur de la carte ou il est branché
    //inline QString getModule()  { return Module; }  //Accesseur du module
    //inline QString getARU()     { return ARU; }     //Accesseur d'arret d'urgence

private:
    QString Titre;
    QString Commande;
    QString Inconnue;
    QString Expression;
    QString Unit;
    QString NomBroche;
    //QString Type;
    //QString Nom;
    //QString Broche;
    //QString Carte;
    QPixmap Photo;

};

#endif // CAPTEURACTIONNEUR_H
