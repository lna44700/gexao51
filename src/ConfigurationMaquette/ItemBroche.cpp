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
#include "F_ChoisirSonde.h"
#include "ItemBroche.h"
#include "Capteur.h"

ItemBroche::ItemBroche(QGraphicsItem *parent) :
    QGraphicsItem   (parent),
    AcceptDrag      (NORMAL),
    CapConnect      (NULL)
{
    this->setAcceptDrops(true);
}

void ItemBroche::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    //Si aucun capteur n'est connecté
    if(this->CapConnect == NULL)
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
        switch(this->AcceptDrag)
        {
        case NORMAL: //Cas normal, sans drag
            PremiereCouleur = QColor(0xFE, 0xFE, 0x98, 0xFF) ;
            SecondeCouleur  = QColor(0xC3, 0xC3, 0x67, 0xFF) ;
            Texte           = this->NomBroche;
            break;

        case ACCEPT: //Cas si le drag est acceptable
            PremiereCouleur = QColor(0x98, 0xFE, 0x98, 0xFF) ;
            SecondeCouleur  = QColor(0x62, 0xBA, 0x62, 0xFF) ;
            Texte           = "Oui";
            break;

        case REFUSE: //Cas si le drag est refusé
            PremiereCouleur = QColor(0xFE, 0x98, 0x98, 0xFF) ;
            SecondeCouleur  = QColor(0xBA, 0x62, 0x62, 0xFF) ;
            Texte           = "Non";
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
        painter->drawPixmap(0, 0, this->CapConnect->getPhoto().scaled(this->boundingRect().size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

QRectF ItemBroche::boundingRect() const
{
    return QRectF(0, 0, TAILLE_ITEM, TAILLE_ITEM);
}

void ItemBroche::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    //Si c'est un drag que l'on a initié, on le gere
    if(event->mimeData()->text().left(3) == "Cap")
    {
        event->accept();

        QSettings Config("./IOCard/" + event->mimeData()->text().mid(4) + "/config.ini", QSettings::IniFormat);

        this->AcceptDrag = ACCEPT;

        //On update la vue
        QWidget* viewport = this->scene()->views().at(0)->viewport();
        viewport->update();
    }
    else //Si c'est un drag random, on renvois à la gestion parente
    {
        QGraphicsItem::dragEnterEvent(event);
    }
}

void ItemBroche::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();

    this->AcceptDrag = NORMAL;

    QWidget* viewport = this->scene()->views().at(0)->viewport();
    viewport->update();
}

void ItemBroche::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    //Si c'est un drag que l'on a initié
    if(event->mimeData()->text().left(3) == "Cap" && this->AcceptDrag == ACCEPT)
    {
        //S'il n'y a pas de capteur déjà connecté
        if(this->CapConnect == NULL)
        {
            this->DefinirCapteur(event->mimeData()->text().mid(4), NomBroche);
            QWidget* viewport = this->scene()->views().at(0)->viewport();
            viewport->update();
        }
        else if(QMessageBox::question(0, "Modification", "Voulez vous écraser le capteur déjà installé?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            delete this->CapConnect;
            this->CapConnect = NULL;
            this->DefinirCapteur(event->mimeData()->text().mid(4), this->NomBroche);
            QWidget* viewport = this->scene()->views().at(0)->viewport();
            viewport->update();
        }
    }
    else //Si le drag est initié de façon random par le systeme, on renvois vers la gestion parente
    {
        QGraphicsItem::dragEnterEvent(event);
    }
}

void ItemBroche::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if(this->CapConnect != NULL)
    {
        event->accept();
    }
}

void ItemBroche::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    if(this->CapConnect != NULL)
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
                //f_DefinirActionsEtTests f_Modif(this->CapConnect, Conf);

                //f_Modif.exec();
            }
            else
            {
                if(QMessageBox::question(0, "Suppression", "Etes-vous sûr de supprimer cet élément?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                {
                    //delete CapConnect;
                    //CapConnect = 0;
                    CapConnect=NULL;

                    this->AcceptDrag = NORMAL;

                    QWidget* viewport = this->scene()->views().at(0)->viewport();
                    viewport->update();
                }
            }
        }

        event->accept();
    }
}

void ItemBroche::DefinirCapteur(QString NomCapteur, QString NomBroche)
{
    //Création de l'objet capteur
    this->AcceptDrag = ACCEPT;
    Capteur* NouveauCA = new Capteur(NomCapteur, NomBroche);

    //On l'ajoute réellement et on update la vue
    this->CapConnect = NouveauCA;

}

void ItemBroche::setNom(QString Nom)
{
    if(!Nom.isEmpty())
    {
        this->NomBroche = Nom;
    }
}

