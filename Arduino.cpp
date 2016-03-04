#include "Arduino.h"
#include "qextserialPort.h"
#include "qextserialenumerator.h"
#include <QDebug>
#include <QString>

Arduino::Arduino():
    Port(NULL),
    Buffer("")
{

}

Arduino::~Arduino()
{
    this->Port=NULL;
    this->Buffer="";
}

bool Arduino::Ouvrir()
{

    //Création de la variable de retour de la fonction
    bool bRetour (false);

    //Création de la variable qui permet de tester si un arduino a été détecté
    bool ArduinoPresent(false);

    //L'objet mentionnant les infos des ports série
    QextSerialEnumerator enumerateur;

    //on met ces infos dans une liste
    QList<QextPortInfo> Ports = enumerateur.getPorts();

    //On parcours la totalité des ports séries détectés
    for(int i=0; i < Ports.length(); i++)
        {
            if(Ports[i].vendorID == 0x2341)    //Si un port avec un VID de 0x2341 l'Arduino est détecté
            {
                ArduinoPresent = true;  //La variable prend la valeur true si un arduino est détecté

                this->NomPort = Ports[i].portName;    //NomPort est complété par le numéro de port de l'Arduino

                //L'objet Port série
                this->Port = new QextSerialPort();

                //Quelques règlages pour que tout marche bien
                this->Port->setPortName(this->NomPort);
                this->Port->setBaudRate(BAUD9600);              //On règle la vitesse utilisée
                this->Port->setParity(PAR_NONE);                //On règle la parité
                this->Port->setStopBits(STOP_1);                //On règle le nombre de bits de stop
                this->Port->setDataBits(DATA_8);                //On règle le nombre de bits de données
                this->Port->setFlowControl(FLOW_OFF);           //Pas de contrôle de flux

                //On ouvre la connexion avec l'arduino
                Port->open(QextSerialPort::ReadWrite);

                //On test si la connexion avec l'arduino à réussie
                if(this->Port->isOpen())
                {
                    bRetour = true;
                    qDebug() << "La connexion avec l'arduino à réussie";
                }
                else
                {
                    qDebug() << "La connexion avec l'arduino à échouée";
                }
            }
        }

        //Vérification de la détection d'un arduino
        if(ArduinoPresent == false)
        {
            qDebug() << "Aucun arduino n'a été détecté";
        }

    return bRetour;
}

bool Arduino::Fermer()
{
    bool bRetour(false);

    //On ferme la connexion avec l'arduino
    this->Port->close();

    //On test si la fermeture de la connexion avec l'arduino à réussie
    if(!this->Port->isOpen())
    {
        bRetour = true;
        qDebug() << "La fermeture de la connexion avec l'arduino à réussie";
    }
    else
    {
        qDebug() << "La fermeture de la connexion avec l'arduino à échouée";
    }

    return bRetour;
}

void Arduino::EcrirePort(QString Commande)
{
    if(this->Port->isOpen() == true)
    {
        this->Port->write(Commande.toStdString().c_str());
    }
    else
    {
        qDebug() << "L'écriture sur le port série à échouée";
    }
}

QByteArray Arduino::LirePort()
{
    if(this->Port->isOpen() == true)
    {
        this->Buffer+=Port->readAll();
    }
    else
    {
        qDebug() << "La lecture sur le port série à échouée";
    }

    return this->Buffer;
}

int Arduino::LireCapteur(QString Commande)
{
    while(Buffer.isEmpty()!=true)
    {
        Buffer.clear();
    }
    int Retour(0);

    QString DonneesLues("");

    //Récupération des premiers caractères de la commande afin de définir quelle type d'entrée est à lire
    QString TypeEntree("");
    TypeEntree = Commande;
    TypeEntree.resize(1);

    //Type d'entrée Jack ou I2C
    this->EcrirePort(Commande);

    while(this->LirePort().right(1)!="\n")
    {
        this->RetourLecturePort = this->LirePort();
    }
    qDebug() << RetourLecturePort;
    DonneesLues += RetourLecturePort;
    DonneesLues = DonneesLues.remove(0,6);
    DonneesLues.resize(DonneesLues.size()-2);

    Retour = DonneesLues.toInt(0,10);

    //Type d'entrée I2C seulement
    if(TypeEntree == "i")
    {
        QString CopieDonneesLues("");
        int PoidsFort(0);
        int PoidsFaible(0);

        CopieDonneesLues = DonneesLues;
        DonneesLues.resize(1);

        if(DonneesLues == "-")
        {
            Retour = (-1);
        }
        else
        {
            PoidsFaible = DonneesLues.toInt(0,10);

            CopieDonneesLues = CopieDonneesLues.remove(0,2);
            PoidsFort = CopieDonneesLues.toInt(0,10);

            Retour = ((PoidsFaible*256)+PoidsFort);
        }

    }

    return Retour;
}
