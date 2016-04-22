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

        explicit F_ChoisirSonde(Arduino *oMonArduino/*, QSettings* Config, QStringList ListeInterfaceInstall*/, QWidget *parent = 0) ;   // Constructeur
        ~F_ChoisirSonde();

        QList<Capteur*>                 ListeCapteurI2C;
        QList<Capteur*>                 ListeCapteurAnalogique;
        QList<ScenePlanDeCablage*>      Scenes;     //Liste des scene affichant le plan


public slots:

        void on_CB_Interface_currentIndexChanged(int index);
        void on_Btn_Supprimer_clicked();
        void on_Btn_Valider_clicked();

private:

        Ui::F_ChoisirSonde          *ui ;                   // Interface graphique
        unsigned int                indexPrecedent;
        ListeCapteurVue*            ListeCap;               //Liste des capteur
        QStringList                 ListeInterfaceInstall;
        QList<QStandardItemModel*>  Models;                 //Liste des modeles listant les capteur

};

#endif // F_CHOISIRSONDE_H
