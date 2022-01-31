#ifndef CONTROLECAMERA_H
#define CONTROLECAMERA_H

/**
  * @file controlecamera.h
  *
  * @brief Déclaration de la classe ControleCamera.
  *
  * @author Nicolas BOFFREDO
  *
  * @version 1.1
  *
  * @date Jeudi 13 Juin 2019
  *
  */

#include <QObject>

/**
  * @class ControleCamera
  *
  * @brief Déclaration de la classe ControleCamera.
  *
  * @author Nicolas BOFFREDO
  *
  * @version 1.1
  *
  * @date Jeudi 13 Juin 2019
  *
  */
class ControleCamera : public QObject
{
    Q_OBJECT
public:
    explicit ControleCamera(QObject *parent = nullptr);

private:

signals:
    void trameCree(QString trame); //!< Signal émis lorsqu'une trame a été crée et prête à être transmise.

public slots:
    /**
     * @brief Permet de pivoter la caméra vers la gauche, en émettant la trame correspondante. Correspond à la croix directionnelle gauche de la manette.
     * @fn tourneCameraGauche
     * @param boutonAppuye bool vrai si le bouton est appuyé, faux sinon.
     */
    void tourneCameraGauche(bool boutonAppuye);
    /**
     * @brief Permet de pivoter la caméra vers la droite, en émettant la trame correspondante. Correspond à la croix directionnelle droite de la manette.
     * @fn tourneCameraDroite
     * @param boutonAppuye bool vrai si le bouton est appuyé, faux sinon.
     */
    void tourneCameraDroite(bool boutonAppuye);
};

#endif // CONTROLECAMERA_H
