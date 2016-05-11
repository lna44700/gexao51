//-------------------------------------------------------------------------------
/**
 * @file        Arduino.cpp
 * @brief       Connexion, détection, lecture et écriture de l'Arduino
 *
 * @author      S.GUICHARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       1/02/16
 * @version     1.0
 * @date        20/04/16
 *
 * Cette classe va permettre que lorsque la maquette est déconnectée il y a possibilité
 * de la reconnecter automatiquement. Elle va permettre également de communiquer avec l'Arduino.
 *
 * Fabrication  Gexao51.pro
 */

//=====   En-Têtes Personnels   =====
#include "Arduino.h"
#include "qextserialPort.h"
#include "qextserialenumerator.h"

//=====   En-Têtes standards    =====
#include <QDebug>
#include <QString>
#include <unistd.h>

//-------------------------------------------------------------------------------

/**
 * Constructeur par défaut.
 */
Arduino::Arduino():
    pSemaphore(),
    bEstOuvert(false)
{
    this->pSemaphore = new QSemaphore (1);

    //L'objet Port série
    this->pPort = new QextSerialPort();
}

//-------------------------------------------------------------------------------

/**
 * Déstructeur.
 */
Arduino::~Arduino()
{
    this->pPort = NULL;
    this->pSemaphore = NULL;
    this->byaBuffer = "";
    this->sNomPort = "";
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va permettre de détecter si un Arduino Méga 2560 est connecté sur un port USB.
 * La méthode va également permettre l'ouverture du port afin de pouvoir communiquer avec le Shell Méga.
 * @code
 *     Ouvrir();
 * @endcode
 */
bool Arduino::Ouvrir()
{

    //Création de la variable de retour de la fonction
    bool bRetour (false);

    //Création de la variable qui permet de tester si un arduino a été détecté
    bool bArduinoPresent (false);

    //L'objet mentionnant les infos des ports série
    QextSerialEnumerator enumerateur;

    //on met ces infos dans une liste
    QList<QextPortInfo> lstPorts = enumerateur.getPorts();

    //On parcours la totalité des ports séries détectés
    for(int i = 0; i < lstPorts.length(); i++)
        {
            if(lstPorts[i].vendorID == 0x2341)    //Si un port avec un VID de 0x2341 l'Arduino est détecté
            {
                bArduinoPresent = true;  //La variable prend la valeur true si un arduino est détecté

                this->sNomPort = lstPorts[i].portName;    //NomPort est complété par le numéro de port de l'Arduino



                //Quelques règlages pour que tout marche bien
                this->pPort->setPortName    (this->sNomPort);
                this->pPort->setBaudRate    (BAUD9600);           //On règle la vitesse utilisée
                this->pPort->setParity      (PAR_NONE);           //On règle la parité
                this->pPort->setStopBits    (STOP_1);             //On règle le nombre de bits de stop
                this->pPort->setDataBits    (DATA_8);             //On règle le nombre de bits de données
                this->pPort->setFlowControl (FLOW_OFF);           //Pas de contrôle de flux

                //On ouvre la connexion avec l'arduino
                this->pPort->open(QextSerialPort::ReadWrite);
                //sleep(1);
                //this->LirePort();
                //sleep(1);

                //On test si la connexion avec l'arduino à réussie
                if(this->pPort->isOpen())
                {
                    bRetour = true;
                    qDebug() << "La connexion avec l'arduino à réussie";

                    this->bEstOuvert =true;
                }
                else
                {
                    bRetour = false;
                    this->bEstOuvert =false;

                    //qDebug() << "La connexion avec l'arduino à échouée";

                }
            }
        }

        //Vérification de la détection d'un arduino
        if(bArduinoPresent == false)
        {
            bRetour = false;
            qDebug() << "Aucun arduino n'a été détecté";
            this->bEstOuvert =false;

        }

    return bRetour;
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va permettre de fermer le port.
 * @code
 *     Fermer();
 * @endcode
 */
bool Arduino::Fermer()
{
    bool bRetour (false);

    //On ferme la connexion avec l'arduino
    this->pPort->close();

    //On test si la fermeture de la connexion avec l'arduino à réussie
    if(!this->pPort->isOpen())
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

//-------------------------------------------------------------------------------

/**
 * Cette méthode va permettre d'écrire sur le port série la commande reçu en paramètre.
 * @code
 *    EcrirePort("A10");
 * @endcode
 * @param Commande Commande à écrire sur le port permettant d'interroger le Shell Méga sur l'une de ses broches.
 */
void Arduino::EcrirePort(QString sCommande)
{
    //Si le port série est ouvert
    if(this->pPort->isOpen() == true)
    {
        this->pPort->write(sCommande.toStdString().c_str()); //On écrit sur le port la commande reçue en paramètre
    }
    //Sinon un message d'erreur est envoyé en sortie de compilation
    else
    {
        qDebug() << "L'écriture sur le port série à échouée";
    }
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va permettre de lire sur le port les données envoyées par le Shell Méga.
 * @code
 *    LirePort();
 * @endcode
 * @return QByteArray Buffer, retourne un buffer contenant les données lues sur le port.
 */
QByteArray Arduino::LirePort()
{
    //Si le port série est ouvert
    if(this->pPort->isOpen() == true)
    {
        while(this->byaBuffer.right(1) != "\n")
        {
            this->byaBuffer += this->pPort->readAll(); //On remplie le Buffer
        }
    }
    //Sinon un message d'erreur est envoyé en sortie de compilation
    else
    {
        qDebug() << "La lecture sur le port série à échouée";
    }

    qDebug() << byaBuffer;
    return this->byaBuffer;
}

//-------------------------------------------------------------------------------

/**
 * Cette méthode va permettre d'interpréter les données reçu par le Shell Méga en récupérant la valeur du capteur.
 * @code
 *    LireCapteur("A10");
 * @endcode
 * @param sCommande Commande à écrire sur le port permettant d'interroger le Shell Méga sur l'une de ses broches.
 * @return nRetour, retourne la valeur du capteur.
 */
int Arduino::LireCapteur(QString sCommande)
{

    //Acquisition du sémaphore
    this->pSemaphore->acquire(1);

    QByteArray byaRetourLecturePort("");
    QString sDonneesLues("");
    int nRetour(0);

    //On efface entièrement le Buffer pour une nouvelle lecture
    while(this->byaBuffer.isEmpty() != true)
    {
        this->byaBuffer.clear();
    }
    qDebug() << "aaa";

    //Type d'entrée Jack ou I2C
    this->EcrirePort(sCommande);
    qDebug() << "aaa";

    //Lecture du port jusqu'à la fin de la ligne
    while(byaRetourLecturePort.right(1) != "\n" )
    {
        byaRetourLecturePort = this->LirePort(); //On récupère le Buffer de la fonction LirePort()
    }

    sDonneesLues += byaRetourLecturePort;     //On copie le Buffer dans un QString

    qDebug() << sDonneesLues;

    sDonneesLues = sDonneesLues.remove(0,6);      //On supprime les 6 premiers caractères (VALUE=)
    sDonneesLues.resize(sDonneesLues.size()-2);   //On supprime les deux derniers caractères (\r\n)

    //Si l'entrée est de type jack, on renvoie la valeur maintenant
    //La valeur de retour est un entier
    nRetour = sDonneesLues.toInt();

    //Si l'entrée est de type I2C (si la commande commence par 'i'), la donnée à interpréter est sur deux octets,
    //il faut donc calculer la valeur décimale en fonction du poids fort et du poids faible
    if(sCommande[0] == 'i')
    {
        QString sCopieDonneesLues  ("");
        unsigned char wPoidsFort   (0);
        unsigned char wPoidsFaible (0);

        //On copie la variable DonneesLues
        sCopieDonneesLues = sDonneesLues;

        //On redimentionne la variable pour vérifier si elle commence par le caractère '-',
        //afin de savoir si une sonde à été détectée ou non
        sDonneesLues.resize(1);

        //Retourne la valeur -1 si aucun capteur n'est détecté sur le BUS I2C à l'adresse passée en paramètre
        if(sDonneesLues == "-")
        {
            nRetour = (-1);
        }
        else
        {
            //Si le caractère n'est pas '-', alors il s'agit de l'octet de poids fort, qu'on met dans une variable
            wPoidsFort = sDonneesLues.toInt();

            //On utilise la variable de copie pour récupérer l'octet de poids faible
            //On supprime les deux premiers caractères (le caractère correspondant au poids fort et la virgule)
            sCopieDonneesLues = sCopieDonneesLues.remove(0,2);

            //On met le poids faible dans une variable
            wPoidsFaible = sCopieDonneesLues.toInt();

            //On renvoie la valeur décimale
            nRetour = ((wPoidsFort*256)+wPoidsFaible);
        }
    }

    //On libère le sémaphore
    this->pSemaphore->release(1);

    return nRetour;
}
//-------------------------------------------------------------------------------
