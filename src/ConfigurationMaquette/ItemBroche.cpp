//-------------------------------------------------------------------------------
/**
 * @file        ItemBroche.cpp
 * @brief       Gestion du Drag & Drop
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
#include "F_ChoisirSonde.h"
#include "ItemBroche.h"
#include "Capteur.h"

//=====   En-Têtes standards    =====
#include <QBrush>
#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QDrag>
#include <QMimeData>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>

//-------------------------------------------------------------------------------

/**
 * Constructeur paramétré
 * @param *oMonArduino Pointeur de l'objet Arduino créé dans le fichier d'application F_Principale
 */
ItemBroche::ItemBroche(Arduino *oMonArduino, QGraphicsItem *parent) :
    QGraphicsItem   (parent),
    tacptAccepteDrag      (NORMAL),
    pCapteurConnecte      (NULL)
{
    this->setAcceptDrops(true);
    this->oMonArduino = new Arduino;
    this->oMonArduino = oMonArduino;
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va peindre sur les photos d'interface les zones
 * prévues pour le Drop et gérer ce qui y est affiché à l'interieur de celle-ci
 */
void ItemBroche::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    //Si aucun capteur n'est connecté
    if(this->pCapteurConnecte == NULL)
    {
        //On dessine un cercle avec le nom de la broche
        QRectF rect (this->boundingRect());

        //Définition du dégradé
        QPointF         PremierPoint    (0, 0) ;
        QPointF         SecondPoint     (0, TAILLE_ITEM) ;
        QLinearGradient grad            (PremierPoint, SecondPoint) ;
        QColor          PremiereCouleur;
        QColor          SecondeCouleur;
        QString         Texte;

        //Gestion de l'affichage en cas de drag
        switch(this->tacptAccepteDrag)
        {
        case NORMAL: //Cas normal, sans drag
            PremiereCouleur = QColor(0xFE, 0xFE, 0x98, 0xFF) ;
            SecondeCouleur  = QColor(0xC3, 0xC3, 0x67, 0xFF) ;
            Texte           = this->sNomBroche;
            break;

        case ACCEPT: //Cas si le drag est acceptable
            PremiereCouleur = QColor(0x98, 0xFE, 0x98, 0xFF) ;
            SecondeCouleur  = QColor(0x62, 0xBA, 0x62, 0xFF) ;
            Texte           = "Oui";
            break;
        }

        grad.setColorAt(0, PremiereCouleur);
        grad.setColorAt(1, SecondeCouleur);

        //Définition du brush
        QBrush brush(grad);
        painter->setBrush(brush);

        painter->drawEllipse(rect);

        painter->setPen(QPen());

        painter->drawText(rect, Qt::AlignCenter, Texte);
    }
    else //Sinon on affiche la photo
    {
        painter->drawPixmap(0, 0, this->pCapteurConnecte->Get_pxmPhoto().scaled(this->boundingRect().size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va définir la taille de chaque zone prévue pour le Drop
 * @return QrectF contient les dimensions de la zone à créer
 */
QRectF ItemBroche::boundingRect() const
{
    return QRectF(0, 0, TAILLE_ITEM, TAILLE_ITEM);
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va gérer le déplassement des capteurs qui seront dans la liste, pour le Drag
 */
void ItemBroche::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    //Si c'est un drag que l'on a initié, on le gere
    if(event->mimeData()->text().left(3) == "Cap")
    {
        event->accept();

        QSettings Config("./IOCard/" + event->mimeData()->text().mid(4) + "/config.ini", QSettings::IniFormat);

        this->tacptAccepteDrag = ACCEPT;

        //On update la vue
        QWidget* viewport = this->scene()->views().at(0)->viewport();
        viewport->update();
    }
    else //Si c'est un drag random, on renvois à la gestion parente
    {
        QGraphicsItem::dragEnterEvent(event);
    }
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va être appelée lorsqu'un Drag est annulé
 */
void ItemBroche::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();

    this->tacptAccepteDrag = NORMAL;

    QWidget* viewport = this->scene()->views().at(0)->viewport();
    viewport->update();
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va gérer le relachement des photos de capteur dans les zones prévues
 * pour le Drop
 */
void ItemBroche::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    //Si c'est un drag que l'on a initié
    if(event->mimeData()->text().left(3) == "Cap" && this->tacptAccepteDrag == ACCEPT)
    {
        //S'il n'y a pas de capteur déjà connecté
        if(this->pCapteurConnecte == NULL)
        {
            this->DefinirCapteur(event->mimeData()->text().mid(4), sNomBroche);
            QWidget* viewport = this->scene()->views().at(0)->viewport();
            viewport->update();
        }
        else if(QMessageBox::question(0, "Modification", "Voulez vous écraser le capteur déjà installé?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            delete this->pCapteurConnecte;
            this->pCapteurConnecte = NULL;
            this->DefinirCapteur(event->mimeData()->text().mid(4), this->sNomBroche);
            QWidget* viewport = this->scene()->views().at(0)->viewport();
            viewport->update();
        }
    }
    else //Si le drag est initié de façon random par le systeme, on renvois vers la gestion parente
    {
        QGraphicsItem::dragEnterEvent(event);
    }
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va gérer le double click lors du Drag and Drop
 */
void ItemBroche::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if(this->pCapteurConnecte != NULL)
    {
        event->accept();
    }
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va être appelé lors d'un click droit sur un capteur déposer sur l'interface
 * et permettre de le supprimer
 */
void ItemBroche::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    if(this->pCapteurConnecte != NULL)
    {
        //création du menu
        QMenu menu;
        //menu.addAction("Modifier");
        menu.addAction("Supprimer");
        menu.move(event->screenPos());

        //Exécution du menu et récupération de l'action choisie
        QAction *selectedAction = menu.exec();


        //Traitement de l'action en question
        if(selectedAction != NULL)
        {
            if(selectedAction->text() == "Modifier")
            {
                //f_DefinirActionsEtTests f_Modif(this->pCapteurConnecte, Conf);

                //f_Modif.exec();
            }
            else
            {
                if(QMessageBox::question(0, "Suppression", "Etes-vous sûr de supprimer cet élément?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                {
                    //delete pCapteurConnecte;
                    //pCapteurConnecte = 0;
                    pCapteurConnecte=NULL;

                    this->tacptAccepteDrag = NORMAL;

                    QWidget* viewport = this->scene()->views().at(0)->viewport();
                    viewport->update();
                }
            }
        }

        event->accept();
    }
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va créer un nouvel objet capteur
 * @param sNomCapteur QString contenant le nom du capteur
 * @param sNomBoche QString contenant le nom de la broche sur laquelle l'image du capteur
 * à été déposée
 */
void ItemBroche::DefinirCapteur(QString sNomCapteur, QString sNomBroche)
{
    //Création de l'objet capteur
    this->tacptAccepteDrag = ACCEPT;
    Capteur* NouveauCA = new Capteur(this->oMonArduino, sNomCapteur, sNomBroche);

    //On l'ajoute réellement et on update la vue
    this->pCapteurConnecte = NouveauCA;
}

//-------------------------------------------------------------------------------

/**
 * Accesseur permettant de récupérer l'objet capteur connecté à une broche
 * @return *pCapteurConnecte Capteur. Pointeur sur le capteur connecté
 */
Capteur *ItemBroche::Get_pCapteurConnecte()
{
    return this->pCapteurConnecte;
}

//-------------------------------------------------------------------------------

/**
 * Mutateur permettant de définir le nom de la broche
 * @param sNomBroche QString
 */
void ItemBroche::Set_sNomBroche(QString sNomBroche)
{
    if(!sNomBroche.isEmpty())
    {
        this->sNomBroche = sNomBroche;
    }
}

//-------------------------------------------------------------------------------

/**
 * Mutateur permettant de placer un objet capteur sur la broche
 * @param *pCapteurConnecte Capteur
 */
void *ItemBroche::Set_pCapteurConnecte(Capteur *pCapteurConnecte)
{
    this->pCapteurConnecte = pCapteurConnecte;
}

//-------------------------------------------------------------------------------

/**
 * Mutateur permettant de modifier l'attribut tacptAccepteDrag
 * @param *tacptAccepteDrag TypeAccept prend la valeur "ACCEPT" ou "NORMAL"
 */
void ItemBroche::Set_tacptAccepteDrag(TypeAccept tacptAccepteDrag)
{
    this->tacptAccepteDrag = tacptAccepteDrag;
}
//-------------------------------------------------------------------------------
