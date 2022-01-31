#include "bras.h"
#include <cmath>
#include <QDebug>

/**
* \file bras.cpp
*
* \brief Définition de la classe Bras.
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/

/**
 * @brief Constructeur de la classe Bras
 * @fn Bras::Bras
 * @param parent QObject* objet Qt parent
 */
Bras::Bras(QObject *parent) : QObject(parent), valeurTourneEpaulePrecedente(0), valeurLeveEpaulePrecedente(0), valeurTournePoignetPrecedente(0), valeurLevePoignetPrecedente(0)
{

}

// SLOTS
/**
 * @brief Contrôle la rotation de l'épaule (entre autre, la direction du bras), en émettant la trame correspondante. Correspond au joystick droite de la manette.
 * @fn Bras::tourneEpaule
 * @param valeur double Force de l'appui sur le joystick (entre -1 et 1).
 */
void Bras::tourneEpaule(double valeur)    // Joystick droit axe x
{
    QString trame = "$TEP";
    int direction = 0;      // Par défaut, la direction est indiquée à 0

    if (valeur <= -0.5)
        direction = 1;

    else if (valeur >= 0.5)
        direction = -1;

    if (valeurTourneEpaulePrecedente != direction)
    {
        valeurTourneEpaulePrecedente = direction;
        trame += QString::number(direction) + "\n";
        if (direction != 0)
            emit trameCree(trame);
    }
}

/**
 * @brief Contrôle l'angle de levé de l'épaule, en émettant la trame correspondante. Correspond au joystick droite de la manette.
 * @fn Bras::leveEpaule
 * @param valeur double Force de l'appui sur le joystick (entre -1 et 1).
 */
void Bras::leveEpaule(double valeur)       // Joystick droit axe y
{
    QString trame = "$LEP";
    int direction = 0;      // Par défaut, la direction est indiquée à 0

    if (valeur <= -0.5)
        direction = 1;

    else if (valeur >= 0.5)
        direction = -1;

    if (valeurLeveEpaulePrecedente != direction)
    {
        valeurLeveEpaulePrecedente = direction;
        trame += QString::number(direction) + "\n";
       emit trameCree(trame);
    }
}

/**
 * @brief Permet de lever le coude, en émettant la trame correspondante. Correspond au bouton triangle de la manette.
 * @fn Bras::leveCoude
 * @param appuye bool Bouton appuyé, ou non.
 *
 * @todo Changer l'envoie de trame. Envoyer 1 quand Triangle, -1 quand Croix. -> Diminution nombre code différents dans la trame, simplification du décodage des trames.
 */
void Bras::leveCoude(bool appuye)          // Triangle
{
    QString trame = "$LCO" + QString::number(appuye) + "\n";

    emit trameCree(trame);
}

/**
 * @brief Permet de plier le coude, en émettant la trame correspondante. Correspond au bouton X de la manette.
 * @fn Bras::baisseCoude
 * @param appuye bool Bouton appuyé, ou non.
 */
void Bras::baisseCoude(bool appuye)          // Croix
{
    QString trame = "$LCO" + QString::number(-appuye) + "\n";

    emit trameCree(trame);
}

/**
 * @brief Permet de pivoter le poignet, en émettant la trame correspondante. Correspond au joystick gauche axe X de la manette.
 * @fn Bras::tournePoignet
 * @param valeur double Force d'appui sur le joystick.
 */
void Bras::tournePoignet(double valeur)   // Joystick Gauche Axe X
{
    QString trame = "$TPO";
    int direction = 0;      // Par défaut, la direction est indiquée à 0

    if (valeur <= -0.5)
        direction = -1;

    else if (valeur >= 0.5)
        direction = 1;

    if (valeurTournePoignetPrecedente != direction)
    {
        valeurTournePoignetPrecedente = direction;
        trame += QString::number(direction) + "\n";
        emit trameCree(trame);
    }
}

/**
 * @brief Permet de lever ou baisser le poignet, en émettant la trame correspondante. Correspond au joystick gauche axe Y de la manette.
 * @fn Bras::levePoignet
 * @param valeur double Force d'appui sur le joystick.
 */
void Bras::levePoignet(double valeur)   // Joystick Gauche Axe Y
{
    QString trame = "$LPO";
    int direction = 0;      // Par défaut, la direction est indiquée à 0

    if (valeur <= -0.5)
        direction = 1;

    else if (valeur >= 0.5)
        direction = -1;

    if (valeurLevePoignetPrecedente != direction)
    {
        valeurLevePoignetPrecedente = direction;
        trame += QString::number(direction) + "\n";
        emit trameCree(trame);
    }
}

/**
 * @brief Permet de serrer la pince, en émettant la trame correspondante. Correspond au bouton R1 de la manette.
 * @fn Bras::serrePince
 * @param appuye bool Bouton appuyé, ou non.
 */
void Bras::serrePince(bool appuye)         // R1
{
    QString trame = "$FPI" + QString::number(appuye) + "\n";

    emit trameCree(trame);
}

/**
 * @brief Permet de relâcher la pince, en émettant la trame correspondante. Correspond au bouton L1 de la manette.
 * @fn Bras::lachePince
 * @param appuye bool Bouton appuyé, ou non.
 */
void Bras::lachePince(bool appuye)         // L1
{
    QString trame = "$OPI" + QString::number(-appuye) + "\n";

    emit trameCree(trame);
}

/**
 * @brief Emet la trame : poser dans le bac le contenu de la pince.
 * @fn Bras::depose
 * @param appuye bool Touche appuyée ou non.
 */
void Bras::depose(bool appuye)             // Bouton start
{
    QString trame = "$DEP" + QString::number(appuye) + "/";

    emit trameCree(trame);
}
