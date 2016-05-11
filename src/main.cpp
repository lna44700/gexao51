//-------------------------------------------------------------------------------
/**
 * @file        main.cpp
 * @brief       Création de l'objet F_Principale et affichage en plein écran de la fenêtre
 *
 * @author      S.GUICHARD
 * @author      S.MENARD
 *
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        27/04/16
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "F_Principale.h"

//=====   En-Têtes standards    =====
#include <QApplication>

//-------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    F_Principale oMaFenetre;
    oMaFenetre.showMaximized();

    return a.exec();
}
//-------------------------------------------------------------------------------
