#include <QSettings>
#include <QDebug>
#include "ScenePlanDeCablage.h"

ScenePlanDeCablage::ScenePlanDeCablage(QObject *parent) :
    QGraphicsScene  (parent),
    Interface       (NULL)

{

}

ScenePlanDeCablage::~ScenePlanDeCablage()
{
    if(this->Interface != NULL)
    {
        this->removeItem(this->Interface);
        delete this->Interface;
        this->Interface = NULL;
    }

    for(register int i = 0; i < this->ListeBroche.length(); i++)
    {
        if(this->ListeBroche[i] != NULL)
        {
            this->removeItem(this->ListeBroche[i]);
            delete this->ListeBroche[i];
            this->ListeBroche[i] = NULL;
        }
    }
}

void ScenePlanDeCablage::setInterface(QString NomInterface)
{
    this->NomInterface = NomInterface;

    //Photo Interface
    if(Interface != NULL)
    {
        this->removeItem(this->Interface);
        delete this->Interface;
        this->Interface = NULL;
    }

    this->Interface = new Photo("./Interfaces/" + NomInterface);
    this->addItem(this->Interface);

    //Liste Broche
    for(register int i = 0; i < ListeBroche.length(); i++)
    {
        if(ListeBroche[i] != 0)
        {
            this->removeItem(ListeBroche[i]);
            delete ListeBroche[i];
            ListeBroche[i] = 0;
        }
    }

    //Ouverture de la configuration de l'interface
    QSettings Config("./Interfaces/" + NomInterface + "/config.ini", QSettings::IniFormat);

    QStringList ListeBroche(Config.childGroups());

    for(register int i = 0; i < ListeBroche.length(); i++)
    {
        if(ListeBroche[i].left(6) == "BROCHE")
        {
            Config.beginGroup(ListeBroche[i]);

            if(Config.value("Type").toStringList().contains("AI"))
            {
                ItemBroche *NouvelItem = (new ItemBroche(/*NomInterface/*, Conf*/));
                NouvelItem->setNom(Config.value("Nom").toString());
                NouvelItem->setX(Config.value("X").toInt());
                NouvelItem->setY(Config.value("Y").toInt());

                this->addItem(NouvelItem);

                this->ListeBroche.append(NouvelItem);
            }
            Config.endGroup();

        }
    }
}

