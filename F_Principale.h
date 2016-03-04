#ifndef F_PRINCIPALE_H
#define F_PRINCIPALE_H

#include <QMainWindow>
#include <QMdiArea>

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
    Ui::F_Principale *ui;

public slots:
    void displayAbout();
    void createChild();
    void displaySelection();
};

#endif // F_PRINCIPALE_H
