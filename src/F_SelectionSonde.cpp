#include "F_SelectionSonde.h"
#include "ui_selectionsondes.h"
#include <QListWidgetItem>

F_SelectionSonde::F_SelectionSonde(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_SelectionSonde)
{
    ui->setupUi(this);

    QListWidgetItem* item = new QListWidgetItem("test", ui->LW_ListeSondes);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
    item->setCheckState(Qt::Unchecked); // AND initialize check state
}

F_SelectionSonde::~F_SelectionSonde()
{
    delete ui;
}
