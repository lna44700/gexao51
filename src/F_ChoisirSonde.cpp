#include "F_ChoisirSonde.h"
#include "ui_F_ChoisirSonde.h"
#include "F_Principale.h"
#include <QSettings>
#include <QDebug>
#include <String>
#include <unistd.h>
#include <QDir>
#include <QLabel>

#include <QMouseEvent>
#include <QStandardItemModel>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

F_ChoisirSonde::F_ChoisirSonde(Arduino *oMonArduino, QSettings* Config, QStringList ListeInterfaceInstall, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::F_ChoisirSonde),
    ListeCap(new ListeCapteurVue),
    Config  (Config)
{
    this->Config->sync();

    ui->setupUi(this);


        QString Commande ("");
        unsigned int NombreCapteur (0);
        QList<int> Adresse;

        //Détéction du nombre de capteur I2C
        for(unsigned int i=0; i <=143; i++)
        {
            Commande = "iR00";
            Commande += QString::number(i);
            Commande += "02";

            if(i > 9)
            {
                Commande.remove(2,1);
            }
            if(i > 99)
            {
                Commande.remove(2,1);
            }

            //qDebug() << Commande;
            //qDebug() << oMonArduino->LireCapteur(Commande);

            if(oMonArduino->LireCapteur(Commande) != -1)
            {
                Adresse.push_back(i);
                //qDebug() << Commande;
                //qDebug() << oMonArduino->LireCapteur(Commande);

            }
        }

    //qDebug() << Adresse.length();
    QList<int>::iterator p = Adresse.begin();
/*
    while (p !=Adresse.end())
    {
        qDebug() << *p;
        p++;
    }*/

        QList<QString> NomCapteur;
        QSettings Adresses("Adresses.ini", QSettings::IniFormat);
        QList<QString>::iterator p2;

        p = Adresse.begin();

        while(p !=Adresse.end())
        {
            if(*p >= 0 && *p <= 15)
            {
                NomCapteur.push_back( Adresses.value("0-15/NOM").toString());
            }
            if(*p >= 16 && *p <= 31)
            {
                NomCapteur.push_back( Adresses.value("16-31/NOM").toString());
            }
            if(*p >= 32 && *p <= 47)
            {
                NomCapteur.push_back( Adresses.value("32-47/NOM").toString());
            }
            if(*p >= 48 && *p <= 63)
            {
                NomCapteur.push_back( Adresses.value("48-63/NOM").toString());
            }
            if(*p >= 64 && *p <= 79)
            {
                NomCapteur.push_back( Adresses.value("64-79/NOM").toString());
            }
            if(*p >= 80 && *p <= 95)
            {
                NomCapteur.push_back( Adresses.value("80-95/NOM").toString());
            }
            if(*p >= 96 && *p <= 111)
            {
                NomCapteur.push_back( Adresses.value("96-111/NOM").toString());
            }
            if(*p >= 112 && *p <= 127)
            {
                NomCapteur.push_back( Adresses.value("112-127/NOM").toString());
            }
            if(*p >=128 && *p <= 143)
            {
                NomCapteur.push_back( Adresses.value("128-143/NOM").toString());
            }

            p++;
        }
        p2 = NomCapteur.begin();
        while(p2 !=NomCapteur.end())
        {
            qDebug() << *p2;
            p2++;
        }

        for(unsigned int k=0; k<Adresse.length();k++)
        {
            QString Nom (NomCapteur[k]);
            QString Broche (Adresse[k]);
            Capteur* NouveauCA = new Capteur(Nom,Broche);
            ListeI2C.push_back(NouveauCA);
        }


     ListeInterfaceInstall << "Easycon1" << "Easycon2";

        this->ui->GrpBx->layout()->addWidget(this->ListeCap);


        this->ui->CBx_Interface->addItems(ListeInterfaceInstall);

        for(register int i = 0; i < ListeInterfaceInstall.length(); i++)
        {
            //Scene
            this->Scenes.append(new ScenePlanDeCablage(Config));
            this->Scenes.last()->setInterface(ListeInterfaceInstall[i]);

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

        this->ListeCap->setModel(this->Models.at(this->ui->CBx_Interface->currentIndex()));
        this->ui->Vue->setScene(this->Scenes.at(this->ui->CBx_Interface->currentIndex()));

}


F_ChoisirSonde::~F_ChoisirSonde()
{
    delete ui;
}

void F_ChoisirSonde::on_CBx_Interface_currentIndexChanged(int index)
{
    if(index < this->Models.length())
    {
        this->ListeCap->setModel(this->Models[index]);
        this->ui->Vue->setScene(this->Scenes[index]);
    }
}

void F_ChoisirSonde::on_Bt_Annuler_clicked()
{
    this->close();
}

void F_ChoisirSonde::on_Bt_Valider_clicked()
{
    for(unsigned int i=0; i<this->Scenes.length(); i++)
    {
        for(unsigned int j=0; j<this->Scenes[i]->ListeBroche.length();j++)
        {
            this->ListeCapteur << (this->Scenes[i]->ListeBroche[j]->NouveauCA);
        }
    }

    this->close();
}
