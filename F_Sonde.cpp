#include "F_Sonde.h"
#include "ui_F_Sonde.h"
#include <QDate>
#include <QString>
#include <QTime>

F_Sonde::F_Sonde(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_Sonde)
{
    ui->setupUi(this);

    QDate date = QDate::currentDate();
    ui->deDateAcquisition->setDate(date);

    QTime heure = QTime::currentTime();
    ui->tiHeureAcquisition->setTime(heure);

    ui->leTpsAcquisition->setPlaceholderText("en minutes");
    ui->leIntervalle->setPlaceholderText("en secondes");
}

F_Sonde::~F_Sonde()
{
    delete ui;
}

void F_Sonde::on_btnModeAcquisition_clicked()
{
    QString sIndication("en minutes");
    if (ui->btnModeAcquisition->text() == "Automatique")
    {
        ui->btnModeAcquisition->setText("Manuel");
        ui->tlTpsAcquisition->hide();
        ui->leTpsAcquisition->hide();
        ui->tlIntervalle->hide();
        ui->leIntervalle->hide();
        ui->tlDateHeure->hide();
        ui->deDateAcquisition->hide();
        ui->tiHeureAcquisition->hide();

    }
    else if (ui->btnModeAcquisition->text() == "Manuel")
    {
        ui->btnModeAcquisition->setText("Automatique");
        ui->tlTpsAcquisition->show();
        ui->leTpsAcquisition->show();
        ui->tlIntervalle->show();
        ui->leIntervalle->show();
        ui->tlDateHeure->show();
        ui->deDateAcquisition->show();
        ui->tiHeureAcquisition->show();
    }
}

void F_Sonde::on_cbModeVisualisation_toggled(bool checked)
{
    if (checked == true)
    {
        // desactiver le tableau
    }
}
