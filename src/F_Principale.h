#ifndef F_PRINCIPALE_H
#define F_PRINCIPALE_H

#include <QMainWindow>
#include <QMdiArea>
#include "Arduino.h"
#include <QSettings>
#include <QLabel>
#include "ConfigurationMaquette/Capteur.h"
#include "F_ChoisirSonde.h"
#include "F_SelectionSondes.h"
#include "F_Sonde.h"
#include "F_ListeCalibrage.h"

namespace Ui {
class F_Principale;
}

class F_Principale : public QMainWindow
{
    Q_OBJECT

public:
    explicit F_Principale(QWidget *parent = 0);
    ~F_Principale();

private:
    Ui::F_Principale    *ui;
    Arduino             *oMonArduino;
    F_ChoisirSonde      *oMonChoixSonde;
    F_ListeCalibrage    *oMaListeCalibrage;
    F_Sonde             *oMaSonde;
    QList<Capteur*>     lstListeCapteurAnalogique;
    QList<Capteur*>     lstListeCapteurComplete;
    QTimer              *pTimer;
    QTimer              *pTimer2;
    QLabel              *pLblIcon;
    QLabel              *pLblTexte;
    QPixmap             pxmIconeVert;
    QPixmap             pxmIconeRouge;
    int                 nDonneesLues;
    bool                bEstDetecte;
    bool                bEtatPrecedent;
    QList<F_Sonde*>     lstListeSonde;

private slots:
    void on_ActionHeureDatePartout();
    void on_FenetreAPropos();
    void on_FenetreAcquisition();
    void on_FenetreChoixSonde();
    void on_FenetreCharger();
    void on_FenetreSauvegarder();
    void on_FenetreListeCalibrage();
    void on_DetecterArduino();
    void on_MiseAJourQList();
    void on_LancerPartout();
    void on_StopperPartout();
    void on_SauvegarderPartout();
    void on_StopperSauvegardePartout();
    void on_ParametresPartout();
};

#endif // F_PRINCIPALE_H
