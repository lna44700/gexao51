#include "F_Sonde.h"
#include "ui_F_Sonde.h"

F_Sonde::F_Sonde(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_Sonde)
{
    ui->setupUi(this);
}

F_Sonde::~F_Sonde()
{
    delete ui;
}
