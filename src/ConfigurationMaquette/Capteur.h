#ifndef CAPTEUR_H
#define CAPTEUR_H

class Capteur;

#include <QString>
#include <QPixmap>
#include <QSettings>
#include "../F_Calibrage.h"

class Capteur
{
public:
    Capteur(Arduino *oMonArduino, QString sNomCapteur, QString sNomBroche);  //Constructeur

    bool            Get_bCalibrageNecessaire();
    QString         Get_sNomCapteur();
    QString         Get_sNomBroche();
    QString         Get_sTitre();
    QString         Get_sCommande();
    QString         Get_sInconnue();
    QString         Get_sExpression();
    QString         Get_sUnite();
    QPixmap         Get_pxmPhoto();
    F_Calibrage     *Get_oMonCalibrage();
    bool            Get_bCapteurCalibre();

    void            Set_bCapteurCalibre(bool bCapteurCalibre);

private:
    QString         sNomCapteur;
    QString         sNomBroche;
    QString         sTitre;
    QString         sCommande;
    QString         sInconnue;
    QString         sExpression;
    QString         sUnite;
    QPixmap         pxmPhoto;
    QSettings       stgConfiguration;
    F_Calibrage     *oMonCalibrage;
    bool            bCapteurCalibre;
    bool            bCalibrageNecessaire;
    Arduino         *oMonArduino;

};

#endif // CAPTEUR_H
