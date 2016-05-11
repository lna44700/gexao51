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
    ItemBroche(Arduino *oMonArduino, QGraphicsItem* parent = 0); //Constructeur

    void            paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/); //DEssine l'objet
    QRectF          boundingRect() const; //Rectangle entourant l'objet
    void            DefinirCapteur(QString sNomCapteur, QString sNomBroche); //Defninir le capteur

    Capteur         *Get_pCapteurConnecte();

    void            Set_sNomBroche(QString sNomBroche);                    //|
    void            *Set_pCapteurConnecte(Capteur *pCapteurConnecte);
    void            Set_tacptAccepteDrag(TypeAccept tacptAccepteDrag);

protected:
    void            dragEnterEvent(QGraphicsSceneDragDropEvent *event);     //Evenement d'entrée d'un drag dans l'item
    void            dragLeaveEvent(QGraphicsSceneDragDropEvent *event);     //Evenement de sortie d'un drag
    void            dropEvent(QGraphicsSceneDragDropEvent *event);          //Evenement de dépot
    void            mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);//Evenement de double click
    void            contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

private:
    QString         sNomBroche;
    Arduino         *oMonArduino;
    Capteur         *pCapteurConnecte;  //Capteur connecté à cette broche
    TypeAccept      tacptAccepteDrag;

};

#endif // ITEMBROCHE_H
