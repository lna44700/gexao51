#ifndef SCENEPLANDECABLAGE_H
#define SCENEPLANDECABLAGE_H

class ScenePlanDeCablage;

#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QSettings>
#include  "Photo.h"
#include "ItemBroche.h"

class ScenePlanDeCablage : public QGraphicsScene
{
    Q_OBJECT
public:
    ScenePlanDeCablage(QSettings* Conf, QObject* parent = 0); //Constructeur
    ~ScenePlanDeCablage(); //Destructeur

    void    setInterface(QString NomInterface); //Mutateur de l'interface à afficher

    QList<Capteur*> ListeCapteur;
    QList<ItemBroche*>  ListeBroche;//Liste des broches possibles




private:
    Photo*              Interface;  //Photo de l'interface
    QString             NomInterface;
    QSettings* Conf;                //Config generale
};

#endif // SCENEPLANDECABLAGE_H
