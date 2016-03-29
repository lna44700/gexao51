#ifndef F_SONDE_H
#define F_SONDE_H

#include <QWidget>
#include "Arduino.h"
#include <QStandardItem>
#include <QTimer>

namespace Ui {
class F_Sonde;
}

class F_Sonde :public QWidget
{
    Q_OBJECT

public:
    explicit F_Sonde(Arduino *oMonArduino, QWidget *parent = 0);
    ~F_Sonde();

private slots:
    void on_Bt_ModeAcquisition_clicked();

    void on_Bt_Lancer_clicked();

    void on_Bt_Enregistrement_clicked();

private:
    Ui::F_Sonde *ui;
    Arduino *oArduino;
    QStandardItemModel *model = new QStandardItemModel;
    QTimer MinuteurIntervalleMesures;

};

#endif // F_SONDE_H
