//-------------------------------------------------------------------------------
/**
 * @file        F_ChoisirSonde.cpp
 * @brief       Configuration de la maquette
 *
 * @author      S.GUICHARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        20/04/16
 *
 * Lors de l'appel au constructeur de la classe, un balayage de toute les adresses I2C possibles sera éffectuer,
 * afin de créér et paramètrer les objets capteur corresondant à leurs adresses.
 * Cette classe va créér une IHM pour placer les différents capteurs analogique sur la maquette.
 * Ainsi les objets capteur analogique seront créé
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "F_ChoisirSonde.h"
#include "ui_F_ChoisirSonde.h"
#include "F_Principale.h"

//=====   En-Têtes standards    =====
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <String>
#include <unistd.h>
#include <QDir>
#include <QLabel>

//-------------------------------------------------------------------------------

/**
 * Constructeur paramétré
 * @param *oMonArduino Pointeur de l'objet Arduino créé dans le fichier d'application F_Principale
 */
F_ChoisirSonde::F_ChoisirSonde(Arduino *oMonArduino, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::F_ChoisirSonde),
    oMaListeCapteurVue(new ListeCapteurVue)
{
    this->ui->setupUi(this);

    //Ajout de l'icone de l'application
    QIcon icone(":/new/prefix1/images/icones/icone_g_key.ico");
    QWidget::setWindowIcon(icone);

    this->oMonArduino = new Arduino;
    this->oMonArduino = oMonArduino;

    this->slstListeInterface << "Arduino Mega 2560" << "Easycon 1" << "Easycon 2";

    this->ui->GrpBx_Capteur->layout()->addWidget(this->oMaListeCapteurVue);


    this->ui->CBx_Interface->addItems(this->slstListeInterface);

    for(register int i = 0; i < this->slstListeInterface.length(); i++)
    {
        //Scene
        this->lstScenes.append(new ScenePlanDeCablage(this->oMonArduino));
        this->lstScenes.last()->Set_Interface(this->slstListeInterface[i]);

        //Model Liste Capteur
        this->lstModeles.append(new QStandardItemModel);

        QDir dirReperoire("./IOCard");

        QFileInfoList filstListeRepertoire (dirReperoire.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

        for(register int j = 0; j < filstListeRepertoire.length(); j++)
        {
            if(filstListeRepertoire[j].isDir())
            {
                QDir dirRepertoireIOCard("./IOCard/" + filstListeRepertoire[j].completeBaseName());

                QFileInfoList filstListeFichier (dirRepertoireIOCard.entryInfoList());

                bool bSortieBoucle(false);

                for(register int k = 0; k < filstListeFichier.length() && !bSortieBoucle; k++)
                {
                    if(filstListeFichier[k].isFile() && filstListeFichier[k].suffix() == "ini")
                    {
                        QSettings stgConfiguration(filstListeFichier[k].filePath(), QSettings::IniFormat);
                        QString sInterface(stgConfiguration.value("PORT/Type").toString());
                        if(sInterface == "AI")
                        {
                            bSortieBoucle = true;

                            QStandardItem* oMonNouvelItem(new QStandardItem(stgConfiguration.value("TITRE/NOM").toString()));

                            oMonNouvelItem->setData(filstListeRepertoire[j].completeBaseName(), Qt::UserRole);

                            oMonNouvelItem->setIcon(QIcon(dirRepertoireIOCard.absoluteFilePath("photo.png")));

                            this->lstModeles.last()->appendRow(oMonNouvelItem);
                        }

                    }
                }
            }
        }
    }

    this->oMaListeCapteurVue->setModel(this->lstModeles.at(this->ui->CBx_Interface->currentIndex()));
    this->ui->Vue->setScene(this->lstScenes.at(this->ui->CBx_Interface->currentIndex()));

    this->pTimer = new QTimer;
    this->pTimer2 = new QTimer;

    this->pTimer->setInterval(1000);
    this->pTimer2->setInterval(1000);

    connect(pTimer,SIGNAL(timeout()),this,SLOT(on_MiseAJourQList()));
    connect(pTimer2,SIGNAL(timeout()),this,SLOT(on_AfficherCalibrage()));

    this->setWindowModality(Qt::ApplicationModal);

}

//-------------------------------------------------------------------------------

/**
 * Destructeur
 */
F_ChoisirSonde::~F_ChoisirSonde()
{
    delete this->ui;
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode est en charge de scanner les adresses I2C et de créer les objets capteurs
 * à laquelle elle est connecté. De plus lorsque l'Arduino est déconnecté puis re-connecté,
 * un nouveau scan est effectué pou mettre à jour la liste des objets capteur I2C
 */
void F_ChoisirSonde::ScannerAdresseI2C()
{
    QString sCommande ("");
    QList<QString> lstAdresse;
    QList<QString> lstNomCapteur;
    QSettings stgAdresses("Adresses.ini", QSettings::IniFormat);


    //Détéction du nombre de capteur I2C
    for(unsigned int i=0; i <=143; i++)
    {
        sCommande = "iR00" + QString::number(i) + "02";

        if(i > 9)
        {
            sCommande.remove(2,1);
        }
        if(i > 99)
        {
            sCommande.remove(2,1);
        }


        if(this->oMonArduino->LireCapteur(sCommande) != -1)
        {
            if(i == 0 && i <= 15)
            {
                lstNomCapteur.push_back( stgAdresses.value("0-15/NOM").toString());
            }
            if(i >= 16 && i <= 31)
            {
                lstNomCapteur.push_back( stgAdresses.value("16-31/NOM").toString());
            }
            if(i >= 32 && i <= 47)
            {
                lstNomCapteur.push_back( stgAdresses.value("32-47/NOM").toString());
            }
            if(i >= 48 && i <= 63)
            {
                lstNomCapteur.push_back( stgAdresses.value("48-63/NOM").toString());
            }
            if(i >= 64 && i <= 79)
            {
                lstNomCapteur.push_back( stgAdresses.value("64-79/NOM").toString());
            }
            if(i >= 80 && i <= 95)
            {
                lstNomCapteur.push_back( stgAdresses.value("80-95/NOM").toString());
            }
            if(i >= 96 && i <= 111)
            {
                lstNomCapteur.push_back( stgAdresses.value("96-111/NOM").toString());
            }
            if(i >= 112 && i <= 127)
            {
                lstNomCapteur.push_back( stgAdresses.value("112-127/NOM").toString());
            }
            if(i >=128 && i <= 143)
            {
                lstNomCapteur.push_back( stgAdresses.value("128-143/NOM").toString());
            }


            lstAdresse.push_back(QString::number(i));

        }
    }


    //On supprime de la liste les capteurs qui ne sont plus présents
    for(unsigned int i=0; i<lstListeCapteurI2C.length();i++)
    {
        bool bCapteurEstToujoursPresent(false);

        for(unsigned int j=0; j<lstAdresse.length();j++)
        {
            if(this->lstListeCapteurI2C[i]->Get_sNomBroche() == lstAdresse[j])
            {
                bCapteurEstToujoursPresent = true;
            }
        }

        if(bCapteurEstToujoursPresent == false)
        {
            this->lstListeCapteurI2C[i]->Get_oMonCalibrage()->close();
            this->lstListeCapteurI2C.removeAt(i);
        }
    }

    //On supprime de la liste les capteurs déjà présent
    for(unsigned int i=0; i<lstAdresse.length();i++)
    {
        bool bCapteurEstDejaPresent(false);

        for(unsigned int j=0; j<lstListeCapteurI2C.length();j++)
        {
            if(lstAdresse[i] == this->lstListeCapteurI2C[j]->Get_sNomBroche())
            {
                bCapteurEstDejaPresent = true;
            }
        }

        if(bCapteurEstDejaPresent == true)
        {
            lstAdresse[i].clear();
            lstNomCapteur[i].clear();
        }
    }

    for(unsigned int i=0; i<lstAdresse.length();i++)
    {
        if(lstAdresse[i] != NULL && lstNomCapteur[i] != NULL)
        {
            QString sNom (lstNomCapteur[i]);
            QString sBroche ((lstAdresse[i]));

            Capteur* oMonCapteur = new Capteur(this->oMonArduino, sNom, sBroche);
            this->lstListeCapteurI2C.push_back(oMonCapteur);
        }

    }

}

//-------------------------------------------------------------------------------

/**
 * Ce slot va permettre d'actualiser les capteurs entre la scène Arduino Méga
 * 2560 et les scènes Easycon 1 et 2.
 * @param nIndex int correspondant à l'index courrant du ComboBox contenat la liste des
 * différentes interfaces
 */
void F_ChoisirSonde::on_ComboBoxInterface(int nIndex)
{    
    if(this->isVisible())
    {
        QList<ItemBroche *> lstListeBroche0;
        lstListeBroche0 = this->lstScenes[0]->Get_lstListeBroche();

        QList<ItemBroche *> lstListeBroche1;
        lstListeBroche1 = this->lstScenes[1]->Get_lstListeBroche();

        QList<ItemBroche *> lstListeBroche2;
        lstListeBroche2 = this->lstScenes[2]->Get_lstListeBroche();

            if(this->wIndexPrecedent == 1 || this->wIndexPrecedent ==2)
            {
                for(unsigned int i=0; i<lstListeBroche0.length(); i++ )
                {
                    Capteur *oCapteurConnecte0 = new Capteur(NULL, "", "");
                    oCapteurConnecte0 = lstListeBroche0[i]->Get_pCapteurConnecte();

                    if(i >= 0 && i <=9)
                    {
                        Capteur *oCapteurConnecte1 = new Capteur(NULL, "", "");
                        oCapteurConnecte1 = lstListeBroche1[i]->Get_pCapteurConnecte();

                        if(oCapteurConnecte0 != oCapteurConnecte1)
                        {
                            lstListeBroche0[i]->Set_pCapteurConnecte(NULL);

                            //TypeAccept tacptAccepteDrag;
                            //tacptAccepteDrag = lstListeBroche0[i]->Get_tacptAccepteDrag();

                            lstListeBroche0[i]->Set_tacptAccepteDrag(NORMAL);
                            lstListeBroche0[i]->update();
                        }
                    }

                    if(i >= 10 && i <=15)
                    {
                        Capteur *oCapteurConnecte2 = new Capteur(NULL, "", "");
                        oCapteurConnecte2 = lstListeBroche2[i-10]->Get_pCapteurConnecte();

                        if(oCapteurConnecte0 != oCapteurConnecte2)
                        {
                            lstListeBroche0[i]->Set_pCapteurConnecte(NULL);

                            lstListeBroche0[i]->Set_tacptAccepteDrag(NORMAL);
                            lstListeBroche0[i]->update();
                        }
                    }
                }
            }
            else
            {

                    for(unsigned int i=0; i<lstListeBroche1.length(); i++ )
                    {
                        Capteur *oCapteurConnecte0 = new Capteur(NULL, "", "");
                        oCapteurConnecte0 = lstListeBroche0[i]->Get_pCapteurConnecte();

                        Capteur *oCapteurConnecte1 = new Capteur(NULL, "", "");
                        oCapteurConnecte1 = lstListeBroche1[i]->Get_pCapteurConnecte();

                        if(oCapteurConnecte1 != oCapteurConnecte0)
                        {
                            lstListeBroche1[i]->Set_pCapteurConnecte(NULL);

                            //TypeAccept tacptAccepteDrag;
                            //tacptAccepteDrag = lstListeBroche1[i]->Get_tacptAccepteDrag();

                            lstListeBroche1[i]->Set_tacptAccepteDrag(NORMAL);
                            lstListeBroche1[i]->update();
                        }
                    }

                    for(unsigned int i=0; i<lstListeBroche2.length(); i++ )
                    {
                        Capteur *oCapteurConnecte0 = new Capteur(NULL, "", "");
                        oCapteurConnecte0 = lstListeBroche0[i]->Get_pCapteurConnecte();

                        Capteur *oCapteurConnecte2 = new Capteur(NULL, "", "");
                        oCapteurConnecte2 = lstListeBroche2[i]->Get_pCapteurConnecte();

                        if(oCapteurConnecte2 != oCapteurConnecte0)
                        {
                            lstListeBroche2[i]->Set_pCapteurConnecte(NULL);

                            //TypeAccept tacptAccepteDrag;
                            //tacptAccepteDrag = lstListeBroche2[i]->Get_tacptAccepteDrag();

                            lstListeBroche2[i]->Set_tacptAccepteDrag(NORMAL);
                            lstListeBroche2[i]->update();
                        }
                    }

            }

            for(unsigned int i=0; i<lstListeBroche0.length(); i++)
            {
                Capteur *oCapteurConnecte0 = new Capteur(NULL, "", "");
                oCapteurConnecte0 = lstListeBroche0[i]->Get_pCapteurConnecte();

                if(oCapteurConnecte0 != NULL)
                {
                    QString sNomBroche0;
                    sNomBroche0 = oCapteurConnecte0->Get_sNomBroche();

                    if(sNomBroche0.toInt() >=0 && sNomBroche0.toInt() <= 9)
                    {

                            //oCapteurConnecte1 = lstListeBroche1[sNomBroche0.toInt()]->Get_pCapteurConnecte();
                            lstListeBroche1[sNomBroche0.toInt()]->Set_pCapteurConnecte(oCapteurConnecte0);
                    }

                    if(sNomBroche0.toInt() >=10 && sNomBroche0.toInt() <= 15)
                    {
                        //oCapteurConnecte2 = lstListeBroche2[sNomBroche0.toInt()-10]->Get_pCapteurConnecte();
                        lstListeBroche2[sNomBroche0.toInt()-10]->Set_pCapteurConnecte(oCapteurConnecte0);
                    }
                }
            }

            for(unsigned int i=0; i< lstListeBroche1.length() ; i++ )
            {

                Capteur *oCapteurConnecte1 = new Capteur(NULL, "", "");
                oCapteurConnecte1 = lstListeBroche1[i]->Get_pCapteurConnecte();

                if(oCapteurConnecte1 != NULL)
                {
                    QString sNomBroche1;
                    sNomBroche1 = oCapteurConnecte1->Get_sNomBroche();

                    lstListeBroche0[sNomBroche1.toInt()]->Set_pCapteurConnecte(oCapteurConnecte1);

                }
            }


            for(unsigned int i=0; i<lstListeBroche2.length(); i++ )
            {
                Capteur *oCapteurConnecte2 = new Capteur(NULL, "", "");
                oCapteurConnecte2 = lstListeBroche2[i]->Get_pCapteurConnecte();

                if(oCapteurConnecte2 != NULL)
                {
                    QString sNomBroche2;
                    sNomBroche2 = oCapteurConnecte2->Get_sNomBroche();

                    lstListeBroche0[sNomBroche2.toInt()]->Set_pCapteurConnecte(oCapteurConnecte2);
                }
            }



    if(nIndex < this->lstModeles.length())
    {
        this->oMaListeCapteurVue->setModel(this->lstModeles[nIndex]);
        this->ui->Vue->setScene(this->lstScenes[nIndex]);
        this->ui->CBx_Interface->setCurrentIndex(nIndex);
    }

    this->wIndexPrecedent = nIndex;
    }
}

//-------------------------------------------------------------------------------

/**
 * Ce slot va permettre de supprimer tous les capteurs présents sur les scènes
 */
void F_ChoisirSonde::on_ActionSupprimer()
{
    for(unsigned int i=0; i<this->lstScenes.length(); i++)
    {
        QList<ItemBroche *> lstListeBroche;
        lstListeBroche = this->lstScenes[i]->Get_lstListeBroche();

        for(unsigned int j=0; j<lstListeBroche.length(); j++)
        {
            lstListeBroche[j]->Set_pCapteurConnecte(NULL);
            lstListeBroche[j]->Set_tacptAccepteDrag(NORMAL);
            lstListeBroche[j]->update();
        }
    }
}

//-------------------------------------------------------------------------------

/**
 * Ce slot va arrêter le timer qui permet de mettre à jour la liste des capteurs placés sur
 * les scèenes, et fermer la fenêtre F_ChoisirSonde.
 */
void F_ChoisirSonde::on_ActionValider()
{
    this->pTimer->stop();
    this->close();
}

//-------------------------------------------------------------------------------

/**
 * Ce slot va mettre à jour la liste des capteurs placés sur les scènes grâce à un connect.
 */
void F_ChoisirSonde::on_MiseAJourQList()
{

    this->lstListeCapteurAnalogique.clear();

    bool bCapteurTrouve (false);

    for(unsigned int i=0; i<this->lstScenes.length(); i++)
    {
        QList<ItemBroche *> lstListeBroche;
        lstListeBroche = this->lstScenes[i]->Get_lstListeBroche();

        for(unsigned int j=0; j < lstListeBroche.length();j++)
        {
            Capteur *oCapteurConnecte = new Capteur(NULL, "", "");
            oCapteurConnecte = lstListeBroche[j]->Get_pCapteurConnecte();

            if(oCapteurConnecte != NULL)
            {
                for(unsigned int k=0; k<this->lstListeCapteurAnalogique.length(); k++)
                {
                    if(this->lstListeCapteurAnalogique[k] == oCapteurConnecte)
                    {
                        bCapteurTrouve = true;
                    }
                }

                if(bCapteurTrouve != true)
                {
                    this->lstListeCapteurAnalogique << oCapteurConnecte;
                }

                bCapteurTrouve = false;
            }
        }
    }
}

//-------------------------------------------------------------------------------

/**
 * Ce slot va permettre d'ouvrir automatiquement grâce à un connect une fenêtre de calibrage
 * dès lors qu'un capteur sera placé sur la scène.
 */
void F_ChoisirSonde::on_AfficherCalibrage()
{
    this->lstListeCapteurComplete=lstListeCapteurAnalogique+lstListeCapteurI2C;

    for(unsigned int i=0; i<this->lstListeCapteurComplete.length(); i++)
    {
        bool bCapteurCalibre;
        bCapteurCalibre = this->lstListeCapteurComplete[i]->Get_bCapteurCalibre();

        bool bCalibrageNecessaire;
        bCalibrageNecessaire = this->lstListeCapteurComplete[i]->Get_bCalibrageNecessaire();

        F_Calibrage *oMonCalibrage = new F_Calibrage(NULL);
        oMonCalibrage = this->lstListeCapteurComplete[i]->Get_oMonCalibrage();

        QString sCommande;
        sCommande = this->lstListeCapteurComplete[i]->Get_sCommande();

        QString sNomBroche;
        sNomBroche = this->lstListeCapteurComplete[i]->Get_sNomBroche();

        if(bCapteurCalibre == false && bCalibrageNecessaire == true)
        {
            oMonCalibrage->Set_sCommande(sCommande);
            oMonCalibrage->Set_sNomBroche(sNomBroche);
            oMonCalibrage->setWindowTitle(this->lstListeCapteurComplete[i]->Get_sTitre() + " - Adresse : " + sNomBroche);

            if(this->oMonArduino->bEstOuvert == false)
            {
                oMonCalibrage->setEnabled(false);
            }

            oMonCalibrage->show();

            this->lstListeCapteurComplete[i]->Set_bCapteurCalibre(true);
        }

        if(bCapteurCalibre == true && bCalibrageNecessaire == true)
        {
            if(this->oMonArduino->bEstOuvert == true && oMonCalibrage->isEnabled() == false)
            {
                oMonCalibrage->setEnabled(true);
            }
        }

    }

}

//-------------------------------------------------------------------------------

/**
 * Accesseur du Timer connecté sur le slot de mise à jour de la liste des capteurs
 * @return pTimer QTimer* qui permet de pouvoir démarrer le timer à partir de F_Principale
 */
QTimer* F_ChoisirSonde::Get_pTimer()
{
    return this->pTimer;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur du Timer connecté sur le slot permettant de démarrer une fenêtre de calibrage s'y
 * celà est nécessaire.
 * @return pTimer2 QTimer* qui permet de pouvoir démarrer le timer à partir de F_Principale
 */
QTimer *F_ChoisirSonde::Get_pTimer2()
{
    return this->pTimer2;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de la liste des objets capteurs placés sur la scène permettant de
 * sauvegarder les informations nécéssaires.
 * @return lstListeCapteurAnalogique QList<Capteur*>
 */
QList<Capteur*> F_ChoisirSonde::Get_lstListeCapteurAnalogique()
{
    return this->lstListeCapteurAnalogique;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de la liste des objets capteurs complète permettant d'utiliser leurs attributs
 * pour l'acquisition
 * @return lstListeCapteurComplete QList<Capteur*>
 */
QList<Capteur*> F_ChoisirSonde::Get_lstListeCapteurComplete()
{
    return this->lstListeCapteurComplete;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de la liste des objets scenes
 * @return lstScenes QList<ScenePlanDeCablage *>
 */
QList<ScenePlanDeCablage *> F_ChoisirSonde::Get_lstScenes()
{
    return this->lstScenes;
}
//-------------------------------------------------------------------------------
