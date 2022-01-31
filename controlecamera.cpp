/**
  *
  * @file controlecamera.cpp
  *
  * @brief Définition de la classe ControleCamera.
  *
  * @author Nicolas BOFFREDO
  *
  * @version 1.1
  *
  */

#include "controlecamera.h"
#include <cmath>
#include <QDebug>

ControleCamera::ControleCamera(QObject *parent) : QObject(parent)
{

}

/**
 * @brief Permet de pivoter la caméra vers la gauche, en émettant la trame correspondante. Correspond à la croix directionnelle gauche de la manette.
 * @fn ControleCamera::tourneCameraGauche
 * @param boutonAppuye bool vrai si le bouton est appuyé, faux sinon.
 */
void ControleCamera::tourneCameraGauche(bool boutonAppuye)
{
    QString trame = "$TCA" + QString::number(-boutonAppuye) + "\n";
    emit trameCree(trame);
}

/**
 * @brief Permet de pivoter la caméra vers la droite, en émettant la trame correspondante. Correspond à la croix directionnelle droite de la manette.
 * @fn ControleCamera::tourneCameraDroite
 * @param boutonAppuye bool vrai si le bouton est appuyé, faux sinon.
 */
void ControleCamera::tourneCameraDroite(bool boutonAppuye)
{
    QString trame = "$TCA" + QString::number(boutonAppuye) + "\n";
    emit trameCree(trame);
}

