#ifndef SCENEPLANDECABLAGE_H
#define SCENEPLANDECABLAGE_H

class ScenePlanDeCablage;

#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QSettings>
#include "Photo.h"
#include "ItemBroche.h"

class ScenePlanDeCablage : public QGraphicsScene
{
    Q_OBJECT

public:
    ScenePlanDeCablage(Arduino *oMonArduino, QObject* parent = 0); //Constructeur
    ~ScenePlanDeCablage(); //Destructeur

    QList<ItemBroche *> Get_lstListeBroche();

    void                Set_Interface(QString sNomInterface); //Mutateur de l'interface à afficher

private:
    Photo               *oMaPhoto;  //Photo de l'interface
    QString             sNomInterface;
    Arduino             *oMonArduino;
    QList<ItemBroche*>  lstListeBroche;//Liste des broches possibles

};

#endif // SCENEPLANDECABLAGE_H
