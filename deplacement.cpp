#include "deplacement.h"
#include <cmath>
#include <QDebug>

/**
* \file deplacement.cpp
*
* \brief Définition de la classe Deplacement
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/

Deplacement::Deplacement(QObject *parent) : QObject(parent), valeurAvancerPrecedente("0"), valeurTournerPrecedente("0")
{

}

// JOYSTICK GAUCHE
/**
 * @brief Slot activé lorsque le joystick gauche est poussé à droite ou gauche. Emet un signal contenant la trame correspondante (déplacement : tourner à droite/gauche).
 * @fn Deplacement::tourneDroiteGauche
 * @param valeur double Force de l'appui sur le joystick (compris entre -1 et 1).
 */
void Deplacement::tourneDroiteGauche(double valeur)
{
    QString trame = "";

    valeur = round(valeur);

    if(valeur > 0)
        trame = "$DRO" + QString::number(valeur) + "\n";
    else if(valeur < 0)
        trame = "$GRO" + QString::number(-valeur) + "\n";
    else
        trame = "$DRO0\n";

    if (QString::number(valeur) != valeurTournerPrecedente)
    {
        emit trameCree(trame);
        valeurTournerPrecedente = QString::number(valeur);
    }
}

/**
 * @brief Slot activé lorsque le joystick gauche est poussé en avant ou arrière. Emet un signal contenant la trame correspondante (déplacement : avancer/reculer).
 * @fn Deplacement::rouleAvantArriere
 * @param valeur double Force de l'appui sur le joystick (compris entre -1 et 1).
 */
void Deplacement::rouleAvantArriere(double valeur)
{
    QString trame = "";
    valeur = round(valeur);

    if(valeur >= 0.5)
        trame = "$RRO" + QString::number(-valeur) + "\n";
    else if(valeur <= -0.5)
        trame = "$ARO" + QString::number(-valeur) + "\n";
    else
        trame = "$ARO0\n";

    if (QString::number(valeur) != valeurAvancerPrecedente)
    {
        emit trameCree(trame);
        valeurAvancerPrecedente = QString::number(valeur);
    }
}
