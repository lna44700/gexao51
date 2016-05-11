#ifndef F_SELECTIONSONDES_H
#define F_SELECTIONSONDES_H

#include <QWidget>

namespace Ui {
class F_SelectionSondes;
}

class F_SelectionSondes : public QWidget
{
    Q_OBJECT

public:
    explicit F_SelectionSondes(QWidget *parent = 0);
    ~F_SelectionSondes();

private:
    Ui::F_SelectionSondes *ui;
};

#endif // F_SELECTIONSONDES_H
