#include "F_Sonde.h"
#include "ui_F_Sonde.h"
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


using namespace std;

F_Sonde::F_Sonde(Arduino *oMonArduino, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_Sonde)
{
    ui->setupUi(this);
    this->oArduino = oMonArduino;

    //Affiche la date actuelle par défaut
    QDate date = QDate::currentDate();
    ui->deDateAcquisition->setDate(date);

    //Affiche l'heure actuelle par défaut
    QTime heure = QTime::currentTime();
    ui->tiHeureAcquisition->setTime(heure);

    //Indications d'écriture dans les champs d'entrée
    ui->leTpsAcquisition->setPlaceholderText("en minutes");
    //ui->leIntervalle->setPlaceholderText("en secondes");

    //Valide uniquement des chiffres de 1 à 9999
    //QValidator *ValidatorIntervalle = new QIntValidator(1, 9999, this);
    //ui->leIntervalle->setValidator(ValidatorIntervalle);
    QValidator *ValidatorTpsMesure = new QIntValidator(1, 9999, this);
    ui->leTpsAcquisition->setValidator(ValidatorTpsMesure);

    //Met l'icone sur le bouton lancer
    ui->btnLancer->setText("");
    ui->btnLancer->setIcon(QIcon(":/new/prefix1/images/icones/start.ico"));
    ui->btnLancer->setIconSize(QSize(30,30));
    ui->btnLancer->setCheckable(true);
    ui->btnLancer->setEnabled(false);

    //Met l'icone sur le bouton stopper
    ui->btnEnregistrement->setText("");
    ui->btnEnregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
    ui->btnEnregistrement->setIconSize(QSize(30,30));
    ui->btnEnregistrement->setCheckable(true);


    //Gere le tableau (nombre de colonnes, taille, intitulé)
    ui->tvValeurs->setColumnCount(1);
    ui->tvValeurs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvValeurs->setColumnWidth(0,215);
    ui->tvValeurs->setHorizontalHeaderLabels(QStringList()<<"Valeur");
    ui->tvValeurs->verticalHeader()->setFixedWidth(30);
}

F_Sonde::~F_Sonde()
{
    delete ui;
}

void F_Sonde::on_btnModeAcquisition_clicked()//Action lors du clique sur le bouton de mode d'acquisition
{
    if (ui->btnModeAcquisition->text() == "Automatique")//Si le texte du bouton est 'Automatique'
    {
        //Si choix de l'acquisition en automatique, rend non visibles les objets graphiques et change du texte du bouton
        ui->btnModeAcquisition->setText("Manuel");
        /*ui->tlTpsAcquisition->hide();
        ui->leTpsAcquisition->hide();
        ui->tlIntervalle->hide();
        ui->spIntervalle->hide();*/
        ui->tlDateHeure->hide();
        ui->deDateAcquisition->hide();
        ui->tiHeureAcquisition->hide();
        ui->btnLancer->setEnabled(true);

    }
    else if (ui->btnModeAcquisition->text() == "Manuel")//Si le texte du bouton est 'Manuel'
    {
        //Si choix de l'acquisition en manuel, affichage des objets graphiques et changement du texte du bouton
        ui->btnModeAcquisition->setText("Automatique");
        /*ui->tlTpsAcquisition->show();
        ui->leTpsAcquisition->show();
        ui->tlIntervalle->show();
        ui->spIntervalle->show();*/
        ui->tlDateHeure->show();
        ui->deDateAcquisition->show();
        ui->tiHeureAcquisition->show();
        ui->btnLancer->setEnabled(false);
    }
}

void F_Sonde::on_btnLancer_clicked()//Action lorsque le bouton Lancer est appuyé
{

    //**************************ICONE******************************
    static bool bEtatLancer(true);

    //Change l'icone Lancer du touton par l'icone Stop
    if (bEtatLancer == true)
    {
        //Met l'icone sur le bouton stopper
        ui->btnLancer->setText("");
        ui->btnLancer->setIcon(QIcon(":/new/prefix1/images/icones/stop.ico"));
        ui->btnLancer->setIconSize(QSize(30,30));
        ui->btnLancer->setCheckable(true);
        bEtatLancer = false;
    }
    else if (bEtatLancer == false)
    {
        //Met l'icone sur le bouton lancer
        ui->btnLancer->setText("");
        ui->btnLancer->setIcon(QIcon(":/new/prefix1/images/icones/start.ico"));
        ui->btnLancer->setIconSize(QSize(30,30));
        ui->btnLancer->setCheckable(true);
        bEtatLancer = true;
    }
    //******************************************************

    //++++++++++++++++++++VERIFICATION+++++++++++++++++++

    //Verifie si une valeur a été rentrée dans les champs
    if (ui->spIntervalle->value() == NULL || ui->leTpsAcquisition == NULL)
    {
        //Remet en place l'icone sur le bouton lancer
        ui->btnLancer->setText("");
        ui->btnLancer->setIcon(QIcon(":/new/prefix1/images/icones/start.ico"));
        ui->btnLancer->setIconSize(QSize(30,30));
        ui->btnLancer->setCheckable(true);
        bEtatLancer = true;

        //Affiche un message d'erreur
        QMessageBox::about(this, tr("Erreur"),
        tr("Vous avez oublié de rentrer un temps et/ou un intervalle d'acquisition !"));
        return;//Quitter la fonction
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++


    //---------------------AFFICHAGE-------------------

    //Declaration des variables pour tableau
    int Affichage(0);
    int nIntervalle(0);
    int nDureeMesure(0);
    int nDureeTotale(0);

    //Calcul des temps d'acquisition et nombre de mesures
    nIntervalle = ui->spIntervalle->value();
    nDureeMesure = ui->leTpsAcquisition->text().toInt();
    nDureeMesure = nDureeMesure*60;
    nDureeTotale = nDureeMesure/nIntervalle;
    /*qDebug()<<nIntervalle;
    qDebug()<<nDureeMesure;
    qDebug()<<nDureeTotale;*/

    for(unsigned int i=nDureeTotale;i<0;i--)
    {
        //Récupération de la données et affichage sur lecteur LCD
        Affichage = this->oArduino->LireCapteur("A10");
        qDebug()<<Affichage;
        ui->lcdValeur->display(Affichage);
        sleep(nIntervalle);
     }
    //----------------------------------------------------
}

void F_Sonde::on_btnEnregistrement_clicked(bool checked)//Action lorsque le bouton Stopper est appuyé
{
    //************************ICONE*************************
    static bool bEtatRecup(true);

    //Change l'icone du bouton Record par l'icone Stop
    if (bEtatRecup == true)
    {
        //Met l'icone sur le bouton stopper
        ui->btnEnregistrement->setText("");
        ui->btnEnregistrement->setIcon(QIcon(":/new/prefix1/images/icones/stop.ico"));
        ui->btnEnregistrement->setIconSize(QSize(30,30));
        ui->btnEnregistrement->setCheckable(true);
        bEtatRecup = false;
    }
    else if (bEtatRecup == false)
    {
        //Met l'icone sur le bouton lancer
        ui->btnEnregistrement->setText("");
        ui->btnEnregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
        ui->btnEnregistrement->setIconSize(QSize(30,30));
        ui->btnEnregistrement->setCheckable(true);
        bEtatRecup = true;
    }
    //*****************************************************

    //++++++++++++++++++++++++++++VERIFICATION++++++++++++++++++++++++++++++++

    if (ui->spIntervalle->value() == NULL || ui->leTpsAcquisition == NULL)
    {
        //Met l'icone sur le bouton lancer
        ui->btnEnregistrement->setText("");
        ui->btnEnregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
        ui->btnEnregistrement->setIconSize(QSize(30,30));
        ui->btnEnregistrement->setCheckable(true);
        bEtatRecup = true;

        QMessageBox::about(this, tr("Erreur"),
        tr("Vous avez oublié de rentrer un temps et/ou un intervalle d'acquisition !"));
        return;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //---------------------TABLEAU------------------------

    //Declaration des variables pour tableau
    int Affichage(0);
    int nIntervalle(0);
    int nDureeMesure(0);
    int nDureeTotale(0);

    //Calcul des temps d'acquisition et nombre de mesures
    nIntervalle = ui->spIntervalle->value();
    nDureeMesure = ui->leTpsAcquisition->text().toInt();
    nDureeMesure = nDureeMesure*60;
    nDureeTotale = nDureeMesure/nIntervalle;
    /*qDebug()<<nIntervalle;
    qDebug()<<nDureeMesure;
    qDebug()<<nDureeTotale;*/

    /*for(unsigned int i=nDureeTotale;i<0;i--)
    {*/

        //Récupération de la données et affichage sur lecteur LCD
        Affichage = 25; //this->oArduino->LireCapteur("A10");
        qDebug()<<Affichage;
        ui->lcdValeur->display(Affichage);
        ui->tvValeurs->setRowCount(nDureeTotale);

        //Affichage de la valeur dans le tableau
        QString Var;
        QTableWidgetItem *item = new QTableWidgetItem(Var.setNum(Affichage));
        ui->tvValeurs->setItem(0,0,item);
        //sleep(2);

    //}

    //Nom du fichier à partir de la date et l'heure
    QTime heure = QTime::currentTime();
    QDate date = QDate::currentDate();
    QString FormatDate = "dd-MM-yyyy";
    QString FormatHeure = "HH-mm";
    QString Heure;
    QString Date;
    Heure = heure.toString(FormatHeure);
    Date = date.toString(FormatDate);
    QString NomTempo;
    NomTempo = Date + "_" + Heure + ".csv";
    const char* NomFichier = NomTempo.toStdString().c_str();

    //Création et remplissage du fichier
    ofstream file (NomFichier, ios::out);
    file << "#" << ";" << "Valeur" << endl;

    /*for(i=0;i<nDureeTotale;i++)
    {*/
        file << "1" << ";" << Affichage << endl;
    //}

     //----------------------------------------------
}
