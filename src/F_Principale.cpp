#include "F_Principale.h"
#include "ui_F_Principale.h"
#include <F_Sonde.h>
#include "F_ChoisirSonde.h"
#include <QtGui>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <unistd.h>
#include <Arduino.h>
#include <QPixmap>
#include <QLabel>
#include <QFileDialog>

F_Principale::F_Principale(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::F_Principale),
    DonneesLues(0)//à enlever quand f_sonde sera terminée
{ 
    ui->setupUi(this);

    //Ajout de l'icone de l'application
    QIcon icon(":/new/prefix1/images/icones/icone_g_key.ico");
    QWidget::setWindowIcon(icon);

    //Définition des icones et texte pour la détection de l'Arduino
    QLabel *iconLbl = new QLabel;
    QLabel *texteLbl = new QLabel;
    QPixmap IconeVert(":/new/prefix1/images/icones/led_verte_red.ico");
    QPixmap IconeRouge(":/new/prefix1/images/icones/led_rouge_red.ico");

    this->oMonArduino = new Arduino;

    /*this->oMonArduino->Ouvrir();
    sleep(1);*/

    //Lecture du port pour lire le nom


    /*this->DonneesLues = oMonArduino->LireCapteur("A10");
    qDebug() << this->DonneesLues;*/

    //Ajout de la led de détection de l'Arduino - Vert si détection, rouge sinon
    if(this->oMonArduino->Ouvrir() == true)
    {
        sleep(1);
        //Etat = this->oMonArduino->LirePort();
        iconLbl->setPixmap(IconeVert);
        texteLbl->setText("Arduino détecté !");

        ui->statusBar->addWidget(iconLbl);
        ui->statusBar->addWidget(texteLbl);
    }
    else
    {
        iconLbl->setPixmap(IconeRouge);
        texteLbl->setText("Arduino non détecté !");
        //Etat = texteLbl->text();
        ui->statusBar->addWidget(iconLbl);
        ui->statusBar->addWidget(texteLbl);
    }
}

F_Principale::~F_Principale()
{
    delete ui;
}

void F_Principale::displayAbout() //action qui affiche un message en pop-up
{
    QMessageBox::about(this, tr("À propos"),
    tr("<p>Ce logiciel a été développé par : <br/> Sylvain GUICHARD et Sylvain MENARD, étudiants au Lycée Nicolas Appert à Orvault "
       "<br/> <br/> Remerciments pour les icones : "
       "<br/> <a href=\"http://www.proglyphs.com/\">http://www.proglyphs.com/</a> "
       "<br/> <a href=\"https://www.iconfinder.com/paomedia\">https://www.iconfinder.com/paomedia</a> "
       "<br/> <a href=\"http://wefunction.com/\">http://wefunction.com/</a> "
       "<br/> <a href=\"https://www.iconfinder.com/Chanut-is\">https://www.iconfinder.com/Chanut-is</a> "
       "<br/> <a href=\"https://www.iconfinder.com/icons4android.com\">https://www.iconfinder.com/icons4android.com</a> "
       "<br/> <br/> Année 2016 "
       "<br/> <br/> Version 0.01</p>")); // le message qui doit s'afficher
}

void F_Principale::createChild() //action de création de la fenêtre enfant
{
    F_Sonde *f_sonde = new F_Sonde(this->oMonArduino, static_cast<QWidget*>(ui->mdiArea)); // création d'une variable de type f_sonde
    //connect(this,)
    f_sonde->setAttribute(Qt::WA_DeleteOnClose); //detruit le widget lors de la fermeture de l'évenement
    ui->mdiArea->addSubWindow(f_sonde); // ajoute la fenêtre enfant f_sonde à la fenêtre MDI
    f_sonde->show(); // affiche la fenêtre enfant
}

void F_Principale::displaySelection()//action d'affichage fenêtre de séléction des sondes
{
    F_ChoisirSonde *f_choix = new F_ChoisirSonde(this->oMonArduino, &(this->Config), this->Liste) ;
    f_choix->show();
}

//Affiche la même heure et date partout sur la base de la première fenêtre
void F_Principale::on_actionHeureDatePartout_triggered()
{
    /*ui->mdiArea->sub
    foreach (F_Sonde *f_sonde, ui->mdiArea)
    {
        f_sonde->
    }*/
}

//Fenêtre d'ouverture des paramètres d'acquisition
void F_Principale::on_actionOuvrir_triggered()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir les paramètres d'acquisition", QString(), "Fichier de paramètre (*.ini)");
}

//Fenêtre de sauvegarde des paramètres d'acquisition
void F_Principale::on_actionSauvegarder_triggered()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer les paramètres d'acquisition", QString(), "Fichier de paramètre (*.ini)");
}
