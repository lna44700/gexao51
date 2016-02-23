#include "f_principale.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    F_Principale w;
    w.show();

    return a.exec();
}
