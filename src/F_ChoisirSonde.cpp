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

F_ChoisirSonde::F_ChoisirSonde(Arduino *oMonArduino, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::F_ChoisirSonde),
    ListeCap(new ListeCapteurVue)
{
    this->ui->setupUi(this);


        QString Commande ("");
        QList<QString> Adresse;
        QList<QString> NomCapteur;
        QSettings Adresses("Adresses.ini", QSettings::IniFormat);

        //Détéction du nombre de capteur I2C
        for(unsigned int i=0; i <=143; i++)
        {
            Commande = "iR00" + QString::number(i) + "02";


            if(i > 9)
            {
                Commande.remove(2,1);
            }
            if(i > 99)
            {
                Commande.remove(2,1);
            }

            if(oMonArduino->LireCapteur(Commande) != -1)
            {

                if(i == 0 && i <= 15)
                {
                    NomCapteur.push_back( Adresses.value("0-15/NOM").toString());
                }
                if(i >= 16 && i <= 31)
                {
                    NomCapteur.push_back( Adresses.value("16-31/NOM").toString());
                }
                if(i >= 32 && i <= 47)
                {
                    NomCapteur.push_back( Adresses.value("32-47/NOM").toString());
                }
                if(i >= 48 && i <= 63)
                {
                    NomCapteur.push_back( Adresses.value("48-63/NOM").toString());
                }
                if(i >= 64 && i <= 79)
                {
                    NomCapteur.push_back( Adresses.value("64-79/NOM").toString());
                }
                if(i >= 80 && i <= 95)
                {
                    NomCapteur.push_back( Adresses.value("80-95/NOM").toString());
                }
                if(i >= 96 && i <= 111)
                {
                    NomCapteur.push_back( Adresses.value("96-111/NOM").toString());
                }
                if(i >= 112 && i <= 127)
                {
                    NomCapteur.push_back( Adresses.value("112-127/NOM").toString());
                }
                if(i >=128 && i <= 143)
                {
                    NomCapteur.push_back( Adresses.value("128-143/NOM").toString());
                }


                Adresse.push_back(QString::number(i));

            }
        }


        for(unsigned int k=0; k<Adresse.length();k++)
        {
            QString Nom (NomCapteur[k]);
            QString Broche ((Adresse[k]));

            Capteur* NouveauCA = new Capteur(Nom, Broche);
            this->ListeCapteurI2C.push_back(NouveauCA);
        }


        this->ListeInterfaceInstall << "Arduino Mega 2560" << "Easycon 1" << "Easycon 2";

        this->ui->GrpBx->layout()->addWidget(this->ListeCap);


        this->ui->CB_Interface->addItems(this->ListeInterfaceInstall);

        for(register int i = 0; i < this->ListeInterfaceInstall.length(); i++)
        {
            //Scene
            this->Scenes.append(new ScenePlanDeCablage(/*Config*/));
            this->Scenes.last()->setInterface(this->ListeInterfaceInstall[i]);

            //Model Liste Capteur
            this->Models.append(new QStandardItemModel);

            QDir DirControl("./IOCard");

            QFileInfoList ListeDir (DirControl.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

            for(register int j = 0; j < ListeDir.length(); j++)
            {
                if(ListeDir[j].isDir())
                {
                    QDir RepertoireCourant("./IOCard/" + ListeDir[j].completeBaseName());

                    QFileInfoList ListeFichier (RepertoireCourant.entryInfoList());

                    bool SortieBoucle(false);

                    for(register int k = 0; k < ListeFichier.length() && !SortieBoucle; k++)
                    {
                        if(ListeFichier[k].isFile())
                        {
                            if(ListeFichier[k].suffix() == "ini")
                            {
                                QSettings ConfigCarte(ListeFichier[k].filePath(), QSettings::IniFormat);
                                QString Interface(ConfigCarte.value("PORT/Type").toString());
                                if(Interface == "AI")
                                {
                                    SortieBoucle = true;

                                    QStandardItem* NouvelItem(new QStandardItem(ConfigCarte.value("TITRE/NOM").toString()));

                                    NouvelItem->setData(ListeDir[j].completeBaseName(), Qt::UserRole);

                                    NouvelItem->setIcon(QIcon(RepertoireCourant.absoluteFilePath("photo.png")));

                                    this->Models.last()->appendRow(NouvelItem);
                                }
                            }
                        }
                    }
                }
            }
        }

        this->ListeCap->setModel(this->Models.at(this->ui->CB_Interface->currentIndex()));
        this->ui->Vue->setScene(this->Scenes.at(this->ui->CB_Interface->currentIndex()));

}


F_ChoisirSonde::~F_ChoisirSonde()
{
    delete this->ui;
}

void F_ChoisirSonde::on_CB_Interface_currentIndexChanged(int index)
{
    if(this->isVisible())
    {
            if(this->indexPrecedent == 1 || this->indexPrecedent ==2)
            {
                for(unsigned int i=0; i<this->Scenes[0]->ListeBroche.length(); i++ )
                {
                    if(i >= 0 && i <=9)
                    {
                        if(this->Scenes[0]->ListeBroche[i]->CapConnect != this->Scenes[1]->ListeBroche[i]->CapConnect)
                        {
                            this->Scenes[0]->ListeBroche[i]->CapConnect = NULL;
                            this->Scenes[0]->ListeBroche[i]->AcceptDrag = NORMAL;
                            this->Scenes[0]->ListeBroche[i]->update();
                        }
                    }

                    if(i >= 10 && i <=15)
                    {
                        if(this->Scenes[0]->ListeBroche[i]->CapConnect != this->Scenes[2]->ListeBroche[i-10]->CapConnect)
                        {
                            this->Scenes[0]->ListeBroche[i]->CapConnect = NULL;
                            this->Scenes[0]->ListeBroche[i]->AcceptDrag = NORMAL;
                            this->Scenes[0]->ListeBroche[i]->update();
                        }
                    }
                }
            }
            else
            {



                    for(unsigned int i=0; i<this->Scenes[1]->ListeBroche.length(); i++ )
                    {
                        if(this->Scenes[1]->ListeBroche[i]->CapConnect != this->Scenes[0]->ListeBroche[i]->CapConnect)
                        {
                            this->Scenes[1]->ListeBroche[i]->CapConnect = NULL;
                            this->Scenes[1]->ListeBroche[i]->AcceptDrag = NORMAL;
                            this->Scenes[1]->ListeBroche[i]->update();
                        }
                    }

                    for(unsigned int i=0; i<this->Scenes[2]->ListeBroche.length(); i++ )
                    {
                        if(this->Scenes[2]->ListeBroche[i]->CapConnect != this->Scenes[0]->ListeBroche[i]->CapConnect)
                        {
                            this->Scenes[2]->ListeBroche[i]->CapConnect = NULL;
                            this->Scenes[2]->ListeBroche[i]->AcceptDrag = NORMAL;
                            this->Scenes[2]->ListeBroche[i]->update();
                        }
                    }



            }

            for(unsigned int j=0; j<this->Scenes[0]->ListeBroche.length(); j++)
            {
                if(this->Scenes[0]->ListeBroche[j]->CapConnect != NULL)
                {
                    if(this->Scenes[0]->ListeBroche[j]->CapConnect->NomBroche.toInt() >=0 && this->Scenes[0]->ListeBroche[j]->CapConnect->NomBroche.toInt() <= 9)
                    {
                            this->Scenes[1]->ListeBroche[this->Scenes[0]->ListeBroche[j]->CapConnect->NomBroche.toInt()]->CapConnect = this->Scenes[0]->ListeBroche[j]->CapConnect;
                    }

                    if(this->Scenes[0]->ListeBroche[j]->CapConnect->NomBroche.toInt() >=10 && this->Scenes[0]->ListeBroche[j]->CapConnect->NomBroche.toInt() <= 15)
                    {
                            this->Scenes[2]->ListeBroche[this->Scenes[0]->ListeBroche[j]->CapConnect->NomBroche.toInt()-10]->CapConnect = this->Scenes[0]->ListeBroche[j]->CapConnect;
                    }
                }
            }
            for(unsigned int j=0; j<this->Scenes[1]->ListeBroche.length(); j++ )
            {
                if(this->Scenes[1]->ListeBroche[j]->CapConnect != NULL)
                {
                        this->Scenes[0]->ListeBroche[this->Scenes[1]->ListeBroche[j]->CapConnect->NomBroche.toInt()]->CapConnect = this->Scenes[1]->ListeBroche[j]->CapConnect;
                }
            }

            for(unsigned int k=0; k<this->Scenes[2]->ListeBroche.length(); k++ )
            {
                if(this->Scenes[2]->ListeBroche[k]->CapConnect != NULL)
                {
                        this->Scenes[0]->ListeBroche[this->Scenes[2]->ListeBroche[k]->CapConnect->NomBroche.toInt()]->CapConnect = this->Scenes[2]->ListeBroche[k]->CapConnect;
                }
            }


    }

    if(index < this->Models.length())
    {
        this->ListeCap->setModel(this->Models[index]);
        this->ui->Vue->setScene(this->Scenes[index]);
        this->ui->CB_Interface->setCurrentIndex(index);
    }

    this->indexPrecedent = index;
}

void F_ChoisirSonde::on_Btn_Supprimer_clicked()
{
    for(unsigned int i=0; i<this->Scenes.length(); i++)
    {
        for(unsigned int j=0; j<this->Scenes[i]->ListeBroche.length(); j++)
        {
            this->Scenes[i]->ListeBroche[j]->CapConnect = NULL;
            this->Scenes[i]->ListeBroche[j]->AcceptDrag = NORMAL;
            this->Scenes[i]->ListeBroche[j]->update();
        }
    }
}

void F_ChoisirSonde::on_Btn_Valider_clicked()
{

    this->ListeCapteurAnalogique.clear();

    bool CapteurTrouve (false);

    for(unsigned int i=0; i<this->Scenes.length(); i++)
    {
        for(unsigned int j=0; j<this->Scenes[i]->ListeBroche.length();j++)
        {
            if(this->Scenes[i]->ListeBroche[j]->CapConnect != NULL)
            {
                for(unsigned int k=0; k<this->ListeCapteurAnalogique.length(); k++)
                {
                    if(this->ListeCapteurAnalogique[k] == this->Scenes[i]->ListeBroche[j]->CapConnect)
                    {
                        CapteurTrouve = true;
                    }
                }

                if(CapteurTrouve != true)
                {
                    this->ListeCapteurAnalogique << this->Scenes[i]->ListeBroche[j]->CapConnect;
                }

                CapteurTrouve = false;
            }
        }
    }

    this->close();
}

