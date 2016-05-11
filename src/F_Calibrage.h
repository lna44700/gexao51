#ifndef F_CALIBRAGE_H
#define F_CALIBRAGE_H

#include <QWidget>
#include "Arduino.h"

namespace Ui {
class F_Calibrage;
}

class F_Calibrage : public QWidget
{
    Q_OBJECT

public:
    explicit F_Calibrage(Arduino *oMonArduino, QWidget *parent = 0);
    ~F_Calibrage();

    void                ResolutionEquation();

    float               Get_fA();
    float               Get_fB();

    void                Set_sNomBroche(QString sNomBroche);
    void                Set_sCommande(QString sCommande);


private:
    Ui::F_Calibrage     *ui;
    Arduino             *oMonArduino;
    QString             sNomBroche;
    QString             sCommande;
    float               fRetour1;
    float               fRetour2;
    float               fValeur1;
    float               fValeur2;
    float               fA;
    float               fB;

private slots:
    void                on_MesurerValeur1();
    void                on_MesurerValeur2();
    void                on_Fermer();
};

#endif // F_CALIBRAGE_H
