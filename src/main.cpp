#include "F_Principale.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);
        QTranslator translator;
        translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        a.installTranslator(&translator);

    F_Principale w;

    w.show/*Maximized*/();

    return a.exec();
}
