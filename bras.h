/**
* \file bras.h
*
* \brief Déclaration de la classe Bras. Réceptionne les signaux de la manette destiné aux mouvements du bras, et émet les trames correspondantes.
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/

#ifndef BRAS_H
#define BRAS_H

#include <QObject>

/**
*
* \class Bras
*
* \brief Déclaration de la classe Bras. Réceptionne les signaux de la manette destiné aux mouvements du bras, et émet les trames correspondantes.
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/
class Bras : public QObject
{
    Q_OBJECT

    private:
        int valeurTourneEpaulePrecedente; //!< Dernière valeur de la trame TourneEpaule émise.
        int valeurLeveEpaulePrecedente;   //!< Dernière valeur de la trame LeveEpaule émise.
        int valeurTournePoignetPrecedente;//!< Dernière valeur de la trame TournePoignet émise.
        int valeurLevePoignetPrecedente;  //!< Dernière valeur de la trame LevePoignet émise.

    public:
        Bras(QObject *parent = nullptr);

    public slots:
        /**
         * @brief Emet la trame : tourner l'épaule du bras à droite/gauche.
         * @fn tourneEpaule
         * @param valeur double Force d'appui sur le joystick.
         */
        void tourneEpaule(double valeur);
        /**
         * @brief Emet la trame : lever ou baisser l'épaule du bras.
         * @fn leveEpaule
         * @param valeur double Force d'appui sur le joystick.
         */
        void leveEpaule(double valeur);
        /**
         * @brief Emet la trame :  lever le coude.
         * @fn leveCoude
         * @param appuye bool Touche appuyée ou non.
         */
        void leveCoude(bool appuye);
        /**
         * @brief Emet la trame : plier le coude.
         * @fn baisseCoude
         * @param appuye bool Touche appuyée ou non.
         */
        void baisseCoude(bool appuye);
        /**
         * @brief Permet de pivoter le poignet, en émettant la trame correspondante. Correspond au joystick gauche axe X de la manette.
         * @fn tournePoignet
         * @param valeur double Force d'appui sur le joystick.
         */
        void tournePoignet(double valeur);
        /**
         * @brief Permet de lever ou baisser le poignet, en émettant la trame correspondante. Correspond au joystick gauche axe Y de la manette.
         * @fn levePoignet
         * @param valeur double Force d'appui sur le joystick.
         */
        void levePoignet(double valeur);
        /**
         * @brief Emet la trame : serrer la pince.
         * @fn serrePince
         * @param appuye bool Touche appuyée ou non.
         */
        void serrePince(bool appuye);
        /**
         * @brief Emet la trame : relâcher la pince.
         * @fn lachePince
         * @param appuye bool Touche appuyée ou non.
         */
        void lachePince(bool appuye);
        /**
         * @brief Emet la trame : poser dans le bac le contenu de la pince.
         * @fn depose
         * @param appuye bool Touche appuyée ou non.
         */
        void depose(bool appuye);

    signals:
        void trameCree(QString trame); //!< Signal émis lorsqu'une trame a été créée et prête à être transmise.
};

#endif // BRAS_H
