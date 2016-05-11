#ifndef F_LISTECALIBRAGE_H
#define F_LISTECALIBRAGE_H

#include <QWidget>
#include "ConfigurationMaquette/Capteur.h"

namespace Ui {
class F_ListeCalibrage;
}

class F_ListeCalibrage : public QWidget
{
    Q_OBJECT

public:
    explicit F_ListeCalibrage(QWidget *parent = 0);
    ~F_ListeCalibrage();

    void        Set_lstListeCapteurComboBox(QList<Capteur*> lstListeCapteurComboBox);

private slots:
    void        on_Ouvrir();


private:
    Ui::F_ListeCalibrage *ui;
    QList<Capteur*> lstListeCapteurComboBox;
};

#endif // F_LISTECALIBRAGE_H
