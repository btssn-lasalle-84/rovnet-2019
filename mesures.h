#ifndef MESURES_H
#define MESURES_H

/**
  * \file mesures.h
  *
  * \brief Déclaration de la classe Mesures. Gestion des mesures des capteurs (température, irradiation, et distance).
  *
  * \author REYNIER Jacques & Nicolas BOFFREDO
  *
  * \version 1.1
  *
  * \date Jeudi 13 Juin 2019
  *
  */

#include <QObject>
#include <QTimer>
#include "communicationrov.h"

/**
  * \class Mesures
  *
  * \brief Déclaration de la classe Mesures. Gestion des mesures des capteurs (température, irradiation, et distance).
  *
  * \author REYNIER Jacques & Nicolas BOFFREDO
  *
  * \version 1.1
  *
  * \date Jeudi 13 Juin 2019
  *
  */
class Mesures : public QObject
{
    Q_OBJECT

    private:
        double temperature; //!< Dernière température relevée.
        double irradiation; //!< Dernier taux d'irradiation relevé.
        double distance;    //!< Dernière mesure du capteur de proximité relevé.
        QTimer *compteur;   //!< Compteur envoyant toutes les x secondes un signal timeout.

    public:
        explicit Mesures(QObject *parent = nullptr); //!< Constructeur de la classe Mesures.
        ~Mesures();                                  //!< Destructeur de la classe Mesures.
        /**
         * @brief Renvoie la dernière température reçue.
         * @fn getTemperature
         * @return temperature int Dernière température relevée.
         */
        double getTemperature() const;
        /**
         * @brief Renvoie le dernier taux d'irradiation reçu.
         * @fn getIrradiation
         * @return irradiation int Dernier taux d'irradiation relevé.
         */
        double getIrradiation() const;
        /**
         * @brief Renvoie le dernier relevé du capteur de proximité.
         * @fn getDistance
         * @return distance double Dernier relevé du capteur de proximité.
         */
        double getDistance() const;

        int getFrequenceArchivage() const; //!< Renvoie la fréquence d'archivage des données.

        void stockeDonnees(QString type, QString donnee); //!< Stocke la donnee passée en argument.

    signals:
        void irradiationActualisee(double); //!< Signal émis lorsqu'une nouvelle valeur d'irradiation est reçue.
        void temperatureActualisee(double); //!< Signal émis lorsqu'une nouvelle valeur de température est reçue.
        void distanceActualisee(double);    //!< Signal émis lorsqu'une nouvelle valeur de distance est reçue.
        void mesuresBDDPrete(double temperature, double irradiation); //!< Signal émis toutes les x secondes visant à stocker les valeurs de température et d'irradiation (en argument) dans la BDD.

    public slots:
        void traiteTrame(QString trame);     //!< Vérifie la validité, et découpe la trame reçue.
        void envoieMesuresBDD();             //!< Envoie un signal toutes les 30 secondes contenant la température et l'irradiation.
        void modifieFrequenceArchivage(int); //!< Modifie l'interval du timer correspondant à l'archivage des mesures dans la BDD.
};

#endif // MESURES_H
