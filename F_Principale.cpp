#include "F_Principale.h"
#include "ui_F_Principale.h"

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
