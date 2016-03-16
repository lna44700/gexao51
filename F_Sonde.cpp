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
    ui->btnLancer->setIcon(QIcon(":/new/prefix1/start.ico"));
    ui->btnLancer->setIconSize(QSize(30,30));
    ui->btnLancer->setCheckable(true);
    ui->btnLancer->setEnabled(false);

    //Met l'icone sur le bouton stopper
    ui->btnEnregistrement->setText("");
    ui->btnEnregistrement->setIcon(QIcon(":/new/prefix1/rec.ico"));
    ui->btnEnregistrement->setIconSize(QSize(30,30));
    ui->btnEnregistrement->setCheckable(true);


    ui->tvValeurs->setRowCount(5);
    ui->tvValeurs->setColumnCount(1);
    ui->tvValeurs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvValeurs->setColumnWidth(0,215);
    ui->tvValeurs->setHorizontalHeaderLabels(QStringList()<<"Valeur");
    ui->tvValeurs->verticalHeader()->setFixedWidth(30);
    //ui->tvValeurs->verticalHeader()->setStr
    //m_TableHeader<<"#"<<"Name"<<"Text";
    //ui->tvValeurs->setHorizontalHeaderLabels(m_TableHeader);


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

//-------------------A ENLEVER PLUS TARD-------------------
/*void F_Sonde::on_cbModeVisualisation_toggled(bool checked)
{
    if (checked == true)//Si mode visualisation coché, on désactive les différents objets graphiques
    { 
        ui->tvValeurs->setEnabled(false);
        ui->leTpsAcquisition->setEnabled(false);
        ui->spIntervalle->setEnabled(false);
        ui->deDateAcquisition->setEnabled(false);
        ui->tiHeureAcquisition->setEnabled(false);
        ui->btnLancer->setEnabled(false);
        ui->btnStopper->setEnabled(false);
    }
    else if (checked == false)//Si mode visualisation décoché, on les réactives
    {
        ui->tvValeurs->setEnabled(true);
        ui->leTpsAcquisition->setEnabled(true);
        ui->spIntervalle->setEnabled(true);
        ui->deDateAcquisition->setEnabled(true);
        ui->tiHeureAcquisition->setEnabled(true);
        ui->btnLancer->setEnabled(true);
        ui->btnStopper->setEnabled(true);

    }
}*/
//------------------------------------------------------------------------

void F_Sonde::on_btnLancer_clicked()//Action lorsque le bouton Lancer est appuyé
{
    static bool bEtatLancer(true);

    //Change l'icone Lancer du touton par l'icone Stop
    if (bEtatLancer == true)
    {
        //Met l'icone sur le bouton stopper
        ui->btnLancer->setText("");
        ui->btnLancer->setIcon(QIcon(":/new/prefix1/stop.ico"));
        ui->btnLancer->setIconSize(QSize(30,30));
        ui->btnLancer->setCheckable(true);
        bEtatLancer = false;
    }
    else if (bEtatLancer == false)
    {
        //Met l'icone sur le bouton lancer
        ui->btnLancer->setText("");
        ui->btnLancer->setIcon(QIcon(":/new/prefix1/start.ico"));
        ui->btnLancer->setIconSize(QSize(30,30));
        ui->btnLancer->setCheckable(true);
        bEtatLancer = true;
    }


    int test(0);
    int i(0);

    /*while(i < 5)
    {
        test = this->oArduino->LireCapteur("A10");
        qDebug()<<test;
        ui->lcdValeur->display(test);
        /*i++;
    }*/
    /*if (ui->cbModeVisualisation->isChecked() == false)
    {
        ofstream file ("Test.csv", ios::out);
        file << test << ";" << test << "\n" << endl;

    }*/

}

void F_Sonde::on_btnEnregistrement_clicked(bool checked)//Action lorsque le bouton Stopper est appuyé
{
    static bool bEtatRecup(true);

    //Change l'icone du bouton Record par l'icone Stop
    if (bEtatRecup == true)
    {
        //Met l'icone sur le bouton stopper
        ui->btnEnregistrement->setText("");
        ui->btnEnregistrement->setIcon(QIcon(":/new/prefix1/stop.ico"));
        ui->btnEnregistrement->setIconSize(QSize(30,30));
        ui->btnEnregistrement->setCheckable(true);
        bEtatRecup = false;
    }
    else if (bEtatRecup == false)
    {
        //Met l'icone sur le bouton lancer
        ui->btnEnregistrement->setText("");
        ui->btnEnregistrement->setIcon(QIcon(":/new/prefix1/rec.ico"));
        ui->btnEnregistrement->setIconSize(QSize(30,30));
        ui->btnEnregistrement->setCheckable(true);
        bEtatRecup = true;
    }
}
