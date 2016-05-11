//-------------------------------------------------------------------------------
/**
 * @file        F_Calibrage.cpp
 * @brief       Création de la fenêtre permettant le calibrage
 *
 * @author      S.GUICHARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        27/04/16
 *
 *La fenêtre de calibrage s'ouvre automatiquement lors d'un Drag & Drop d'un capteur.
 *Lors d'un appui sur le bouton mesurer la valeur analogique est alors lue et stockée
 *dans une variable. Deux mesures sont à éffectuer pour que la sonde soit calibrée.
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "F_Calibrage.h"
#include "ui_F_Calibrage.h"

//=====   En-Têtes standards    =====
#include <QDebug>
#include <unistd.h>

//-------------------------------------------------------------------------------

/**
 * Constructeur paramétré
 * @param *oMonArduino Pointeur de l'objet Arduino créé dans le fichier d'application F_Principale
 */
F_Calibrage::F_Calibrage(Arduino *oMonArduino, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_Calibrage),
    oMonArduino(oMonArduino)
{
    ui->setupUi(this);

    //Ajout de l'icone de l'application
    QIcon icone(":/new/prefix1/images/icones/icone_g_key.ico");
    QWidget::setWindowIcon(icone);

    this->setWindowModality(Qt::ApplicationModal);
}

//-------------------------------------------------------------------------------

/**
 * Destructeur
 */
F_Calibrage::~F_Calibrage()
{
    delete ui;
}

//-------------------------------------------------------------------------------

/**
 * Ce slot va mesurer la premiere valeur en envoyant au ShellMéga la commande nécéssaire,
 * puis va lire sur le port la valeur renvoyée. Enfin le résultat sera affiché.
 */
void F_Calibrage::on_MesurerValeur1()
{
    this->fValeur1 = this->ui->SBx_Valeur1->value();

    QString sCommandeAEcrire;

    if(this->sCommande == "A")
    {
        sCommandeAEcrire = this->sCommande+ this->sNomBroche;
    }
    if(this->sCommande == "iR")
    {
        sCommandeAEcrire = "iR00" + this->sNomBroche + "02";

        if(this->sNomBroche.toInt() > 9)
        {
            sCommandeAEcrire.remove(2,1);
        }
        if(this->sNomBroche.toInt() > 99)
        {
            sCommandeAEcrire.remove(2,1);
        }
    }

        this->fRetour1 = this->oMonArduino->LireCapteur(sCommandeAEcrire);
        this->ui->Lcd_Valeur1->display(this->fRetour1);

        //this->ResolutionEquation();
}

//-------------------------------------------------------------------------------

/**
 * Ce slot va mesurer la deuxieme valeur en envoyant au ShellMéga la commande nécéssaire,
 * puis va lire sur le port la valeur renvoyée. Enfin le résultat sera affiché.
 */
void F_Calibrage::on_MesurerValeur2()
{
    this->fValeur2 = this->ui->SBx_Valeur2->value();

    QString sCommandeAEcrire;

    if(this->sCommande == "A")
    {
        sCommandeAEcrire = this->sCommande+this->sNomBroche;
    }
    if(this->sCommande == "iR")
    {
        sCommandeAEcrire = "iR00" + this->sNomBroche + "02";

        if(this->sNomBroche.toInt() > 9)
        {
            sCommandeAEcrire.remove(2,1);
        }
        if(this->sNomBroche.toInt() > 99)
        {
            sCommandeAEcrire.remove(2,1);
        }
    }

        this->fRetour2 = this->oMonArduino->LireCapteur(sCommandeAEcrire);
        this->ui->Lcd_Valeur2->display(this->fRetour2);

}

//-------------------------------------------------------------------------------

/**
 * Ce slot va fermer la fenêtre de calibrage.
 */
void F_Calibrage::on_Fermer()
{
    this->ResolutionEquation();

    this->close();
}

//-------------------------------------------------------------------------------

/**
 * Ce slot va calculer les variables fA et fB afin de les utiliser lors de l'acquisition.
 */
void F_Calibrage::ResolutionEquation()
{
    this->fA =0,0;
    this->fB = 0,0;
    this->fA = (this->fValeur2 - this->fValeur1)/(this->fRetour2 - this->fRetour1);
    this->fB = this->fValeur1 - (((this->fValeur2-this->fValeur1)*this->fRetour1)/ (this->fRetour2 - this->fRetour1));

}

//-------------------------------------------------------------------------------

/**
 * Mutateur du début de la commande à envoyer au ShellMéga
 * @param sCommande QString contenant "A" pour les capteurs Jack et "iR" pour les capteurs
 * I2C
 */
void F_Calibrage::Set_sCommande(QString sCommande)
{
    this->sCommande = sCommande;
}

//-------------------------------------------------------------------------------

/**
 * Mutateur de la fin de commande à envoyer au ShellMéga
 * @param sNomBroche QString contenant le numéro de la broche sur laquelle est connecté
 * le capteur.
 */
void F_Calibrage::Set_sNomBroche(QString sNomBroche)
{
    this->sNomBroche = sNomBroche;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de la première valeur du calibrage
 * @return fA float
 */
float F_Calibrage::Get_fA()
{
    return this->fA;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur de la deuxième valeur du calibrage
 * @return fB float
 */
float F_Calibrage::Get_fB()
{
    return this->fB;
}
//-------------------------------------------------------------------------------
