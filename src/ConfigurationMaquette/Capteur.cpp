//-------------------------------------------------------------------------------
/**
 * @file        Capteur.cpp
 * @brief       Récupération des données dans les fichiers .ini
 *
 * @author      S.GUICHARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        27/04/16
 *
 * Lors de l'appel au constructeur paramétré de la classe Capteur, un nom de capteur
 * et un nom de broche lui est attribué. Grâce au nom du capteur, le fichier de configuration
 * du capteur va être parcourru, et permettre de remplir les attributs de la classe qui
 * vont servir pour le calibrage et l'acquisition.
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "F_Calibrage.h"
#include "Capteur.h"

//=====   En-Têtes standards    =====
#include <QSettings>
#include <QDebug>

//-------------------------------------------------------------------------------

/**
 * Constructeur paramétré
 * @param *oMonArduino Pointeur de l'objet Arduino créé dans le fichier d'application F_Principale
 * @param sNomCapteur QString contenant le nom du répertoire correspondant à l'objet capteur qui est à créer
 * @param sNomBroche QString contenant le nom de la broche sur laquelle sera connecté l'objet capteur
 */
Capteur::Capteur(Arduino *oMonArduino, QString sNomCapteur, QString sNomBroche) :
    pxmPhoto   ("./IOCard/" + sNomCapteur + "/photo.png"),
    sNomCapteur(sNomCapteur),
    sNomBroche(sNomBroche),
    stgConfiguration ("./IOCard/" + sNomCapteur + "/config.ini", QSettings::IniFormat),
    bCapteurCalibre(false),
    bCalibrageNecessaire(false)
{
    //Le Titre
    this->sTitre = this->stgConfiguration.value("TITRE/NOM").toString();

    //Commande
    this->sCommande = this->stgConfiguration.value("SUP/Commande").toString();

    //Inconnue
    this->sInconnue = this->stgConfiguration.value("SUP/Inconnue").toString();

    //Expression
    this->sExpression = this->stgConfiguration.value("SUP/Expression").toString();

    //Unit
    this->sUnite = this->stgConfiguration.value("SUP/Unit").toString();

    if(this->sNomBroche[0] == 'J')
    {
        this->sNomBroche = this->sNomBroche.remove(0,3);
    }

    if(this->sNomBroche[0] == 'A')
    {
            this->sNomBroche = this->sNomBroche.remove(0,1);
    }

    if(this->stgConfiguration.value("Calibrage/Calibrage").toString() == "oui")
    {
        this->oMonCalibrage = new F_Calibrage(oMonArduino);
        this->bCalibrageNecessaire = true;
    }

}

//-------------------------------------------------------------------------------

/**
 * Accesseur du nom de l'objet capeur
 * @return sNomCapteur QString contenant le nom de l'onjet capteur
 */
QString Capteur::Get_sNomCapteur()
{
    return this->sNomCapteur;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur du nom de la broche
 * @return sNomBroche QString contenant la broche sur laquelle l'objet capteur est connecté
 */
QString Capteur::Get_sNomBroche()
{
    if(!this->sNomBroche.isEmpty())
    {
        return this->sNomBroche;
    }
}

//-------------------------------------------------------------------------------

/**
 * Accesseur du titre de l'objet capteur
 * @return sTitre QString contenant le titre de l'objet capteur
 */
QString Capteur::Get_sTitre()
{
    return this->sTitre;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur du début de la commande à envoyer au ShellMéga
 * @return sCommande QString contenant "A" pour l'analogique, ou "iR" pour l'I2C
 */
QString Capteur::Get_sCommande()
{
    return this->sCommande;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de l'inconnue à remplacer dans l'expression de résultat par la valeur analogique
 * reçu par le ShellMéga
 * @return sInconnue QString contenant l'inconnue à remplacer dans l'expression de résultat
 */
QString Capteur::Get_sInconnue()
{
    return this->sInconnue;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de l'expression de résultat
 * @return sInconnue QString contenant l'expression permettant d'interpréter la valeur analogique
 */
QString Capteur::Get_sExpression()
{
    return this->sExpression;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de l'unité
 * @return sUnite QString contenant l'unité du résultat
 */
QString Capteur::Get_sUnite()
{
    return this->sUnite;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de la photo de l'objet capteur
 * @return pxmPhoto QPixmap
 */
QPixmap Capteur::Get_pxmPhoto()
{
    return this->pxmPhoto;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur permettant de savoir si l'objet capteur à besoin d'un calibrage
 * @return bCalibrageNecessaire bool qui prend la valeur true si l'objet capteur nécéssite un calibrage
 */
bool Capteur::Get_bCalibrageNecessaire()
{
    return this->bCalibrageNecessaire;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur du pointeur de la fenêtre qui permet le calibrage du capteur
 * @return *oMonCalibrage F_Calibrage
 */
F_Calibrage *Capteur::Get_oMonCalibrage()
{
    return this->oMonCalibrage;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur permettant de savoir si la fenêtre F_Calibrage à déja été créée
 * @return bCapteurCalibre bool Renvoie true si la fenêtre est déjà créée
 */
bool Capteur::Get_bCapteurCalibre()
{
    return this->bCapteurCalibre;
}

//-------------------------------------------------------------------------------

/**
 * Mutateur permettant de modifier la valeur de bCapteurCalibre
 * @return bCapteurCalibre bool la valeur reçu en paramètre est true dès lors que la
 * fenêtre de calibrage de l'objet capteur est créée
 */
void Capteur::Set_bCapteurCalibre(bool bCapteurCalibre)
{
    this->bCapteurCalibre = bCapteurCalibre;
}
//-------------------------------------------------------------------------------
