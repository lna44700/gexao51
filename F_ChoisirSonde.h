#ifndef F_CHOISIRSONDE_H
#define F_CHOISIRSONDE_H

#include <QDialog>
#include "Arduino.h"
#include <QSettings>
#include <QStandardItemModel>

#include "ConfigurationMaquette/ListeCapteurVue.h"
#include "ConfigurationMaquette/ScenePlanDeCablage.h"

namespace Ui {
class F_ChoisirSonde ;
}

class F_ChoisirSonde : public QDialog
{
    Q_OBJECT

    public:
            explicit F_ChoisirSonde(Arduino *oMonArduino, QSettings* Config, QStringList ListeInterfaceInstall, QWidget *parent = 0) ;   // Constructeur
            ~F_ChoisirSonde() ;                              // Destructeur

private slots:
    void on_CBx_Interface_currentIndexChanged(int index);

    void on_Bt_Annuler_clicked();

    void on_Bt_Valider_clicked();

private:
            Ui::F_ChoisirSonde * ui ;        // Interface graphique
            ListeCapteurVue*            ListeCap;   //Liste des capteur

            QSettings*                  Config;     //Pointeur vers la config

            QList<QStandardItemModel*>  Models;     //Liste des modeles listant les capteur
            QList<ScenePlanDeCablage*>  Scenes;     //Liste des scene affichant le plan
            QList<Capteur*>   ListeI2C;
            QList<Capteur*>   ListeCapteur;


};

#endif // F_CHOISIRSONDE_H
