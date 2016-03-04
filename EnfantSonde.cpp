#include "EnfantSonde.h"
#include <QMdiSubWindow>
#include "F_Sonde.h"
#include <QSize>

EnfantSonde::EnfantSonde(QWidget *parent) :
  QMdiSubWindow(parent)
{
    f_sonde = new F_Sonde(this);
    this->setWidget(f_sonde);
}

EnfantSonde::~EnfantSonde()
{
    if(f_sonde != NULL)
    {
        delete f_sonde;
        f_sonde = NULL;
    }

}
