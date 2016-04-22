#ifndef ITEMBROCHE_H
#define ITEMBROCHE_H

class ItemBroche;

#define TAILLE_ITEM (40)
#include <QGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QSettings>
#include "Capteur.h"

typedef enum {
    ACCEPT,
    REFUSE,
    NORMAL
} TypeAccept;

class ItemBroche : public QGraphicsItem
{

public:

    ItemBroche(QGraphicsItem* parent = 0); //Constructeur

    void            paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/); //DEssine l'objet
    void            setNom(QString Nom);                    //|
    void            DefinirCapteur(QString NomCapteur, QString NomBroche); //Defninir le capteur
    QRectF          boundingRect() const; //Rectangle entourant l'objet
    Capteur         *CapConnect;  //Capteur connecté à cette broche
    TypeAccept      AcceptDrag;



protected:

    void            dragEnterEvent(QGraphicsSceneDragDropEvent *event);     //Evenement d'entrée d'un drag dans l'item
    void            dragLeaveEvent(QGraphicsSceneDragDropEvent *event);     //Evenement de sortie d'un drag
    void            dropEvent(QGraphicsSceneDragDropEvent *event);          //Evenement de dépot
    void            mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);//Evenement de double click
    void            contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

private:

    QString         NomBroche;

};

#endif // ITEMBROCHE_H
