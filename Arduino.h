#ifndef ARDUINO_H
#define ARDUINO_H
#include "qextserialport.h"
#include <QSemaphore>

class Arduino
{

public:
    Arduino();
    ~Arduino();
    bool Ouvrir();
    bool Fermer();
    void EcrirePort(QString Commande);
    QByteArray LirePort();
    int LireCapteur(QString Commande);
    QSemaphore * Semaphore;

private:
    QextSerialPort * Port;
    QByteArray Buffer;
    QString NomPort;

};

#endif // ARDUINO_H
