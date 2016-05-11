//-------------------------------------------------------------------------------
/**
 * @file        Photo.cpp
 * @brief       Gestion de la photo des interfaces
 *
 * @author      S.GUICHARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        27/04/16
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "Photo.h"

//=====   En-Têtes standards    =====
#include <QPainter>

//-------------------------------------------------------------------------------

/**
 * Constructeur paramétré
 * @param sChemin QString contenant le chemin permettant de récupérer la photo
 * de l'interface
 */
Photo::Photo(QString sChemin, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    sChemin(sChemin)
{

}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va afficher la photo de l'interface sur la fenêtre F_ChoisirSonde
 */
void Photo::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->drawPixmap(0, 0, QPixmap(this->sChemin + "/photo.png"));
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode définit la taille de la photo de l'interface
 */
QRectF Photo::boundingRect() const
{
    return QRectF(0, 0, 621, 485);
}
//-------------------------------------------------------------------------------
