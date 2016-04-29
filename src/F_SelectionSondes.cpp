#include "F_SelectionSondes.h"
#include "ui_F_SelectionSondes.h"
#include <QListWidgetItem>

F_SelectionSondes::F_SelectionSondes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_SelectionSondes)
{
    ui->setupUi(this);

    QListWidgetItem *Sonde = new QListWidgetItem("Test", ui->LW_ListeSondes);
    Sonde->setFlags(Sonde->flags() | Qt::ItemIsUserCheckable); // set checkable flag
    Sonde->setCheckState(Qt::Unchecked); // AND initialize check state
}

F_SelectionSondes::~F_SelectionSondes()
{
    delete ui;
}
