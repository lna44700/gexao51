//-------------------------------------------------------------------------------
/**
 * @file        ListeCapteurVue.cpp
 * @brief       Gestion du click sur la photo d'un capteur et du déplacement lors d'un Drag & Drop
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
#include "ListeCapteurVue.h"

//=====   En-Têtes standards    =====
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

//-------------------------------------------------------------------------------

/**
 * Constructeur paramétré
 * @param *oMonArduino Pointeur de l'objet Arduino créé dans le fichier d'application F_Principale
 */
ListeCapteurVue::ListeCapteurVue(QWidget *parent) :
    QListView(parent)
{
    this->setIconSize(QSize(48, 48));
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode gère l'appui sur une photo pour le Drag
 * @param *pEvent QMouseEvent Contient l'êvenement de la souris
 */
void ListeCapteurVue::mousePressEvent(QMouseEvent * pEvent)
{
    //Si c'est un appuis sur le click gauche, on sauvegarde sa position
    if(pEvent->button() == Qt::LeftButton)
    {
        this->pntPositionDebutDrag = pEvent->pos() ;
    }
    QListView::mousePressEvent(pEvent) ;
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode gère le déplacemen de la photo pour le Drag and Drop
 * @param *pEvent QMouseEvent Contient l'êvenement de la souris
 */
void ListeCapteurVue::mouseMoveEvent(QMouseEvent * pEvent)
{
    //Calcul les coordonnée du vecteur entre le point d'origine et la position actuelle, puis de sa longueur
    QPoint Vecteur  (pEvent->pos() - this->pntPositionDebutDrag) ;
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
//-------------------------------------------------------------------------------
