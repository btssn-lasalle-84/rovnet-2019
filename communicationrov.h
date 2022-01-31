/**
* \file communicationrov.h
*
* \brief Déclaration de la classe CommunicationRov. Gère la communication entre le Rov et le Rov.
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/

#ifndef COMMUNICATIONROV_H
#define COMMUNICATIONROV_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

/**
*
* \class CommunicationRov
*
* \brief Déclaration de la classe CommunicationRov. Gère la communication entre le Rov et le Rov.
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/

class CommunicationRov : public QObject
{
    Q_OBJECT

    private:
        QSerialPort *port;   //!< Port série pour la communication le programme et le Rov
        QString adressePort; //!< Adresse du port de communication
        QString trame;       //!< Trame reçue par le port
        QByteArray donnees;  //!< Dernière donnée reçue (ou en cours de réception)

    public:
        explicit CommunicationRov(QObject *parent = nullptr);
        ~CommunicationRov();

        /**
         * @brief Retourne l'é&tat d'ouverture du port de communication vers le Rov
         * @fn CommunicationRov::estCommunicationRovDisponible
         * @return bool vrai si le port vers le Rov est ouvert sinon faux
         */
        bool estCommunicationRovDisponible();
        const QString getPort(); //!< Retourne le port de communication utilisé.
        const QString getBaudRate(); //!< Retourne le baudrate utilisé.

    signals:
        /**
         * @brief Signal émis lorsque des nouvelles données ont été reçues.
         * @fn trameRecue
         * @param trame QString le contenu de la trame reçue du Rov
         */
        void trameRecue(QString trame);

    public slots:
        /**
         * @brief Récupère la trame envoyée par le rov, et émet un signal.
         * @fn lireTrame
         */
        void lireTrame();
        /**
         * @brief Envoie la trame passée en argument au Rov par liaison série.
         * @fn envoieTrame
         * @param trame QString le contenu de la trame envoyée au Rov
         */
        bool envoieTrame(QString trame);
        void changePortCommunication(QString nouveauPort); //!< Modifie le port de communication du rov.
        void changeBaudRate(QString nouveauBaudRate); //!< Modifie le baudrate utilisé.

};

#endif // COMMUNICATIONROV_H
