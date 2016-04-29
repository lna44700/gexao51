/**
 * @file        F_Principale.cpp
 * @brief       Création de la fenêtre principale du programme
 *
 * @author      S.GUICHARD
 * @author      S.MENARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        27/04/16
 *
 * Lors de l'appel au constructeur de la classe, la création de la fenêtre principale du programme contenant
 * les différentes barres (outil, menu, état) et la zone permettant l'accueil de la fenêtre enfant.
 * S'occupe également d'ouvrir la communication entre l'Arduino et le programme.
 * Enfin, contient également les déclarations des différentes fenêtres de choix et de sélections des sondes.
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "F_Principale.h"
#include "ui_F_Principale.h"
#include "F_Sonde.h"
#include "F_ChoisirSonde.h"

//=====   En-Têtes standards    =====
#include <QtGui>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <unistd.h>
#include <Arduino.h>
#include <QPixmap>
#include <QLabel>
#include <QFileDialog>
#include <QWidget>
#include <QGraphicsView>
#include <QListWidgetItem>


/**
 * Constructeur par défaut.
 */
F_Principale::F_Principale(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::F_Principale),
    nDonneesLues(0)//à enlever quand f_sonde sera terminée
{ 
    ui->setupUi(this);

    //Ajout de l'icone de l'application
    QIcon icApplication(":/new/prefix1/images/icones/icone_g_key.ico");
    QWidget::setWindowIcon(icApplication);

    //Définition des icones et texte pour la détection de l'Arduino
    QLabel *lbBarreStatutIcone = new QLabel;
    QLabel *lbBarreStatutLabel = new QLabel;
    QPixmap pmIconeVert(":/new/prefix1/images/icones/led_verte_red.ico");
    QPixmap pmIconeRouge(":/new/prefix1/images/icones/led_rouge_red.ico");

    this->oMonArduino = new Arduino;

    /*this->oMonArduino->Ouvrir();
    this->oMonArduino->LirePort();
    sleep(1);*/

    //Lecture du port pour lire le nom


    /*this->nDonneesLues = oMonArduino->LireCapteur("A10");
    qDebug() << this->nDonneesLues;*/

    //Ajout de la led de détection de l'Arduino - Vert si détection, rouge sinon
    if(this->oMonArduino->Ouvrir() == true)
    {
        sleep(1);
        //this->oMonArduino->LirePort();
        lbBarreStatutIcone->setPixmap(pmIconeVert);
        lbBarreStatutLabel->setText("Arduino détecté !");
        ui->BarreStatut->addWidget(lbBarreStatutIcone);
        ui->BarreStatut->addWidget(lbBarreStatutLabel);
        sleep(1);

    }
    else
    {
        lbBarreStatutIcone->setPixmap(pmIconeRouge);
        lbBarreStatutLabel->setText("Arduino non détecté !");
        ui->BarreStatut->addWidget(lbBarreStatutIcone);
        ui->BarreStatut->addWidget(lbBarreStatutLabel);
        ui->actionChoixSondes->setEnabled(false);
    }

    f_choix = new F_ChoisirSonde(this->oMonArduino/*, &(this->Config), this->Liste*/) ;

    this->ListeCapteurI2C = this->f_choix->ListeCapteurI2C;
}

/**
 * Déstructeur de la classe
 */
F_Principale::~F_Principale()
{
    delete ui;
}

/**
 * Cette méthode va permettre l'affichage de la fenêtre "A propos" lors de l'appui sur le bouton
 * Elle donnera des informations sur les développeurs de l'application, et des liens vers les sources d'images.
 */
void F_Principale::FenetreAPropos() //action qui affiche un message en pop-up
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

/**
 * Cette méthode va permettre l'affichage de la fenêtre enfant dans la zone MDI de l'application.
 */
void F_Principale::FenetreEnfant() //action de création de la fenêtre enfant
{


    F_Sonde *oFenetreSonde = new F_Sonde(this->oMonArduino, static_cast<QWidget*>(ui->ZoneMDI)); // création d'une variable de type f_sonde
    oFenetreSonde->setAttribute(Qt::WA_DeleteOnClose); //detruit le widget lors de la fermeture de l'évenement
    ui->ZoneMDI->addSubWindow(oFenetreSonde); // ajoute la fenêtre enfant f_sonde à la fenêtre MDI
    oFenetreSonde->show(); // affiche la fenêtre enfant

    QListWidgetItem *Sonde = new QListWidgetItem(oFenetreSonde->windowTitle(), ui->LW_ListeSondes);
    Sonde->setFlags(Sonde->flags() | Qt::ItemIsUserCheckable); // set checkable flag
    Sonde->setCheckState(Qt::Unchecked); // AND initialize check state
}

void F_Principale::displayChoixSondes()//action d'affichage fenêtre de séléction des sondes
{
    f_choix->show();
}

void F_Principale::displaySelectionSondes()
{
    this->Selection = new F_SelectionSondes();
    Selection->show();
}

//Affiche la même heure et date partout sur la base de la première fenêtre
void F_Principale::on_actionHeureDatePartout_triggered()
{
    /*ui->ZoneMDI->sub
    foreach (F_Sonde *f_sonde, ui->ZoneMDI)
    {
        f_sonde->
    }*/
}

//Fenêtre d'ouverture des paramètres d'acquisition
void F_Principale::on_actionOuvrir_triggered()
{

    QString Fichier = QFileDialog::getOpenFileName(this, "Ouvrir les paramètres d'acquisition", QString(), "Fichier de paramètre (*.gexao51)");
    // Créer un objet QFile qui contient le fichier "MonFichier.txt"
    QFile MonFichier(Fichier);
    // Ouvrir le fichier en ecriture seule
    MonFichier.open(QIODevice::ReadOnly | QIODevice::Text);
    // Création d'un objet QTextStream relatif à notre fichier (File)
    QTextStream MonFlux(&MonFichier);
    // On choisit le codec relatif au jeu de caractères voulu, pour nous UTF-8 ( c'est un standard)
    MonFlux.setCodec("UTF-8");

    if(MonFichier.exists())
    {
        this->f_choix->on_Btn_Supprimer_clicked();

        QString NombreCapteurJackLus("");
        unsigned int NombreCapteurJack(0);


        NombreCapteurJackLus = MonFlux.readLine();
        NombreCapteurJackLus.remove(0,18);
        NombreCapteurJack = NombreCapteurJackLus.toInt();

        for(unsigned int i=0; i<NombreCapteurJack; i++)
        {
            QString NomCapteur;
            QString NomBroche;

            NomCapteur = MonFlux.readLine();
            NomCapteur.remove(0,11);

            NomBroche = MonFlux.readLine();
            NomBroche.remove(0, 10);

            this->f_choix->Scenes[0]->ListeBroche[NomBroche.toInt()]->DefinirCapteur(NomCapteur, NomBroche);
        }

        this->f_choix->on_CB_Interface_currentIndexChanged(0);

        this->f_choix->on_Btn_Valider_clicked();
    }

}

//Fenêtre de sauvegarde des paramètres d'acquisition
void F_Principale::on_actionSauvegarder_triggered()
{
    QString Fichier=QFileDialog::getSaveFileName(this, "Export report definition", QString(),"Report definition file (*.gexao51)");
    // Créer un objet QFile qui contient le fichier "MonFichier.txt"
    QFile MonFichier(Fichier);
    // Ouvrir le fichier en ecriture seule
    MonFichier.open(QIODevice::WriteOnly | QIODevice::Text);
    // Création d'un objet QTextStream relatif à notre fichier (File)
    QTextStream MonFlux(&MonFichier);
    // On choisit le codec relatif au jeu de caractères voulu, pour nous UTF-8 ( c'est un standard)
    MonFlux.setCodec("UTF-8");

    this->ListeCapteurAnalogique = this->f_choix->ListeCapteurAnalogique;

    MonFlux << "NombreCapteurJack= " << this->ListeCapteurAnalogique.size()  << "\n";

    // Ecriture de chaque élement de la liste de lignes dans le fichier
    for (unsigned int x=0; x<this->ListeCapteurAnalogique.size(); x++)
    {
        MonFlux << "NomCapteur=";

        for (unsigned int y=0; y<this->ListeCapteurAnalogique[x]->NomCapteur.size(); y++)
        {

                MonFlux << this->ListeCapteurAnalogique[x]->NomCapteur[y];

        }

        MonFlux << "\n";

        MonFlux << "NomBroche=";

        for (unsigned int y=0; y<this->ListeCapteurAnalogique[x]->NomBroche.size(); y++)
        {

                MonFlux << this->ListeCapteurAnalogique[x]->NomBroche[y];
        }

        MonFlux << "\n";

    }

}
