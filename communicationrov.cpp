#include "communicationrov.h"
#include <QDebug>

/**
* \file communicationrov.cpp
*
* \brief Définition de la classe CommunicationRov
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Mars 2019
*
*/

CommunicationRov::CommunicationRov(QObject *parent) : QObject(parent), adressePort("/dev/ttyUSB0")
{
    port = new QSerialPort(adressePort);

    port->setBaudRate(QSerialPort::Baud38400);

    port->open(QIODevice::ReadWrite);
    qDebug() << Q_FUNC_INFO << "Port ouvert :" << port->isOpen();

    if(port->isOpen())
        connect(port, SIGNAL(readyRead()), this, SLOT(lireTrame()));
}

CommunicationRov::~CommunicationRov()
{
    port->close();
    delete port;
}

/**
 * @brief Indique le port utilisé pour la communication avec le rov.
 * @fn CommunicationRov::getPort
 * @return adressePort QString le port de communication utilisé actuellement.
 */
const QString CommunicationRov::getPort()
{
    return adressePort;
}

/**
 * @brief Indique le Baudrate utilisé pour la communication avec le rov.
 * @fn CommunicationRov::getBaudRate
 * @return baudrate QString le baudrate utilisé actuellement pour la communication.
 */
const QString CommunicationRov::getBaudRate()
{
    return QString::number(port->baudRate());
}

/**
 * @brief Change le port de communication utilisé pour la liaison avec le rov.
 * @fn CommunicationRov::changePortCommunication
 * @param nouveauPort QString le nouveau port auquel se connecter.
 */
void CommunicationRov::changePortCommunication(QString nouveauPort)
{
    port->close();
    port->setPortName(nouveauPort);
    port->open(QIODevice::ReadWrite);
    adressePort = nouveauPort;
    qDebug() << "Port : " << port->portName() << "Ouvert : " << port->isOpen();
}

/**
 * @brief Change le débit (baudrate) utilisé pour la communication avec le rov.
 * @fn CommunicationRov::changeBaudRate
 * @param nouveauBaudRate QString le nouveau baudrate à utiliser.
 */
void CommunicationRov::changeBaudRate(QString nouveauBaudRate)
{
    port->setBaudRate(nouveauBaudRate.toInt());
    qDebug() << Q_FUNC_INFO << nouveauBaudRate.toInt();
}

/**
 * @brief Retourne l'é&tat d'ouverture du port de communication vers le Rov
 * @fn CommunicationRov::estCommunicationRovDisponible
 * @return bool disponible, vrai si le port vers le Rov est ouvert sinon faux
 */
bool CommunicationRov::estCommunicationRovDisponible()
{
    return port->isOpen();
}

/**
 * @brief Récupère la trame envoyée par le Rov, et la renvoie sous un signal si cette dernière est complète.
 * @fn CommunicationRov::lireTrame
 */
void CommunicationRov::lireTrame()
{
    donnees += port->readAll();
    qDebug() << donnees;

    if(donnees.endsWith("\n"))
    {
        if(donnees.startsWith("$"))
        {
            qDebug() << Q_FUNC_INFO << donnees;
            emit trameRecue(donnees);
        }

        donnees.clear();
    }
}

/**
 * @brief Envoie une trame au Rov.
 * @param trame QString Trame à envoyer.
 * @return bool envoye, vrai si la trame a été envoyée, sinon faux
 */
bool CommunicationRov::envoieTrame(QString trame)
{
    if(!estCommunicationRovDisponible())
        return false;

    qDebug() << Q_FUNC_INFO << "trame" << trame;
    qint64 nbEnvoyes = port->write(trame.toLocal8Bit());
    port->waitForReadyRead(1100);

    if(nbEnvoyes > 0)
        return true;
    return false;
}
