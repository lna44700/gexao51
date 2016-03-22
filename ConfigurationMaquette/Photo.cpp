
#include <QPainter>
#include "Photo.h"

Photo::Photo(QString Chemin, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    Chemin(Chemin)
{
}

void Photo::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->drawPixmap(0, 0, QPixmap(this->Chemin + "/photo.png"));
}

QRectF Photo::boundingRect() const
{
    return QRectF(0, 0, 621, 485);
}
