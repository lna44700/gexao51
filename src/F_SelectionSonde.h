#ifndef F_SELECTIONSONDES_H
#define F_SELECTIONSONDES_H

#include <QWidget>

namespace Ui {
class F_SelectionSonde;
}

class F_SelectionSonde : public QWidget
{
    Q_OBJECT

public:
    explicit F_SelectionSonde(QWidget *parent = 0);
    ~F_SelectionSonde();

private:
    Ui::F_SelectionSonde *ui;
};

#endif // F_SELECTIONSONDES_H
