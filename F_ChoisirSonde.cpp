#include "F_ChoisirSonde.h"
#include "ui_F_ChoisirSonde.h"
#include <QDebug>

F_ChoisirSonde::F_ChoisirSonde(Arduino *oMonArduino, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_ChoisirSonde)
{
    ui->setupUi(this);

    int test;
    test = oMonArduino->LireCapteur("iR01602");
    qDebug() << test;
}

F_ChoisirSonde::~F_ChoisirSonde()
{
    delete ui;
}
