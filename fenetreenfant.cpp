#include "fenetreenfant.h"
#include <QMdiSubWindow>
#include "F_Sonde.h"

FenetreEnfant::FenetreEnfant(QWidget *parent) :
  QMdiSubWindow(parent)
{
    f_sonde = new F_Sonde(this);
    this->setWidget(f_sonde);
}

FenetreEnfant::~FenetreEnfant()
{
    f_sonde->~F_Sonde();
}
