#include "F_SelectionSondes.h"
#include "ui_F_SelectionSondes.h"
#include <QListWidgetItem>

F_SelectionSondes::F_SelectionSondes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::F_SelectionSondes)
{
    ui->setupUi(this);

    QListWidgetItem* item = new QListWidgetItem("item", ui->LW_ListeSondes);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
    item->setCheckState(Qt::Unchecked); // AND initialize check state
}

F_SelectionSondes::~F_SelectionSondes()
{
    delete ui;
}
