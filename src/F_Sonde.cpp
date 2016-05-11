//-------------------------------------------------------------------------------
/**
 * @file        F_Sonde.cpp
 * @brief       Configuration de la fenêtre de sonde
 *
 * @author      S.MENARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        27/04/16
 *
 * Lors de l'appel au constructeur de la classe, la fenêtre de la sonde apparait.
 * Il est possible alors de configurer les mesures (l'intervalle, le temps, la date et heure), puis ensuite de choisir
 * d'afficher uniquement, ou de sauvegarder les données dans un fichier .csv
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "F_Sonde.h"
#include "ui_F_Sonde.h"
#include "F_Principale.h"
#include "Arduino.h"

//=====   En-Têtes standards    =====
#include <QDate>
#include <QTime>
#include <QValidator>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QPixmap>
#include <QStandardItem>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QSemaphore>
#include <QTextCodec>

#include "Parseur/muParserTest.h"

using namespace std;
using namespace mu;


//-------------------------------------------------------------------------------

/**
 * Constructeur par défaut.
 */
F_Sonde::F_Sonde(Arduino *oMonArduino, Capteur* oLeCapteur, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_Sonde),
    bEtatLancer(true),
    bEtatRecup(true),
    nCompteurSauvegarde(0),
    nCompteurAffichage(0)
{
    ui->setupUi(this);
    this->oArduino = oMonArduino;
    this->oLeCapteur = oLeCapteur;
    //this->Semaphore = new QSemaphore (1);

    //
    this->setWindowTitle(this->oLeCapteur->Get_sTitre()+ " - Adresse : " + this->oLeCapteur->Get_sNomBroche());

    //Qt::WindowFlags flags = this->windowFlags();
    //flags = Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint;
    //this->setWindowFlags(Qt::FramelessWindowHint);

    ui->Lb_Valeur->setText("Valeur (en " + this->oLeCapteur->Get_sUnite() + ") :");
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    //Création des timers pour affichage et sauvegarde des données
    this->tmMinuteurIntervalleMesuresAffichage = new QTimer(this);
    this->tmMinuteurIntervalleMesuresAffichage->connect(this->tmMinuteurIntervalleMesuresAffichage, SIGNAL(timeout()), this, SLOT(Affichage()));

    this->tmMinuteurIntervalleMesuresSauvegarde = new QTimer(this);
    this->tmMinuteurIntervalleMesuresSauvegarde->connect(this->tmMinuteurIntervalleMesuresSauvegarde, SIGNAL(timeout()), this, SLOT(Sauvegarde()));

    //Affiche la date actuelle par défaut
    QDate dDateCourante = QDate::currentDate();
    ui->DtE_DateAcquisition->setDate(dDateCourante);

    //Affiche l'heure actuelle par défaut
    QTime tHeureCourante = QTime::currentTime();
    ui->TiE_HeureAcquisition->setTime(tHeureCourante);

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


    //Gerer le tableau (nombre de colonnes, taille, intitulé)
    ui->TbW_Valeurs->setColumnCount(1);
    ui->TbW_Valeurs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->TbW_Valeurs->setColumnWidth(0,222);
    ui->TbW_Valeurs->horizontalHeader()->setStretchLastSection(true);
    ui->TbW_Valeurs->setHorizontalHeaderLabels(QStringList()<<"Valeur");
    ui->TbW_Valeurs->verticalHeader()->setFixedWidth(30);

    //Gerer les minimum des champs d'intervalle et de temps de donnees
    ui->SBx_Intervalle->setMinimum(1);
    ui->SBx_TempsAcquisition->setMinimum(1);


}
//=======================================================================================================================
/**
 * Déstructeur de la classe.
 */
F_Sonde::~F_Sonde()
{
    delete ui;
}
//=======================================================================================================================
/**
 * Cette méthode va permettre de passer du mode automatique d'acquisition (acquerir à une certaine heure ou date)
 * au mode manuel (acquisition lors de l'appuis sur le bouton d'acquisition).
 */
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
/**
 * Cette méthode va permettre l'affichage des valeur récupérées dans l'écran type LCD du programme via la sonde en utilisant la fonction Afficher().
 * Si l'intervalle ou le temps d'acquisition ne sont pas renseigné, un message d'erreur apparait.
 */
void F_Sonde::on_Bt_Lancer_clicked()//Action lorsque le bouton Lancer est appuyé
{

    //Acquisition du sémaphore
    //this->Semaphore->acquire(1);

    //Change l'icone Lancer du bouton par l'icone Stop, verifie les valeurs puis lance la capture et l'affichage
    if (this->bEtatLancer == true)
    {
        //Met l'icone sur le bouton stopper
        ui->Bt_Lancer->setText("");
        ui->Bt_Lancer->setIcon(QIcon(":/new/prefix1/images/icones/stop.ico"));
        ui->Bt_Lancer->setIconSize(QSize(30,30));
        ui->Bt_Lancer->setCheckable(true);
        this->bEtatLancer = false;
        ui->Bt_Lancer->update();

        //+++++++++++++++++++++++++++++++++VERIFICATION+++++++++++++++++++++++++++++++++

        //Verifie si une valeur a été rentrée dans les champs
        if (ui->SBx_Intervalle->value() == NULL || ui->SBx_TempsAcquisition == NULL || this->oArduino->LireCapteur(this->oLeCapteur->Get_sCommande()+this->oLeCapteur->Get_sNomBroche()) == -1)
        {
            //Remet en place l'icone sur le bouton lancer
            ui->Bt_Lancer->setText("");
            ui->Bt_Lancer->setIcon(QIcon(":/new/prefix1/images/icones/start.ico"));
            ui->Bt_Lancer->setIconSize(QSize(30,30));
            ui->Bt_Lancer->setCheckable(true);

            this->bEtatLancer = false;

            //Affiche un message d'erreur
            QMessageBox::about(this, tr("Erreur"),
            tr("Vous avez oublié de rentrer un temps et/ou un intervalle d'acquisition !"));
        }
        //-------------------------------FIN_VERIFICATION-------------------------------
        else
        {
        //+++++++++++++++++++++++++++++++++++AFFICHAGE++++++++++++++++++++++++++++++++++
            //ui->Bt_Enregistrement->setEnabled(false);

            //Calcul des temps d'acquisition et nombre de mesures
            this->nIntervalle = ui->SBx_Intervalle->value();
            this->nDureeMesure = ui->SBx_TempsAcquisition->text().toInt();
            this->nDureeMesure = nDureeMesure*10;
            this->nDureeTotale = nDureeMesure/nIntervalle;


            this->tmMinuteurIntervalleMesuresAffichage->start(nIntervalle*1000);


            //-------------------------------FIN_AFFICHAGE-------------------------------
        }

    }
    else if (this->bEtatLancer == false)
    {
        //Met l'icone sur le bouton lancer
        ui->Bt_Lancer->setText("");
        ui->Bt_Lancer->setIcon(QIcon(":/new/prefix1/images/icones/start.ico"));
        ui->Bt_Lancer->setIconSize(QSize(30,30));
        ui->Bt_Lancer->setCheckable(true);
        this->bEtatLancer = true;
        this->tmMinuteurIntervalleMesuresAffichage->stop();
        ui->Lcd_Valeur->display(0);

    }

}
//=======================================================================================================================
/**
 * Cette méthode va permettre l'affichage des valeur récupérées dans l'écran type LCD du programme via la sonde en utilisant la fonction Sauvegarder().
 * Si l'intervalle ou le temps d'acquisition ne sont pas renseigné, un message d'erreur apparait.
 */
void F_Sonde::on_Bt_Enregistrement_clicked()//Action lorsque le bouton Stopper est appuyé
{
    //Acquisition du sémaphore
    //this->Semaphore->acquire(1);

    //++++++++++++++++++++++++++++++++++++ICONE++++++++++++++++++++++++++++++++++++

    //Change l'icone du bouton Record par l'icone Stop
    if (this->bEtatRecup == true)
    {
        //Met l'icone sur le bouton stopper
        ui->Bt_Enregistrement->setText("");
        ui->Bt_Enregistrement->setIcon(QIcon(":/new/prefix1/images/icones/stop.ico"));
        ui->Bt_Enregistrement->setIconSize(QSize(30,30));
        ui->Bt_Enregistrement->setCheckable(true);
        bEtatRecup = false;
        ui->TbW_Valeurs->clear();
        this->nCompteurSauvegarde = 0;
        //this->fpFichierDonnees.close();

        //+++++++++++++++++++++++++++++++++VERIFICATION+++++++++++++++++++++++++++++++++

        if (ui->SBx_Intervalle->value() == NULL || ui->SBx_TempsAcquisition == NULL)
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

            //Calcul des temps d'acquisition et nombre de mesures
            this->nIntervalle = ui->SBx_Intervalle->value();
            this->nDureeMesure = ui->SBx_TempsAcquisition->text().toInt();
            this->nDureeMesure = nDureeMesure*10;
            this->nDureeTotale = nDureeMesure/nIntervalle;
            ui->TbW_Valeurs->setHorizontalHeaderLabels(QStringList()<<"Valeur");

        //+++++++++++++++++++++++++++++++CREATION_FICHIER+++++++++++++++++++++++++++++++

            //Nom du fichier à partir de la date et l'heure
            QTime tHeureCourante = QTime::currentTime();
            QDate dDateCourante = QDate::currentDate();
            QString sFormatDate = "dd-MM-yyyy";
            QString sFormatHeureSauvegarde = "HH-mm-ss";

            QString sHeure;
            QString sDate;
            sHeure = tHeureCourante.toString(sFormatHeureSauvegarde);
            sDate = dDateCourante.toString(sFormatDate);
            QString sNomTemporaire;
            sNomTemporaire = this->oLeCapteur->Get_sTitre() + "_" + sDate + "_" + sHeure + ".csv";
            const char* cNomFichier = sNomTemporaire.toStdString().c_str();
            qDebug()<<cNomFichier;

            //Création et remplissage du fichier
            QString Valeur;
            Valeur = "Valeur (en " + this->oLeCapteur->Get_sUnite() + ")";
            this->fpFichierDonnees.open(cNomFichier, ios::out);

            this->fpFichierDonnees << "#" << ";" << Valeur.toStdString().c_str() << ";" << "Date" << ";" << "Heure" << endl;

            this->tmMinuteurIntervalleMesuresSauvegarde->start(nIntervalle*1000);



        //----------------------------FIN_AFFICHAGE_TABLEAU-----------------------------
        }
    }
    else if (this->bEtatRecup == false)
    {
        //Met l'icone sur le bouton lancer
        ui->Bt_Enregistrement->setText("");
        ui->Bt_Enregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
        ui->Bt_Enregistrement->setIconSize(QSize(30,30));
        ui->Bt_Enregistrement->setCheckable(true);
        this->bEtatRecup = true;

        this->tmMinuteurIntervalleMesuresSauvegarde->stop();
        ui->Lcd_Valeur->display(0);
    }
    //----------------------------------FIN_ICONE----------------------------------

}
//=======================================================================================================================
/**
 * Cette méthode va permettre la valeur de la sonde, son affichage dans l'afficheur type LCD, dans le tableau
 * de la fenêtre ainsi que l'exportation dans un fichier .csv
 */
void F_Sonde::Sauvegarde()
{

    int nAffichageValeur(0);
    //static int Compteur(0);
    QTime tHeureCourante = QTime::currentTime();
    QDate dDateCourante = QDate::currentDate();
    QString sFormatDate = "dd-MM-yyyy";
    QString sFormatHeureSauvegarde = "HH-mm-ss";
    ui->TbW_Valeurs->setHorizontalHeaderLabels(QStringList()<<"Valeur");

    QString sHeure;
    QString sDate;
    sHeure = tHeureCourante.toString(sFormatHeureSauvegarde);
    sDate = dDateCourante.toString(sFormatDate);

    QString sFormatHeureAffichage = "HH:mm:ss";
    /*QTime tHeureActuelle = QTime::currentTime();
    QDate dDateActuelle = QDate::currentDate();*/
    sHeure = tHeureCourante.toString(sFormatHeureAffichage);
    sDate = dDateCourante.toString(sFormatDate);

    //Récupération de la donnée et affichage sur lecteur LCD
    nAffichageValeur = this->oArduino->LireCapteur(this->oLeCapteur->Get_sCommande()+this->oLeCapteur->Get_sNomBroche());
    qDebug()<<nAffichageValeur;
    ui->Lcd_Valeur->display(nAffichageValeur);
    ui->TbW_Valeurs->setRowCount(this->nDureeTotale);

    //Affichage de la valeur dans le tableau
    QString sTemporaire;
    QTableWidgetItem *Objet = new QTableWidgetItem(sTemporaire.setNum(nAffichageValeur));
    Objet->setTextAlignment(Qt::AlignCenter);
    ui->TbW_Valeurs->setItem(this->nCompteurSauvegarde,0,Objet);
    this->fpFichierDonnees << this->nCompteurSauvegarde+1 << ";" << nAffichageValeur << ";" << sDate.toStdString() << ";" << sHeure.toStdString() << endl;
    this->nCompteurSauvegarde++;

    if (this->nCompteurSauvegarde >= this->nDureeTotale)
    {
        this->StopperSauvegarde();
    }
}

void F_Sonde::StopperAffichage()
{
    ui->Bt_Lancer->setIcon(QIcon(":/new/prefix1/images/icones/start.ico"));
    this->tmMinuteurIntervalleMesuresAffichage->stop();
    this->bEtatLancer = true;
    ui->Lcd_Valeur->display(0);
    this->nCompteurAffichage = 0;
    ui->Bt_Enregistrement->setEnabled(true);
    //this->Semaphore->release(1);//On libère le sémaphore
}

void F_Sonde::StopperSauvegarde()
{
    ui->Bt_Enregistrement->setIcon(QIcon(":/new/prefix1/images/icones/rec.ico"));
    this->tmMinuteurIntervalleMesuresSauvegarde->stop();
    this->bEtatRecup = true;
    ui->Lcd_Valeur->display(0);
    this->nCompteurSauvegarde = 0;
    this->fpFichierDonnees.close();
    //this->Semaphore->release(1);//On libère le sémaphore
}

void F_Sonde::Expression()
{

}
//=======================================================================================================================
/**
 * Cette méthode va récupérer la valeur du capteur puis l'afficher dans l'écran type LCD du programme.
 */
void F_Sonde::Affichage()
{
    int nAffichageValeur(0);
    nAffichageValeur = this->oArduino->LireCapteur(this->oLeCapteur->Get_sCommande()+this->oLeCapteur->Get_sNomBroche());
    qDebug()<<nAffichageValeur << this->nCompteurAffichage;
    ui->Lcd_Valeur->display(nAffichageValeur);
    this->nCompteurAffichage++;

    if (this->nCompteurAffichage >= this->nDureeTotale)
    {
        this->StopperAffichage();
    }
}
//=======================================================================================================================
