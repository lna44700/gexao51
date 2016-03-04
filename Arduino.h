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
    int LireCapteur(QString Commande);

private:
    QextSerialPort * Port;
    QString NomPort;
    QByteArray Buffer;
    QByteArray RetourLecturePort;
};

#endif // ARDUINO_H
