//-------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------

/**
 * Constructeur par défaut.
 */
F_Principale::F_Principale(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::F_Principale),
    oMonArduino(new Arduino),
    oMonChoixSonde(NULL),
    oMaListeCalibrage(new F_ListeCalibrage),
    nDonneesLues(0),//à enlever quand f_sonde sera terminée
    pxmIconeVert(":/new/prefix1/images/icones/led_verte_red.ico"),
    pxmIconeRouge(":/new/prefix1/images/icones/led_rouge_red.ico"),
    bEstDetecte(false),
    bEtatPrecedent(false)
{
    this->ui->setupUi(this);

    //Ajout de l'icone de l'application
    QIcon icone(":/new/prefix1/images/icones/icone_g_key.ico");
    QWidget::setWindowIcon(icone);

    //Définition des icones et texte pour la détection de l'Arduino
    this->pLblIcon = new QLabel;
    this->pLblTexte = new QLabel;

    this->oMonArduino = new Arduino;

    this->oMaListeCalibrage = new F_ListeCalibrage;

    //Création de la fenêtre permettant la configuration de la maquette
    this->oMonChoixSonde = new F_ChoisirSonde(this->oMonArduino) ;

    this->setEnabled(false);

    this->pTimer = new QTimer;
    this->pTimer2 = new QTimer;

    this->pTimer->setInterval(1000);
    this->pTimer2->setInterval(1000);

    this->pTimer->start();

    connect(pTimer,SIGNAL(timeout()),this,SLOT(on_DetecterArduino()));
    connect(pTimer2,SIGNAL(timeout()),this,SLOT(on_MiseAJourQList()));

}

//-------------------------------------------------------------------------------

/**
 * Déstructeur de la classe
 */
F_Principale::~F_Principale()
{
    delete ui;
}

//-------------------------------------------------------------------------------

/**
 * Ce slot permet de détecter si l'Arduino est brancher sur un port USB.
 */
void F_Principale::on_DetecterArduino()
{

    //Si la connexion avec l'Arduino est établie
    if(this->oMonArduino->Ouvrir())
    {
        this->pLblIcon->setPixmap(this->pxmIconeVert);
        this->pLblTexte->setText("Arduino détecté !");
        this->ui->StBr->addWidget(this->pLblIcon);
        this->ui->StBr->addWidget(this->pLblTexte);

        //Boucle effectuée dès que l'Arduino est détecté
        if(this->bEstDetecte==false)
        {
            this->oMonChoixSonde->Get_lstScenes();
            this->bEstDetecte=true;

            //On démarre le Timer permettant d'afficher les fenêtres pour le calibrage des capteurs
            this->pTimer2->start();
        }

        //Si l'Arduino n'était pas détecter
        if(this->bEtatPrecedent == false)
        {
            oMonArduino->LirePort();
            sleep(1);

            this->oMonChoixSonde->ScannerAdresseI2C();

            /*for(unsigned int i=0; i<this->lstListeSonde.length();i++)
            {
                if(this->lstListeSonde[i]->isEnabled() == false)
                {
                    this->lstListeSonde[i]->setEnabled(true);
                }
            }*/

            //On parcours la liste complète des capteurs
            for(unsigned int i=0; i<lstListeCapteurComplete.length(); i++)
            {
                F_Calibrage *oMonCalibrage = new F_Calibrage(this->oMonArduino);
                oMonCalibrage = this->lstListeCapteurComplete[i]->Get_oMonCalibrage();

                //Si une fenêtre de calibrage d'un capteur est désactivée
                if(oMonCalibrage->isEnabled() == false)
                {
                    //Activation de la fenêtre
                    oMonCalibrage->setEnabled(true);
                }
            }

        }

        //l'Arduino est détecté
        this->bEtatPrecedent=true;

    }
    else     //Si la connexion avec l'Arduino n'est pas établie
    {
        //Fermeture de la connexion
        this->oMonArduino->Fermer();
        this->pLblIcon->setPixmap(pxmIconeRouge);
        this->pLblTexte->setText("Arduino non détecté !");
        this->ui->StBr->addWidget(pLblIcon);
        this->ui->StBr->addWidget(pLblTexte);

        //Si l'Arduino était détecté et que la fenêtre de configuration de la maquette est crée
        if(this->bEtatPrecedent==true && this->bEstDetecte == true)
        {
            /*for(unsigned int i=0; i<this->lstListeSonde.length();i++)
            {
                if(this->lstListeSonde[i]->isEnabled() == true)
                {
                    this->lstListeSonde[i]->setEnabled(false);
                }
            }*/

            //On parcours la liste complète des capteurs
            for(unsigned int i=0; i < this->lstListeCapteurComplete.length(); i++)
            {
                //Si une fenêtre de calibrage d'un capteur est activée
                if(this->lstListeCapteurComplete[i]->Get_oMonCalibrage()->isEnabled() == true)
                {
                    //Désactivation de la fenêtre
                    this->lstListeCapteurComplete[i]->Get_oMonCalibrage()->setEnabled(false);
                }
            }
        }

        //L'Arduino n'est pas détecté
        this->bEtatPrecedent=false;
    }

    if(this->isEnabled() == false)
    {
        this->setEnabled(true);
    }

}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va permettre l'affichage de la fenêtre "A propos" lors de l'appui sur le bouton
 * Elle donnera des informations sur les développeurs de l'application, et des liens vers les sources d'images.
 */
void F_Principale::on_FenetreAPropos() //action qui affiche un message en pop-up
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

//-------------------------------------------------------------------------------

/**
 * Ce slot va permettre l'affichage de la fenêtre enfant dans la zone MDI de l'application.
 */
void F_Principale::on_FenetreAcquisition() //action de création de la fenêtre enfant
{


    for (unsigned i=0;i<lstListeCapteurComplete.length();i++)
    {
        this->oMaSonde = new F_Sonde(this->oMonArduino, this->lstListeCapteurComplete[i], static_cast<QWidget*>(ui->MdiA_ZoneMDI)); // création d'une variable de type f_sonde
        this->oMaSonde->setAttribute(Qt::WA_DeleteOnClose); //detruit le widget lors de la fermeture de l'évenement
        //oMaSonde->setFixedSize();
        ui->MdiA_ZoneMDI->addSubWindow(this->oMaSonde); // ajoute la fenêtre enfant f_sonde à la fenêtre MDI
        this->oMaSonde->show(); // affiche la fenêtre enfant

        QListWidgetItem *Sonde = new QListWidgetItem(this->oMaSonde->windowTitle(), ui->LiW_ListeSondes);
        Sonde->setFlags(Sonde->flags() | Qt::ItemIsUserCheckable);
        Sonde->setCheckState(Qt::Checked);
        this->lstListeSonde << this->oMaSonde;
    }
}

//-------------------------------------------------------------------------------

/**
 * Ce slot va afficher la fenêtre F_ChoixSonde et lancer les timers de cette classe.
 */
void F_Principale::on_FenetreChoixSonde()//action d'affichage fenêtre de séléction des sondes
{
    this->oMonChoixSonde->show();

    QTimer *pTimerChoixSonde = new QTimer;
    pTimerChoixSonde = this->oMonChoixSonde->Get_pTimer();
    pTimerChoixSonde->start();

    QTimer *pTimer2ChoixSonde = new QTimer;
    pTimer2ChoixSonde = this->oMonChoixSonde->Get_pTimer2();
    pTimer2ChoixSonde->start();

}

//-------------------------------------------------------------------------------

/**
 * Ce slot va permettre de récupérer les informations contenues dans un fichier d'extention
 * ".gexao51" et utiliser celles-ci pour configurer les scènes de plan de câblage.
 */
void F_Principale::on_FenetreCharger()
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
        QTimer *pTimerChoixSonde = new QTimer;
        pTimerChoixSonde = this->oMonChoixSonde->Get_pTimer();
        pTimerChoixSonde->start();

        this->oMonChoixSonde->on_ActionSupprimer();

        QString NombreCapteurJackLus("");
        unsigned int NombreCapteurJack(0);

        NombreCapteurJackLus = MonFlux.readLine();
        NombreCapteurJackLus.remove(0,18);
        NombreCapteurJack = NombreCapteurJackLus.toInt();

        for(unsigned int i=0; i<NombreCapteurJack; i++)
        {
            QString sNomCapteur;
            QString sNomBroche;

            sNomCapteur = MonFlux.readLine();
            sNomCapteur.remove(0,11);

            sNomBroche = MonFlux.readLine();
            sNomBroche.remove(0, 10);

            QList<ScenePlanDeCablage*> lstScenes;
            lstScenes = this->oMonChoixSonde->Get_lstScenes();

            QList<ItemBroche *> lstListeBroche;
            lstListeBroche = lstScenes[0]->Get_lstListeBroche();

            lstListeBroche[sNomBroche.toInt()]->DefinirCapteur(sNomCapteur, sNomBroche);
        }
        
        this->oMonChoixSonde->on_ComboBoxInterface(0);
    }
}

//-------------------------------------------------------------------------------

/**
  * Ce slot va permettre de sauvegarder dans un fichier d'extention ".gexao51" les paramètres
  * nécéssaires à la création d'un objet capteur, soit le nom du capteur et son nom de broche.
  */
void F_Principale::on_FenetreSauvegarder()
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

    QList<Capteur*> lstListeCapteurAnalogiqueChoixSonde;
    lstListeCapteurAnalogiqueChoixSonde = this->oMonChoixSonde->Get_lstListeCapteurAnalogique();

    this->lstListeCapteurAnalogique = lstListeCapteurAnalogiqueChoixSonde;

    MonFlux << "NombreCapteurJack= " << this->lstListeCapteurAnalogique.size()  << "\n";

    // Ecriture de chaque élement de la liste de lignes dans le fichier
    for (unsigned int x=0; x<this->lstListeCapteurAnalogique.size(); x++)
    {
        MonFlux << "NomCapteur=";

        QString sNomCapteur;
        sNomCapteur = this->lstListeCapteurAnalogique[x]->Get_sNomCapteur();

        for (unsigned int y=0; y<sNomCapteur.size(); y++)
        {
            MonFlux << sNomCapteur[y];
        }

        MonFlux << "\n";

        MonFlux << "NomBroche=";

        QString sNomBroche;
        sNomBroche = this->lstListeCapteurAnalogique[x]->Get_sNomBroche();

        for(unsigned int y=0; y<sNomBroche.size(); y++)
        {
            MonFlux << sNomBroche[y];
        }

        MonFlux << "\n";
    }
}

//-------------------------------------------------------------------------------

/**
  * Ce slot permet grâce à un connect de mettre à jour l'attribut lstListeCapteurComplete.
  * De plus, celà permettra d'actualiser la liste des capteurs nécéssitant un calibrage
  * dans la combobox de la fenêtre F_ListeCalibrage.
  */
void F_Principale::on_MiseAJourQList()
{
    this->lstListeCapteurComplete = this->oMonChoixSonde->Get_lstListeCapteurComplete();


    this->oMaListeCalibrage->Set_lstListeCapteurComboBox(this->lstListeCapteurComplete);

}

void F_Principale::on_LancerPartout()
{
    for(unsigned int i=0;i<this->lstListeSonde.length();i++)
    {
        this->lstListeSonde[i]->on_Bt_Lancer_clicked();
    }
}

void F_Principale::on_StopperPartout()
{
    for(unsigned int i=0;i<this->lstListeSonde.length();i++)
    {
        this->lstListeSonde[i]->StopperAffichage();
    }
}

void F_Principale::on_SauvegarderPartout()
{
    for(unsigned int i=0;i<this->lstListeSonde.length();i++)
    {
        this->lstListeSonde[i]->on_Bt_Enregistrement_clicked();
    }
}

void F_Principale::on_StopperSauvegardePartout()
{
    for(unsigned int i=0;i<this->lstListeSonde.length();i++)
    {
        this->lstListeSonde[i]->on_Bt_Lancer_clicked();
    }
}

//-------------------------------------------------------------------------------

//Affiche la même heure et date partout sur la base de la première fenêtre
void F_Principale::on_ActionHeureDatePartout()
{
    /*for(unsigned int i=0;i<this->lstListeSonde.length();i++)
    {
        this->lstListeSonde[i]->
    }*/
}

void F_Principale::on_ParametresPartout()
{
    /*for(unsigned int i=0;i<this->lstListeSonde.length();i++)
    {
        this->lstListeSonde[i]->on_Bt_Lancer_clicked();
    }*/
}

//-------------------------------------------------------------------------------

/**
  * Ce slot va permettre d'afficher la fenêtre F_ListeCalibrage.
  */
void F_Principale::on_FenetreListeCalibrage()
{
    this->oMaListeCalibrage->show();
}
//-------------------------------------------------------------------------------
