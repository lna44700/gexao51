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

ItemBroche::ItemBroche(QString NomInterface, QSettings *Conf, QGraphicsItem *parent) :
    QGraphicsItem   (parent),
    NomInterface    (NomInterface),
    AcceptDrag      (NORMAL),
    CapConnect      (0),
    Conf            (Conf)
{
    this->setAcceptDrops(true);
}

void ItemBroche::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    //Si aucun capteur n'est connecté
    if(CapConnect == 0)
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
        switch(AcceptDrag)
        {
        case NORMAL: //Cas normal, sans drag
            PremiereCouleur = QColor(0xFE, 0xFE, 0x98, 0xFF) ;
            SecondeCouleur  = QColor(0xC3, 0xC3, 0x67, 0xFF) ;
            Texte           = NomBroche;
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

        //Si le drag est acceptable ou non sur cette broche
        if(this->TypeBroche.contains(Config.value("PORT/Type").toString()))
        {
            this->AcceptDrag = ACCEPT;
        }
        else
        {
            this->AcceptDrag = REFUSE;
        }

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
        if(CapConnect == 0)
        {
            this->DefinirCapteur(event->mimeData()->text().mid(4));
        }
        else if(QMessageBox::question(0, "Modification", "Voulez vous écraser le capteur déjà installé?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            delete this->CapConnect;
            this->CapConnect = 0;
            this->DefinirCapteur(event->mimeData()->text().mid(4));
        }
    }
    else //Si le drag est initié de façon random par le systeme, on renvois vers la gestion parente
    {
        QGraphicsItem::dragEnterEvent(event);
    }
}

void ItemBroche::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if(this->CapConnect != 0)
    {
        event->accept();
        //f_DefinirActionsEtTests f_Modif(this->CapConnect, Conf);

        //f_Modif.exec();
    }
}

void ItemBroche::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    if(CapConnect != 0)
    {
        //création du menu
        QMenu menu;
        menu.addAction("Modifier");
        menu.addAction("Supprimer");
        menu.move(event->screenPos());

        //Exécution du menu et récupération de l'action choisie
        QAction* selectedAction = menu.exec();


        //Traitement de l'action en question
        if(selectedAction != 0)
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
                    delete CapConnect;
                    CapConnect = 0;

                    this->AcceptDrag = NORMAL;

                    QWidget* viewport = this->scene()->views().at(0)->viewport();
                    viewport->update();
                }
            }
        }

        event->accept();
    }
}

void ItemBroche::DefinirCapteur(QString NomCapteur)
{
    NouveauCA = NULL;
    //Création de l'objet capteur
    NouveauCA = new Capteur(NomCapteur, NomBroche);

    //Si on peut accepter le drop en fonction du type
    if(TypeBroche.contains("AI"))
    {
        //On l'ajoute réellement et on update la vue
        this->CapConnect = NouveauCA;
        QWidget* viewport = this->scene()->views().at(0)->viewport();
        viewport->update();
    }
    else
    {
       // delete NouveauCA; //Si le drop n'est pas acceptable, on delete le nouveau capteur
    }
}

Capteur *ItemBroche::get_NouveauCA() const
{
    return this->NouveauCA;
}


void ItemBroche::setId(unsigned int Id)
{
    if( Id > 0)
    {
        this->Id = Id;
    }
}

void ItemBroche::setNom(QString Nom)
{
    if(!Nom.isEmpty())
    {
        this->NomBroche = Nom;
    }
}

void ItemBroche::setBrocheNum(int BrocheNum)
{
    if(BrocheNum > 0)
        this->BrocheNum = BrocheNum;
}

void ItemBroche::setBrocheAna(int BrocheAna)
{
    if(BrocheAna >= -1)
        this->BrocheAna = BrocheAna;
}

void ItemBroche::setTypeBroche(QStringList TypeBroche)
{
    this->TypeBroche = TypeBroche;
}
