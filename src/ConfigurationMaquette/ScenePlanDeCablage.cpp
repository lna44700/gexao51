//-------------------------------------------------------------------------------
/**
 * @file        ScenePlanDeCablage.cpp
 * @brief       Réalisation de la scène permettant de réaliser le câblage
 *
 * @author      S.GUICHARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        27/04/16
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "ScenePlanDeCablage.h"

//=====   En-Têtes standards    =====
#include <QSettings>
#include <QDebug>

//-------------------------------------------------------------------------------

/**
 * Constructeur paramétré
 * @param *oMonArduino Pointeur de l'objet Arduino créé dans le fichier d'application F_Principale
 */
ScenePlanDeCablage::ScenePlanDeCablage(Arduino *oMonArduino, QObject *parent) :
    QGraphicsScene  (parent),
    oMaPhoto       (NULL)
{
    this->oMonArduino = new Arduino;
    this->oMonArduino = oMonArduino;

}

//-------------------------------------------------------------------------------
/**
 * Destructeur
 */
ScenePlanDeCablage::~ScenePlanDeCablage()
{
    if(this->oMaPhoto != NULL)
    {
        this->removeItem(this->oMaPhoto);
        delete this->oMaPhoto;
        this->oMaPhoto = NULL;
    }

    for(register int i = 0; i < this->lstListeBroche.length(); i++)
    {
        if(this->lstListeBroche[i] != NULL)
        {
            this->removeItem(this->lstListeBroche[i]);
            delete this->lstListeBroche[i];
            this->lstListeBroche[i] = NULL;
        }
    }
}

//-------------------------------------------------------------------------------

/**
 * Mutateur qui va permettre de définir une interface
 * @param sNomInterface QString contenant le nom de l'interface, ce qui va permettre de
 * récupérer son fichier de configuration et sa photo. De plus les objet ItemBroche
 * seront créées dans cette méthode
 */
void ScenePlanDeCablage::Set_Interface(QString sNomInterface)
{
    this->sNomInterface = sNomInterface;

    //Photo Interface
    if(oMaPhoto != NULL)
    {
        this->removeItem(this->oMaPhoto);
        delete this->oMaPhoto;
        this->oMaPhoto = NULL;
    }

    this->oMaPhoto = new Photo("./Interfaces/" + sNomInterface);
    this->addItem(this->oMaPhoto);

    //Liste Broche
    for(register int i = 0; i < lstListeBroche.length(); i++)
    {
        if(lstListeBroche[i] != 0)
        {
            this->removeItem(lstListeBroche[i]);
            delete lstListeBroche[i];
            lstListeBroche[i] = 0;
        }
    }

    //Ouverture de la configuration de l'interface
    QSettings stgConfiguration("./Interfaces/" + sNomInterface + "/config.ini", QSettings::IniFormat);

    QStringList slstListeBroche(stgConfiguration.childGroups());

    for(register int i = 0; i < slstListeBroche.length(); i++)
    {
        if(slstListeBroche[i].left(6) == "BROCHE")
        {
            stgConfiguration.beginGroup(slstListeBroche[i]);

            if(stgConfiguration.value("Type").toStringList().contains("AI"))
            {
                ItemBroche *oMonNouvelItem = new ItemBroche(this->oMonArduino);
                oMonNouvelItem->Set_sNomBroche(stgConfiguration.value("Nom").toString());
                oMonNouvelItem->setX(stgConfiguration.value("X").toInt());
                oMonNouvelItem->setY(stgConfiguration.value("Y").toInt());

                this->addItem(oMonNouvelItem);

                this->lstListeBroche.append(oMonNouvelItem);
            }
            stgConfiguration.endGroup();

        }
    }
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de la liste des broches présentes sur une scène
 * @return lstListeBroche QList<ItemBroche *>
 */
QList<ItemBroche *> ScenePlanDeCablage::Get_lstListeBroche()
{
    return this->lstListeBroche;
}
//-------------------------------------------------------------------------------
