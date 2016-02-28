#ifndef ARDUINO_H
#define ARDUINO_H
#include "qextserialport.h"

class Arduino
{
public:
    Arduino();
    ~Arduino();
    bool Ouvrir();
    bool Fermer();
    void EcrirePort(QString Commande);
    QByteArray LirePort();



private:
    QextSerialPort * Port;
    QString NomPort;
    QByteArray Buffer;
};

#endif // ARDUINO_H
