#ifndef F_CHOISIRSONDE_H
#define F_CHOISIRSONDE_H

#include <QWidget>
#include <Arduino.h>

namespace Ui {
class F_ChoisirSonde;
}

class F_ChoisirSonde : public QWidget
{
    Q_OBJECT

public:
    explicit F_ChoisirSonde(Arduino *oMonArduino, QWidget *parent = 0);
    ~F_ChoisirSonde();

private:
    Ui::F_ChoisirSonde *ui;
};

#endif // F_CHOISIRSONDE_H
