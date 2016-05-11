#ifndef F_SONDE_H
#define F_SONDE_H

#include "Arduino.h"
#include "ConfigurationMaquette/Capteur.h"

#include <QWidget>
#include <QStandardItem>
#include <QTimer>
#include <fstream>
#include <iostream>
#include <unistd.h>

using namespace std;

namespace Ui {
class F_Sonde;
}

class F_Sonde :public QWidget
{
    Q_OBJECT

public:
    explicit F_Sonde(Arduino *oMonArduino, Capteur* oLeCapteur, QWidget *parent = 0);
    ~F_Sonde();



public slots:
    void        on_Bt_ModeAcquisition_clicked();

    void        on_Bt_Lancer_clicked();

    void        on_Bt_Enregistrement_clicked();

    void        Affichage();

    void        Sauvegarde();

    void        StopperAffichage();

    void        StopperSauvegarde();



private:
    Ui::F_Sonde *ui;
    Arduino *oArduino;
    QStandardItemModel *model = new QStandardItemModel;
    QTimer *tmMinuteurIntervalleMesuresAffichage;
    QTimer *tmMinuteurIntervalleMesuresSauvegarde;
    int nDureeMesure;
    int nDureeTotale;
    int nIntervalle;
    bool bEtatLancer;
    bool bEtatRecup;
    unsigned int nCompteurSauvegarde;
    unsigned int nCompteurAffichage;
    ofstream fpFichierDonnees;
    QSemaphore *Semaphore;
    void Expression();

    Capteur* oLeCapteur;

};

#endif // F_SONDE_H
