#ifndef ARDUINO_H
#define ARDUINO_H
#include "qextserialport.h"
#include <QSemaphore>

class Arduino
{

public:
    Arduino();
    ~Arduino();   

    void                EcrirePort(QString sCommande);
    int                 LireCapteur(QString sCommande);
    bool                Ouvrir();
    bool                Fermer();
    QByteArray          LirePort();

    bool                bEstOuvert;

private:
    QSemaphore          *pSemaphore;
    QextSerialPort      *pPort;
    QByteArray          byaBuffer;
    QString             sNomPort;

};

#endif // ARDUINO_H
