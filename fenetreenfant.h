#ifndef FENETREENFANT_H
#define FENETREENFANT_H

#include <QtGui>
#include <QWidget>
#include <QMdiSubWindow>
#include <F_Sonde.h>


namespace Ui {
    class FenetreEnfant;
}

class FenetreEnfant : public QMdiSubWindow
{
    Q_OBJECT

public:
    explicit FenetreEnfant(QWidget *parent = 0);
    ~FenetreEnfant();

private:
    F_Sonde *f_sonde;
};

#endif // FENETREENFANT_H
