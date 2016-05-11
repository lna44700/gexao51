//-------------------------------------------------------------------------------
/**
 * @file        F_ListeCalibrage.cpp
 * @brief       Création de la fenêtre qui contiendra la liste des fenêtres F_Calibrage
 * afin de les ouvrir lorsqu'elles sont fermées
 *
 * @author      S.GUICHARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        27/04/16
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "F_ListeCalibrage.h"
#include "ui_F_ListeCalibrage.h"

//=====   En-Têtes standards    =====
#include <QDebug>

//-------------------------------------------------------------------------------

/**
 * Constructeur par défaut
 */
F_ListeCalibrage::F_ListeCalibrage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_ListeCalibrage)
{
    ui->setupUi(this);

    setWindowTitle("Liste des capteurs à calibrer");

    //Ajout de l'icone de l'application
    QIcon icone(":/new/prefix1/images/icones/icone_g_key.ico");
    QWidget::setWindowIcon(icone);

    this->setWindowModality(Qt::ApplicationModal);

}

//-------------------------------------------------------------------------------

/**
 * Destructeur
 */
F_ListeCalibrage::~F_ListeCalibrage()
{
    delete ui;
}

//-------------------------------------------------------------------------------

/**
 * Ce slot permet d'afficher la fenêtre de calibrage correspondant à l'objet capteur
 * affiché dans la combobox.
 */
void F_ListeCalibrage::on_Ouvrir()
{
    int Index(0);

    Index = this->ui->CBx_Calibrage->currentIndex();

    F_Calibrage *oMonCalibrage = new F_Calibrage(NULL);
    oMonCalibrage = this->lstListeCapteurComboBox[Index]->Get_oMonCalibrage();

    oMonCalibrage->show();
}

//-------------------------------------------------------------------------------

/**
 * Mutateur permettant de remplir la combobox avec la liste des capteurs
 * qui nécéssite un calibrage/
 * @param lstListeCapteurComboBox QList<Capteur*> contient la liste de tous les capteurs I2C
 * et Jack
 */
void F_ListeCalibrage::Set_lstListeCapteurComboBox(QList<Capteur*> lstListeCapteurComboBox)
{

    if(this->isVisible() == false)

    {
        this->ui->CBx_Calibrage->clear();
        this->lstListeCapteurComboBox.clear();

        for(unsigned int i=0; i < lstListeCapteurComboBox.length(); i++)
        {
            bool bCalibrageNecessaire;
            bCalibrageNecessaire = lstListeCapteurComboBox[i]->Get_bCalibrageNecessaire();

            if(bCalibrageNecessaire == true)
            {
                this->lstListeCapteurComboBox.push_back(lstListeCapteurComboBox[i]);
            }
        }

        for(unsigned int i=0; i<this->lstListeCapteurComboBox.length(); i++ )
        {
            QString sTitre;
            sTitre = this->lstListeCapteurComboBox[i]->Get_sTitre();

            QString sNomBroche;
            sNomBroche = this->lstListeCapteurComboBox[i]->Get_sNomBroche();

            if(!sTitre.isEmpty())
            {
                this->ui->CBx_Calibrage->addItem(sTitre + " - Adresse : " + sNomBroche);
            }

        }
    }
}
//-------------------------------------------------------------------------------
