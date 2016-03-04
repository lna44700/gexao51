#include "F_Principale.h"
#include "ui_F_Principale.h"
#include <F_Sonde.h>
#include "fenetreenfant.h"
#include "F_ChoisirSonde.h"
#include <QtGui>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>

F_Principale::F_Principale(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::F_Principale)
{
    ui->setupUi(this);
}

F_Principale::~F_Principale()
{
    delete ui;
}

void F_Principale::displayAbout() //action qui affiche un message en pop-up
{
    QMessageBox::about(this, tr("À propos"),
    tr("<p>Ce logiciel a été développé par Sylvain GUICHARD et Sylvain MENARD, étudiants au Lycée Nicolas Appert à Orvault.</p>")); // le message qui doit s'afficher
}

void F_Principale::createChild() // création de la feêtre enfant
{
    F_Sonde *f_sonde = new F_Sonde(ui->mdiArea); // création d'une variable de type f_sonde
    f_sonde->setAttribute(Qt::WA_DeleteOnClose); //detruit le widget lors de la fermeture de l'évenement
    ui->mdiArea->addSubWindow(f_sonde); // ajoute la fenêtre enfant f_sonde à la fenêtre MDI
    f_sonde->show(); // affiche la fenêtre enfant
}

void F_Principale::displaySelection()
{

    F_ChoisirSonde *f_choix = new F_ChoisirSonde ;
    f_choix->show();
}
