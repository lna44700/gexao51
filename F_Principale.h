#ifndef F_PRINCIPALE_H
#define F_PRINCIPALE_H

#include <QMainWindow>
#include <QMdiArea>
#include "Arduino.h"

namespace Ui {
class F_Principale;
}

class F_Principale : public QMainWindow
{
    Q_OBJECT

public:
    explicit F_Principale(QWidget *parent = 0);
    ~F_Principale();
    Arduino *oMonArduino;

private:
    Ui::F_Principale *ui;
    int DonneesLues;



public slots:
    void displayAbout();
    void createChild();
    void displaySelection();

private slots:
    void on_actionHeureDatePartout_triggered();
    void on_actionOuvrir_triggered();
    void on_actionSauvegarder_triggered();
};

#endif // F_PRINCIPALE_H
