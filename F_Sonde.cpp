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

    //Met l'icone sur le bouton stopper
    ui->btnStopper->setText("");
    ui->btnStopper->setIcon(QIcon(":/new/prefix1/stop.ico"));
    ui->btnStopper->setIconSize(QSize(30,30));
    ui->btnStopper->setCheckable(true);


}

F_Sonde::~F_Sonde()
{
    delete ui;
}

void F_Sonde::on_btnModeAcquisition_clicked()
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

void F_Sonde::on_btnLancer_clicked()
{
    /*int test(0);
    int i(0);

    while(i < 5)
    {
        test = this->oArduino->LireCapteur("A10");
        ui->lcdValeur->display(test);
        i++;
    }
    if (ui->cbModeVisualisation->isChecked() == false)
    {
        ofstream file ("Test.csv", ios::out);
        file << test << ";" << test << "\n" << endl;

    }*/

}

void F_Sonde::on_btnStopper_clicked(bool checked)
{

}
