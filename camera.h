#ifndef CAMERA_H
#define CAMERA_H

/**
  * @file camera.h
  *
  * @brief Déclaration de la classe Camera.
  *
  * @author Nicolas BOFFREDO
  *
  * @version 1.1
  *
  * @date Jeudi 13 Juin 2019
  *
  */

#include "archives.h"

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>

#define LARGEUR_RESOLUTION_IMAGE 1280
#define HAUTEUR_RESOLUTION_IMAGE 720

/**
  *
  * @class Camera
  *
  * @brief Déclaration de la classe Camera.
  *
  * @author Nicolas BOFFREDO
  *
  * @version 1.1
  *
  * @date Jeudi 13 Juin 2019
  *
  */
class Camera : public QObject
{
    Q_OBJECT

public:
    Camera(QObject *parent = nullptr);
    ~Camera();

    bool estCameraDisponible();
    QCameraViewfinder *getCadreFluxVideo();
    QList<QCameraInfo> getListeCamera();
    void setArchives(Archives *archives);

private:
    QCamera *camera; //!< Permet la connexion avec la caméra
    Archives *archives; //!< Permet la connexion avec les archives
    QCameraViewfinder *cadreFluxVideo; //!< Permet l'affichage du flux vidéo
    QCameraImageCapture *captureImage; //!< Permet la capture d'image
    QString formatSauvegardeTemps; //!< Le format de sauvegarde du temps pour l'archivages
    QString dateImage; //!< Stock la date de la prise de photo pour l'archivage
    QList<QCameraInfo> cameras; //!< Stock la liste des caméras disponibles

    QString nommerCapture();
    void demarrerCamera();
    void demarrerCamera(QCameraInfo);

signals:

public slots:
    void capture();
    void changerCamera(QString);
    void erreurCapture(int id, QCameraImageCapture::Error error, const QString &errorString);
};

#endif // CAMERA_H
