#include "F_Principale.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    F_Principale w;
    w.show();

    int hey;
    return a.exec();
}
