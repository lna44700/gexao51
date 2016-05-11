#ifndef PHOTO_H
#define PHOTO_H

class Photo;

#include <QGraphicsItem>

class Photo : public QGraphicsItem
{
public:
    Photo(QString sChemin, QGraphicsItem * parent = 0); //Constructeur

    void            paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/); //Dessine
    QRectF          boundingRect() const;    //Rectangle l'entourant

private:
    QString         sChemin; //Chemin vers la photo
};

#endif // PHOTO_H
