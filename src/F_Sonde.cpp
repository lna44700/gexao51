#include "F_Sonde.h"
#include "ui_F_Sonde.h"
#include <F_Principale.h>
#include <QDate>
#include <QTime>
#include <QValidator>
#include "Arduino.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QPixmap>
#include <QStandardItem>
#include <QDebug>
#include <QMessageBox>
#include <QString>


using namespace std;

F_Sonde::F_Sonde(Arduino *oMonArduino, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_Sonde)
{
    ui->setupUi(this);
    this->oArduino = oMonArduino;

    //Affiche la date actuelle par défaut
    QDate date = QDate::currentDate();
    ui->DtE_DateAcquisition->setDate(date);

    //Affiche l'heure actuelle par défaut
    QTime heure = QTime::currentTime();
    ui->TiE_HeureAcquisition->setTime(heure);

    //Indications d'écriture dans les champs d'entrée
    ui->LE_TpsAcquisition->setPlaceholderText("en minutes");

    QValidator *ValidatorTpsMesure = new QIntValidator(1, 9999, this);
    ui->LE_TpsAcquisition->setValidator(ValidatorTpsMesure);

    //Met l'icone sur le bouton lancer
    ui->Bt_Lancer->setText("");
    ui->Bt_Lancer->setIcon(QIcon(":/new/prefix1/images/icones/start.ico"));
    ui->Bt_Lancer->setIconSize(QSize(30,30));
    ui->Bt_Lancer->setCheckable(true);
    ui->Bt_Lancer->setEnabled(false);

    //Met l'icone sur le bouton stopper
    ui->Bt_Enregistrement->setText("");
    ui->Bt_Enregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
    ui->Bt_Enregistrement->setIconSize(QSize(30,30));
    ui->Bt_Enregistrement->setCheckable(true);


    //Gere le tableau (nombre de colonnes, taille, intitulé)
    ui->TbW_Valeurs->setColumnCount(1);
    ui->TbW_Valeurs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TbW_Valeurs->setColumnWidth(0,215);
    ui->TbW_Valeurs->setHorizontalHeaderLabels(QStringList()<<"Valeur");
    ui->TbW_Valeurs->verticalHeader()->setFixedWidth(30);


}
//=======================================================================================================================
F_Sonde::~F_Sonde()
{
    delete ui;
}
//=======================================================================================================================
void F_Sonde::on_Bt_ModeAcquisition_clicked()//Action lors du clique sur le bouton de mode d'acquisition
{
    if (ui->Bt_ModeAcquisition->text() == "Automatique")//Si le texte du bouton est 'Automatique'
    {
        //Si choix de l'acquisition en automatique, rend non visibles les objets graphiques et change du texte du bouton
        ui->Bt_ModeAcquisition->setText("Manuel");//Change le texte en Manuel
        ui->Lb_DateHeure->hide();//Cache le label de date et d'heure
        ui->DtE_DateAcquisition->hide();// Cache le champ de sélection de date
        ui->TiE_HeureAcquisition->hide();//Cache le champ de sélection d'heure
        ui->Bt_Lancer->setEnabled(true);//Rend non cliquable le bouton lancé sans sauvegarde des valeurs

    }
    else if (ui->Bt_ModeAcquisition->text() == "Manuel")//Si le texte du bouton est 'Manuel'
    {
        //Si choix de l'acquisition en manuel, affichage des objets graphiques et changement du texte du bouton
        ui->Bt_ModeAcquisition->setText("Automatique");//Change le texte en Automatique
        ui->Lb_DateHeure->show();//Cache le label de date et d'heure
        ui->DtE_DateAcquisition->show();// Cache le champ de sélection de date
        ui->TiE_HeureAcquisition->show();//Cache le champ de sélection d'heure
        ui->Bt_Lancer->setEnabled(false);//Rend cliquable le bouton lancé sans sauvegarde des valeurs
    }
}
//=======================================================================================================================
void F_Sonde::on_Bt_Lancer_clicked()//Action lorsque le bouton Lancer est appuyé
{

    //++++++++++++++++++++++++++++++++++++ICONE++++++++++++++++++++++++++++++++++++
    static bool bEtatLancer(true);

    //Change l'icone Lancer du touton par l'icone Stop
    if (bEtatLancer == true)
    {
        //Met l'icone sur le bouton stopper
        ui->Bt_Lancer->setText("");
        ui->Bt_Lancer->setIcon(QIcon(":/new/prefix1/images/icones/stop.ico"));
        ui->Bt_Lancer->setIconSize(QSize(30,30));
        ui->Bt_Lancer->setCheckable(true);
        bEtatLancer = false;
    }
    else if (bEtatLancer == false)
    {
        //Met l'icone sur le bouton lancer
        ui->Bt_Lancer->setText("");
        ui->Bt_Lancer->setIcon(QIcon(":/new/prefix1/images/icones/start.ico"));
        ui->Bt_Lancer->setIconSize(QSize(30,30));
        ui->Bt_Lancer->setCheckable(true);
        bEtatLancer = true;
    }
    //----------------------------------FIN_ICONE----------------------------------

    //+++++++++++++++++++++++++++++++++VERIFICATION+++++++++++++++++++++++++++++++++

    //Verifie si une valeur a été rentrée dans les champs
    if (ui->SBx_Intervalle->value() == NULL || ui->LE_TpsAcquisition == NULL || this->oArduino->LireCapteur("A10") == -1)
    {
        //Remet en place l'icone sur le bouton lancer
        ui->Bt_Lancer->setText("");
        ui->Bt_Lancer->setIcon(QIcon(":/new/prefix1/images/icones/start.ico"));
        ui->Bt_Lancer->setIconSize(QSize(30,30));
        ui->Bt_Lancer->setCheckable(true);
        bEtatLancer = true;

        //Affiche un message d'erreur
        QMessageBox::about(this, tr("Erreur"),
        tr("Vous avez oublié de rentrer un temps et/ou un intervalle d'acquisition !"));
    }
    //-------------------------------FIN_VERIFICATION-------------------------------
    else
    {
    //+++++++++++++++++++++++++++++++++++AFFICHAGE++++++++++++++++++++++++++++++++++

        //Declaration des variables pour tableau

        int nIntervalle(0);
        int nDureeMesure(0);
        int nDureeTotale(0);

        //Calcul des temps d'acquisition et nombre de mesures
        nIntervalle = ui->SBx_Intervalle->value();
        nDureeMesure = ui->LE_TpsAcquisition->text().toInt();
        nDureeMesure = nDureeMesure*60;
        nDureeTotale = nDureeMesure/nIntervalle;
        //MinuteurIntervalleMesures->setInterval(1000)

        //connect(MinuteurIntervalleMesures, SIGNAL(timeout()), this, SLOT(Affichage()));
        //this->MinuteurIntervalleMesures->start();

    //-------------------------------FIN_AFFICHAGE-------------------------------


    }
}
//=======================================================================================================================

void F_Sonde::Affichage()
{
    int Affichage(0);
    Affichage = this->oArduino->LireCapteur("A10");
    qDebug()<<Affichage;
    ui->Lcd_Valeur->display(Affichage);
}

void F_Sonde::on_Bt_Enregistrement_clicked()//Action lorsque le bouton Stopper est appuyé
{
    //++++++++++++++++++++++++++++++++++++ICONE++++++++++++++++++++++++++++++++++++
    static bool bEtatRecup(true);

    //Change l'icone du bouton Record par l'icone Stop
    if (bEtatRecup == true)
    {
        //Met l'icone sur le bouton stopper
        ui->Bt_Enregistrement->setText("");
        ui->Bt_Enregistrement->setIcon(QIcon(":/new/prefix1/images/icones/stop.ico"));
        ui->Bt_Enregistrement->setIconSize(QSize(30,30));
        ui->Bt_Enregistrement->setCheckable(true);
        bEtatRecup = false;
    }
    else if (bEtatRecup == false)
    {
        //Met l'icone sur le bouton lancer
        ui->Bt_Enregistrement->setText("");
        ui->Bt_Enregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
        ui->Bt_Enregistrement->setIconSize(QSize(30,30));
        ui->Bt_Enregistrement->setCheckable(true);
        bEtatRecup = true;
    }
    //----------------------------------FIN_ICONE----------------------------------

    //+++++++++++++++++++++++++++++++++VERIFICATION+++++++++++++++++++++++++++++++++

    if (ui->SBx_Intervalle->value() == NULL || ui->LE_TpsAcquisition == NULL)
    {
        //Met l'icone sur le bouton lancer
        ui->Bt_Enregistrement->setText("");
        ui->Bt_Enregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
        ui->Bt_Enregistrement->setIconSize(QSize(30,30));
        ui->Bt_Enregistrement->setCheckable(true);
        bEtatRecup = true;

        QMessageBox::about(this, tr("Erreur"),
        tr("Vous avez oublié de rentrer un temps et/ou un intervalle d'acquisition !"));
    }
    //-------------------------------FIN_VERIFICATION-------------------------------
    else
    {
    //+++++++++++++++++++++++++++++++AFFICHAGE_TABLEAU++++++++++++++++++++++++++++++

        //Declaration des variables pour tableau
        int Affichage(0);
        int nIntervalle(0);
        int nDureeMesure(0);
        int nDureeTotale(0);

        //Calcul des temps d'acquisition et nombre de mesures
        nIntervalle = ui->SBx_Intervalle->value();
        nDureeMesure = ui->LE_TpsAcquisition->text().toInt();
        nDureeMesure = nDureeMesure*60;
        nDureeTotale = nDureeMesure/nIntervalle;

        unsigned int j(0);


        //Nom du fichier à partir de la date et l'heure
        QTime heure = QTime::currentTime();
        QDate date = QDate::currentDate();
        QString FormatDate = "dd-MM-yyyy";
        QString FormatHeureSauvegarde = "HH-mm-ss";
        QString FormatHeureAffichage = "HH:mm:ss";
        QString Heure;
        QString Date;
        Heure = heure.toString(FormatHeureSauvegarde);
        Date = date.toString(FormatDate);
        QString NomTempo;
        NomTempo = Date + "_" + Heure + ".csv";
        const char* NomFichier = NomTempo.toStdString().c_str();
        qDebug()<<NomFichier;



        QMessageBox::StandardButton Reponse = QMessageBox::question(this, "Enregistrement",
                              "Voulez-vous enregistrer les valeurs ?", QMessageBox::Yes | QMessageBox::No);

        if (Reponse == QMessageBox::Yes)
        {
            //Création et remplissage du fichier
            ofstream file (NomFichier, ios::out);
            file << "#" << ";" << "Valeur" << ";" << "Date" << ";" << "Heure" << endl;

            for(unsigned int i=nDureeTotale;i>0;i--)//Boucle affichage données sur LCD, tableau et dans fichier
            {

                QTime HeureActuelle = QTime::currentTime();
                QDate DateActuelle = QDate::currentDate();
                Heure = HeureActuelle.toString(FormatHeureAffichage);
                Date = DateActuelle.toString(FormatDate);

                //Récupération de la données et affichage sur lecteur LCD
                Affichage = 25;//this->oArduino->LireCapteur("A10");
                qDebug()<<Affichage;
                ui->Lcd_Valeur->display(Affichage);
                ui->TbW_Valeurs->setRowCount(nDureeTotale);

                //Affichage de la valeur dans le tableau
                QString Var;
                QTableWidgetItem *item = new QTableWidgetItem(Var.setNum(Affichage));
                ui->TbW_Valeurs->setItem(j,0,item);
                file << j+1 << ";" << Affichage << ";" << Date.toStdString() << ";" << Heure.toStdString() << endl;
                j++;
             }

            //Met l'icone sur le bouton lancer
            ui->Bt_Enregistrement->setText("");
            ui->Bt_Enregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
            ui->Bt_Enregistrement->setIconSize(QSize(30,30));
            ui->Bt_Enregistrement->setCheckable(true);
            bEtatRecup = true;
        }
        else if (Reponse == QMessageBox::No)
        {
            for(unsigned int i=nDureeTotale;i>0;i--)//Boucle affichage données sur LCD, tableau et dans fichier
            {
                //Récupération de la données et affichage sur lecteur LCD
                Affichage = this->oArduino->LireCapteur("A10");
                qDebug()<<Affichage;
                ui->Lcd_Valeur->display(Affichage);
                ui->TbW_Valeurs->setRowCount(nDureeTotale);

                //Affichage de la valeur dans le tableau
                QString Var;
                QTableWidgetItem *item = new QTableWidgetItem(Var.setNum(Affichage));
                ui->TbW_Valeurs->setItem(j,0,item);
                j++;
             }

            //Met l'icone sur le bouton lancer
            ui->Bt_Enregistrement->setText("");
            ui->Bt_Enregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
            ui->Bt_Enregistrement->setIconSize(QSize(30,30));
            ui->Bt_Enregistrement->setCheckable(true);
            bEtatRecup = true;
        }

        /*//Création et remplissage du fichier
        ofstream file (NomFichier, ios::out);
        file << "#" << ";" << "Valeur" << ";" << "Date" << ";" << "Heure" << endl;

        for(unsigned int i=nDureeTotale;i>0;i--)//Boucle affichage données sur LCD, tableau et dans fichier
        {

            QTime HeureActuelle = QTime::currentTime();
            QDate DateActuelle = QDate::currentDate();
            Heure = HeureActuelle.toString(FormatHeureAffichage);
            Date = DateActuelle.toString(FormatDate);

            //Récupération de la données et affichage sur lecteur LCD
            Affichage = this->oArduino->LireCapteur("A10");
            qDebug()<<Affichage;
            ui->Lcd_Valeur->display(Affichage);
            ui->TbW_Valeurs->setRowCount(nDureeTotale);

            //Affichage de la valeur dans le tableau
            QString Var;
            QTableWidgetItem *item = new QTableWidgetItem(Var.setNum(Affichage));
            ui->TbW_Valeurs->setItem(j,0,item);
            file << j+1 << ";" << Affichage << ";" << Date.toStdString() << ";" << Heure.toStdString() << endl;
            j++;
         }*/
    //----------------------------FIN_AFFICHAGE_TABLEAU-----------------------------
    }
}
//=======================================================================================================================
