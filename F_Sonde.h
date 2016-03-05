#ifndef F_SONDE_H
#define F_SONDE_H

#include <QWidget>

namespace Ui {
class F_Sonde;
}

class F_Sonde : public QWidget
{
    Q_OBJECT

public:
    explicit F_Sonde(QWidget *parent = 0);
    ~F_Sonde();

private slots:
    void on_btnModeAcquisition_clicked();

    void on_cbModeVisualisation_toggled(bool checked);

private:
    Ui::F_Sonde *ui;
};

#endif // F_SONDE_H
