#include "f_principale.h"
#include "ui_f_principale.h"

F_Principale::F_Principale(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::F_Principale)
{
    ui->setupUi(this);
}

F_Principale::~F_Principale()
{
    delete ui;
}
