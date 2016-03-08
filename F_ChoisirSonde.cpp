#include "F_ChoisirSonde.h"
#include "ui_F_ChoisirSonde.h"

F_ChoisirSonde::F_ChoisirSonde(Arduino *oMonArduino, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_ChoisirSonde)
{
    ui->setupUi(this);
}

F_ChoisirSonde::~F_ChoisirSonde()
{
    delete ui;
}
