#include "F_Principale.h"
#include <QApplication>
#include <QMdiArea>
#include "F_Sonde.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    F_Principale w;
    w.show();




    return a.exec();
}
