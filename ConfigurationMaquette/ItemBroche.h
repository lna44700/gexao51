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
    ItemBroche(QString NomInterface, QSettings *Conf, QGraphicsItem* parent = 0); //Constructeur

            void            paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/); //DEssine l'objet
            QRectF          boundingRect() const; //Rectangle entourant l'objet

    inline  unsigned int    getId()         { return Id; }          //Accesseur de l'id
    inline  QString         getNom()        { return NomBroche; }         //Accesseur du nom
    inline  int             getBrocheNum()  { return BrocheNum; }   //Accesseur du numero de broche numérique
    inline  int             getBrocheAna()  { return BrocheAna; }   //Accesseur du numero de broche analogique
    inline  QStringList     getTypeBroche() { return TypeBroche; }  //Accesseur des types de broches

            void            setId(unsigned int Id);                 //|Setters correspondant...
            void            setNom(QString Nom);                    //|
            void            setBrocheNum(int BrocheNum);            //|
            void            setBrocheAna(int BrocheAna);            //|
            void            setTypeBroche(QStringList TypeBroche);  //|

            void            Enregistrer();
            Capteur *NouveauCA;
            Capteur* CapConnect;  //Capteur connecté à cette broche


            Capteur* get_NouveauCA() const;

protected:
            void            dragEnterEvent(QGraphicsSceneDragDropEvent *event);     //Evenement d'entrée d'un drag dans l'item
            void            dragLeaveEvent(QGraphicsSceneDragDropEvent *event);     //Evenement de sortie d'un drag
            void            dropEvent(QGraphicsSceneDragDropEvent *event);          //Evenement de dépot
            void            mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);//Evenement de double click
            void            contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

private:
            void    DefinirCapteur(QString Mime); //Defninir le capteur

    unsigned int    Id;
    QString         NomBroche;
    QString         NomInterface;
    int             BrocheNum;
    int             BrocheAna;
    QStringList     TypeBroche;

    TypeAccept      AcceptDrag;

    QSettings* Conf;                //Config complete
};

#endif // ITEMBROCHE_H
