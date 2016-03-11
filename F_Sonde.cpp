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
    ui->leIntervalle->setPlaceholderText("en secondes");

    QValidator *ValidatorIntervalle = new QIntValidator(1, 9999, this);
    ui->leIntervalle->setValidator(ValidatorIntervalle);
    QValidator *ValidatorTpsMesure = new QIntValidator(1, 9999, this);
    ui->leTpsAcquisition->setValidator(ValidatorTpsMesure);

    /*QIcon icon;
    icon.addFile("test.ico", QSize(65,65), QIcon::Normal, QIcon::On);*/
    ui->btnLancer->setIcon(QIcon(":/new/prefix1/test.ico"));
    ui->btnLancer->setIconSize(QSize(65,65));
    ui->btnLancer->setCheckable(true);


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
        ui->tlTpsAcquisition->hide();
        ui->leTpsAcquisition->hide();
        ui->tlIntervalle->hide();
        ui->leIntervalle->hide();
        ui->tlDateHeure->hide();
        ui->deDateAcquisition->hide();
        ui->tiHeureAcquisition->hide();

    }
    else if (ui->btnModeAcquisition->text() == "Manuel")//Si le texte du bouton est 'Manuel'
    {
        //Si choix de l'acquisition en manuel, affichage des objets graphiques et changement du texte du bouton
        ui->btnModeAcquisition->setText("Automatique");
        ui->tlTpsAcquisition->show();
        ui->leTpsAcquisition->show();
        ui->tlIntervalle->show();
        ui->leIntervalle->show();
        ui->tlDateHeure->show();
        ui->deDateAcquisition->show();
        ui->tiHeureAcquisition->show();
    }
}

void F_Sonde::on_cbModeVisualisation_toggled(bool checked)
{
    if (checked == true)//Si mode visualisation coché, on désactive les différents objets graphiques
    { 
        ui->tvValeurs->setEnabled(false);
        ui->leTpsAcquisition->setEnabled(false);
        ui->leIntervalle->setEnabled(false);
        ui->deDateAcquisition->setEnabled(false);
        ui->tiHeureAcquisition->setEnabled(false);
        ui->btnLancer->setEnabled(false);
        ui->btnStopper->setEnabled(false);
    }
    else if (checked == false)//Si mode visualisation décoché, on les réactives
    {
        ui->tvValeurs->setEnabled(true);
        ui->leTpsAcquisition->setEnabled(true);
        ui->leIntervalle->setEnabled(true);
        ui->deDateAcquisition->setEnabled(true);
        ui->tiHeureAcquisition->setEnabled(true);
        ui->btnLancer->setEnabled(true);
        ui->btnStopper->setEnabled(true);

    }
}

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
