/**
* \file deplacement.h
*
* \brief Déclaration de la classe Deplacement. Réceptionne les signaux de la manette destiné aux déplacements du Rov, et émet les trames correspondantes.
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/

#ifndef DEPLACEMENT_H
#define DEPLACEMENT_H

#include <QObject>

/**
* \class Deplacement
*
* \brief Déclaration de la classe Deplacement. Réceptionne les signaux de la manette destiné aux déplacements du Rov, et émet les trames correspondantes.
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/
class Deplacement : public QObject
{
    Q_OBJECT

    private:
        QString valeurAvancerPrecedente; //!< Dernière valeur de la trame Avancer émise.
        QString valeurTournerPrecedente; //!< Dernière valeur de la trame Tourner émise.

    public:
        Deplacement(QObject *parent = nullptr);

    public slots:
        /**
         * @brief Emet la trame : tourner à droite/gauche.
         * @fn tourneDroiteGauche
         * @param valeur double Force de l'appui sur le joystick (compris entre -1 et 1).
         */
        void tourneDroiteGauche(double valeur);
        /**
         * @brief Emet la trame : avancer/reculer
         * @fn rouleAvantArriere
         * @param valeur double Force de l'appui sur le joystick (compris entre -1 et 1).
         */
        void rouleAvantArriere(double valeur);

    signals:
        /**
         * @brief Signal émis lorsqu'une trame est prête à être transmise.
         * @param trame QString Trame à transmettre.
         */
        void trameCree(QString trame);
};

#endif // DEPLACEMENT_H
