#ifndef F_CHOISIRSONDE_H
#define F_CHOISIRSONDE_H

#include <QDialog>
#include "Arduino.h"
#include <QSettings>
#include <QStandardItemModel>
#include "F_Calibrage.h"
#include <QTimer>

#include "ConfigurationMaquette/ListeCapteurVue.h"
#include "ConfigurationMaquette/ScenePlanDeCablage.h"


namespace Ui {
class F_ChoisirSonde ;
}

class F_ChoisirSonde : public QDialog
{
    Q_OBJECT

public:
    explicit F_ChoisirSonde(Arduino *oMonArduino/*, QSettings* Config, QStringList ListeInterfaceInstall*/, QWidget *parent = 0) ;   // Constructeur
    ~F_ChoisirSonde();

    void                            ScannerAdresseI2C();

    QTimer                          *Get_pTimer();
    QTimer                          *Get_pTimer2();
    QList<Capteur*>                 Get_lstListeCapteurAnalogique();
    QList<Capteur *>                Get_lstListeCapteurComplete();
    QList<ScenePlanDeCablage *>     Get_lstScenes();

public slots:
    void                            on_ComboBoxInterface(int nIndex);
    void                            on_ActionSupprimer();
    void                            on_ActionValider();

private slots:
    void                            on_MiseAJourQList();
    void                            on_AfficherCalibrage();

private:
    Ui::F_ChoisirSonde              *ui ;                   // Interface graphique
    Arduino                         *oMonArduino;
    F_Calibrage                     *oMonCalibrage;
    ListeCapteurVue                 *oMaListeCapteurVue;               //Liste des capteur
    unsigned int                    wIndexPrecedent;
    QStringList                     slstListeInterface;
    QList<Capteur*>                 lstListeCapteurComplete;
    QList<QStandardItemModel*>      lstModeles;                 //Liste des modeles listant les capteur
    QTimer                          *pTimer;
    QTimer                          *pTimer2;
    QList<Capteur*>                 lstListeCapteurI2C;
    QList<Capteur*>                 lstListeCapteurAnalogique;
    QList<ScenePlanDeCablage*>      lstScenes;     //Liste des scene affichant le plan

};

#endif // F_CHOISIRSONDE_H
