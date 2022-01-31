/**
  *
  * @file camera.cpp
  *
  * @brief Définition de la classe Camera.
  *
  * @author Nicolas BOFFREDO
  *
  * @version 1.1
  *
  */

#include "camera.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <QCameraViewfinder>
#include <QDebug>

Camera::Camera(QObject *parent) : QObject(parent),
  camera(nullptr),
  archives(nullptr),
  cadreFluxVideo(nullptr),
  captureImage(nullptr),
  formatSauvegardeTemps("dd-MM-yyyy_HH:mm:ss")
{
    cadreFluxVideo = new QCameraViewfinder;
    camera = new QCamera;
    captureImage = new QCameraImageCapture(camera);
    cameras = QCameraInfo::availableCameras();

    if(estCameraDisponible())
        demarrerCamera(cameras[0]);

    qDebug() << Q_FUNC_INFO << cameras;
}

Camera::~Camera()
{
    delete cadreFluxVideo;
    delete camera;
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief Retourne un booléen sur l'état de disponibilité de la caméra.
 * @return Un @e booléen, vrai si la caméra est disponible, faux sinon.
 */
bool Camera::estCameraDisponible()
{
    if (cameras.count() > 0)
        return true;

    else
        return false;
}

/** @brief retourne toute les caméras disponible
 */
QList<QCameraInfo> Camera::getListeCamera()
{
    QList<QCameraInfo> listeCamera;
    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        listeCamera.append(cameraInfo);
    }
    return listeCamera;
}

void Camera::setArchives(Archives *archives)
{
    this->archives = archives;
}

/**
 * @brief retourne le flux video
 * @details Un assesseur permettant d'avoir le retour vidéo
 */
QCameraViewfinder *Camera::getCadreFluxVideo()
{
    return cadreFluxVideo;
}

/**
 * @brief Demarre le retour vidéo sur l'IHM
 * @details Par défaut la méthode reçoit la première caméra trouvée
 * @param cameraSelectionnee
 */
void Camera::demarrerCamera(QCameraInfo cameraSelectionnee)
{
    if(camera != nullptr)
        delete camera;
    if(captureImage != nullptr)
        delete captureImage;

    if(estCameraDisponible())
    {
        qDebug() << Q_FUNC_INFO << "cameraSelectionnee" << cameraSelectionnee.deviceName();
        camera = new QCamera(cameraSelectionnee);
        camera->setViewfinder(cadreFluxVideo);
        camera->setCaptureMode(QCamera::CaptureStillImage);
        captureImage = new QCameraImageCapture(camera);
        captureImage->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
        connect(captureImage, SIGNAL(error(int,QCameraImageCapture::Error,QString)), this, SLOT(erreurCapture(int,QCameraImageCapture::Error,QString)));

        camera->start();
    }
}

/**
 * @brief Capture l'image du flux video
 */
void Camera::capture()
{
    qDebug() << Q_FUNC_INFO;
    QString nomCapture = this->nommerCapture();
    qDebug() << Q_FUNC_INFO << "nomCapture" << nomCapture;
    captureImage->capture(nomCapture);
}

/**
 * @brief Renomme la photo capturée au format : "yyyy-MM-dd_HH:mm:ss".
 * @details Indique le chemin vers un dossier de stockage des photos, à l'emplacement de l'éxécutable.
 */
QString Camera::nommerCapture()
{
    QString nom = QApplication::applicationDirPath() + "/defaut";
    QDateTime dateCapture = QDateTime::currentDateTime();
    dateImage = dateCapture.toString(formatSauvegardeTemps);
    nom = archives->getCheminArchives() + "/" + dateImage;
    qDebug() << Q_FUNC_INFO << nom;
    return nom;
}

/**
 * @brief Permet de démarrer le flux vidéo de la caméra choisir dans le QComboBox sur l'IHM
 * @param nomCamera Un \e QString, le nom de la caméra
 */
void Camera::changerCamera(QString nomCamera)
{
    QString cameraTrouvee;
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        cameraTrouvee = cameraInfo.description() + " (" + cameraInfo.deviceName()+ ")";
        if (cameraTrouvee == nomCamera)
        {
            this->demarrerCamera(cameraInfo);
        }
    }
}

void Camera::erreurCapture(int id, QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id)
    Q_UNUSED(error)
    qDebug() << Q_FUNC_INFO << errorString;
}
