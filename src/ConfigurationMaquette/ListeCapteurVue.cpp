
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include "ListeCapteurVue.h"

ListeCapteurVue::ListeCapteurVue(QWidget *parent) :
    QListView(parent)
{
    this->setIconSize(QSize(48, 48));
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ListeCapteurVue::mousePressEvent(QMouseEvent *   pEvent)
{
    //Si c'est un appuis sur le click gauche, on sauvegarde sa position
    if(pEvent->button() == Qt::LeftButton)
    {
        this->PosDebutDrag = pEvent->pos() ;
    }
    QListView::mousePressEvent(pEvent) ;
}

void ListeCapteurVue::mouseMoveEvent(QMouseEvent *    pEvent)
{
    //Calcul les coordonnée du vecteur entre le point d'origine et la position actuelle, puis de sa longueur
    QPoint Vecteur  (pEvent->pos() - this->PosDebutDrag) ;
    int Distance    (Vecteur.manhattanLength()) ;

    //Si la distance est satisfaisante pour commencer un drag
    if(Distance > QApplication::startDragDistance())
    {
        //On commence le drag.
        QDrag* drag = new QDrag(this);

        QStandardItem* ItemDrag (static_cast<QStandardItemModel*>(this->model())->itemFromIndex(this->currentIndex()));
        QSize   TailleImageDrag (32, 32) ;
        QIcon   IconBouton      (ItemDrag->icon()) ;
        QPixmap ImageDrag       (IconBouton.pixmap(TailleImageDrag)) ;

        drag->setPixmap(ImageDrag);
        QMimeData * mime    (new QMimeData);
        mime->setText("Cap " + ItemDrag->data(Qt::UserRole).toString());

        drag->setMimeData(mime);

        drag->exec();
    }
    else
    {
        QListView::mouseMoveEvent(pEvent);
    }
}

