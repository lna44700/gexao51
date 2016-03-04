#ifndef ENFANTSONDE_H
#define ENFANTSONDE_H

#include <QtGui>
#include <QWidget>
#include <QMdiSubWindow>
#include <F_Sonde.h>
#include "EnfantSonde.h"


namespace Ui {
    class EnfantSonde;
}

class EnfantSonde : public QMdiSubWindow
{
    Q_OBJECT

public:
    explicit EnfantSonde(QWidget *parent = 0);
    ~EnfantSonde();

private:
    F_Sonde *f_sonde;
};

#endif // ENFANTSONDE_H
