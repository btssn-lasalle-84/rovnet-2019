/**
  *
  * @file rov.cpp
  *
  * @brief Définition de la classe Rov. Gestion des liaisons entre les différentes classes, et paramètres de la campagne et de la BDD.
  *
  * @author REYNIER Jacques & Nicolas BOFFREDO
  *
  * @version 1.1
  *
  */

#include "rov.h"
#include "basededonnees.h"

#include <QDebug>

/**
 * @brief Constructeur de la classe Rov.
 * @details Instancie des objets Archives, CommunicationRov, Mesures, et BaseDeDonnees. Se connecte à la BDD, et connecte les trames reçues à l'objet Mesures.
 * @fn Rov::Rov
 * @param parent
 */
Rov::Rov(QObject *parent) : QObject(parent), communicationRov(nullptr), mesures(nullptr), camera(nullptr), listeNomsOperateurs(0), listePrenomsOperateurs(0), archivageActif(0)
{
    qDebug() << Q_FUNC_INFO;
    archives = new Archives(this);
    communicationRov = new CommunicationRov(this);
    mesures = new Mesures(this);
    baseDeDonnees = BaseDeDonnees::getInstance("QSQLITE");

    baseDeDonnees->ouvrir("rovnet.sqlite");
    if(baseDeDonnees->estOuvert())
        qDebug() << Q_FUNC_INFO << "ouverture réussie BD";
    else
        qDebug() << Q_FUNC_INFO << "echec ouverture BD";

    // Connexions
    connect(communicationRov, SIGNAL(trameRecue(QString)), mesures, SLOT(traiteTrame(QString)));

    recupererListeNomsOperateurs();
    recupererListePrenomsOperateurs();
}

/**
 * @brief Destructeur de la classe Rov.
 * @fn Rov::~Rov
 */
Rov::~Rov()
{
    BaseDeDonnees::detruireInstance();
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief Rend l'archivage des mesures actif, ou non.
 * @fn Rov::setArchivageActif
 * @param archivageActif bool vrai si l'archivage des mesures est demandé, sinon faux.
 */
void Rov::setArchivageActif(bool archivageActif)
{
    qDebug() << Q_FUNC_INFO << archivageActif;
    this->archivageActif = archivageActif;
    connecteArchivageMesures(archivageActif);
}

/**
 * @brief Indique si l'archivage des mesures dans la BDD est activé ou non.
 * @fn Rov::getArchivageActif
 * @return bool archivageActif état d'activation de l'archivage des mesures.
 */
bool Rov::getArchivageActif() const
{
    return this->archivageActif;
}

/**
 * @brief Active ou désactive l'archivage des mesures dans la BDD.
 * @fn Rov::connecteArchivageMesures
 * @param archivageActif bool nouvel état d'activation.
 */
void Rov::connecteArchivageMesures(bool archivageActif)
{
    if(archivageActif)
        connect(mesures, SIGNAL(mesuresBDDPrete(double, double)), this, SLOT(stockeMesuresBDD(double, double)));
    else
        disconnect(mesures, SIGNAL(mesuresBDDPrete(double, double)), this, SLOT(stockeMesuresBDD(double, double)));
}

/**
 * @brief Accesseur retournant une instance de la classe CommunicationRov
 * @fn Rov::getCommunicationRov
 * @return une instance de la classe \e CommunicationRov
 */
CommunicationRov * Rov::getCommunicationRov() const
{
    return this->communicationRov;
}

/**
 * @brief Accesseur retournant une instance de la classe Mesures
 * @fn Rov::getMesures
 * @return une instance de la classe \e Mesures
 */
Mesures* Rov::getMesures() const
{
    return mesures;
}

/**
 * @brief Accesseur retournant une instance de la classe Camera
 * @fn Rov::getCamera
 * @return une instance de la classe \e Camera
 */
Camera* Rov::getCamera() const
{
    return camera;
}

/**
 * @brief Accesseur retournant une instance de la classe Archives
 * @fn Rov::getArchives
 * @return une instance de la classe \e Archives
 */
Archives* Rov::getArchives() const
{
    return archives;
}

/**
 * @brief Accesseur affectant une instance de la classe Camera
 * @fn Rov::setCamera
 */
void Rov::setCamera(Camera *camera)
{
    this->camera = camera;
}

/**
 * @brief Accesseur affectant une instance de la classe Archives
 * @fn Rov::setArchives
 */
void Rov::setArchives(Archives *archives)
{
    this->archives = archives;
}

/**
 * @brief Accesseur affectant l'id de la campagne en cours
 * @fn Rov::setIdCampagne
 */
void Rov::setIdCampagne(QString idCampagne)
{
    this->idCampagne = idCampagne;
}

/**
 * @brief Accesseur retournant l'id de la campagne en cours
 * @fn Rov::getIdCampagne
 * @return un \e QString correspondant à l'id de la campagne en cours.
 */
QString Rov::getIdCampagne()
{
    return this->idCampagne;
}

/**
 * @brief Crée une nouvelle campagne
 * @fn Rov::creerNouvelleCampagne
 * @param nom QString
 * @param description QString
 * @param idOperateur QString
 * @return bool
 */
bool Rov::creerNouvelleCampagne(QString nom, QString description, QString idOperateur)
{
    if(communicationRov->estCommunicationRovDisponible())
    {
        if(creerDossiersNouvelleCampagne(nom))
        {
            QString cheminArchives = archives->getCheminArchives();
            QString requete = "INSERT INTO 'campagnes'(nom, description, date, cheminArchives, idOperateur) VALUES ('" + nom + "', '" + description + "', datetime('now', 'localtime'), '" + cheminArchives + "', '" + idOperateur + "')";
            bool retour = baseDeDonnees->executer(requete);
            if(!retour)
            {
                return false;
            }

            QString idCampagne;
            bool requeteRecupIdCampagne = baseDeDonnees->recuperer("SELECT idCampagne FROM campagnes WHERE cheminArchives = '" + cheminArchives + "';", idCampagne);
            if(requeteRecupIdCampagne)
            {
                setIdCampagne(idCampagne);
            }
            else
            {
                return false;
            }
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

/**
 * @brief Stocke les mesures dans la BDD.
 * @fn Rov::stockeMesuresBDD
 * @param temperature double
 * @param irradiation double
 */
void Rov::stockeMesuresBDD(double temperature, double irradiation)
{
    QString requete = "INSERT INTO mesures VALUES(datetime('now', 'localtime'), " + QString::number(temperature) + ", " + QString::number(irradiation) + ", " + idCampagne + ");";
    bool reussi = baseDeDonnees->executer(requete);
    qDebug() << requete;

    if (!reussi)
        qDebug() << Q_FUNC_INFO << "ERREUR ! Echec de l'envoi des mesures dans la BDD !";
}

/**
 * @brief Crée un dossier correspondant au nom de la campagne créée
 * @fn Rov::creerDossiersNouvelleCampagne
 */
bool Rov::creerDossiersNouvelleCampagne(QString nomNouvelleCampagne)
{
    QDir dossierApplication(QApplication::applicationDirPath());
    if(dossierApplication.mkdir(nomNouvelleCampagne))
    {
        QString cheminDossierCampagne = QApplication::applicationDirPath() + "/" + nomNouvelleCampagne;
        qDebug() << Q_FUNC_INFO << cheminDossierCampagne;
        return creerDossierArchives(cheminDossierCampagne);
    }
    return false;
}

/**
 * @brief Crée le dossier des archives correspondant à la campagne créée
 * @fn Rov::creerDossierArchives
 */
bool Rov::creerDossierArchives(QString cheminDossierCampagne)
{
    qDebug() << Q_FUNC_INFO;
    QDir dossierArchives(cheminDossierCampagne);
    if(dossierArchives.mkdir("Archives"))
    {
        QString cheminDossierArchives = cheminDossierCampagne + "/Archives/";
        qDebug() << Q_FUNC_INFO << archives << cheminDossierArchives;
        archives->setCheminArchives(cheminDossierArchives);
        return true;
    }
    return false;
}

/**
 * @brief Méthode permettant de récuperer la liste des noms opérateurs
 * @fn Rov::recupererListeNomsOperateurs
 */
void Rov::recupererListeNomsOperateurs()
{
    QString requete = "SELECT nom FROM operateurs";
    bool reussi = baseDeDonnees->recuperer(requete, listeNomsOperateurs);
    qDebug() << requete << listeNomsOperateurs;

    if(reussi)
        qDebug() << Q_FUNC_INFO << listeNomsOperateurs;

    else
        qDebug() << Q_FUNC_INFO << "erreur SQL" << requete;
}

/**
 * @brief Accesseur retournant la liste des noms des opérateurs
 * @fn Rov::getListeNomsOperateurs
 * @return un \e QVector de \e QString, la liste des noms des opérateurs
 */
QVector<QString> Rov::getListeNomsOperateurs()
{
    return listeNomsOperateurs;
}

/**
 * @brief Méthode permettant de récuperer la liste des prenoms opérateurs
 * @fn Rov::recupererListePrenomsOperateurs
 */
void Rov::recupererListePrenomsOperateurs()
{
    QString requete = "SELECT prenom FROM operateurs";
    bool reussi = baseDeDonnees->recuperer(requete, listePrenomsOperateurs);
    qDebug() << requete << listePrenomsOperateurs;

    if(reussi)
        qDebug() << Q_FUNC_INFO << listePrenomsOperateurs;

    else
        qDebug() << Q_FUNC_INFO << "erreur SQL" << requete;
}

/**
 * @brief Accesseur retournant la liste des noms des opérateurs
 * @fn Rov::getListePrenomsOperateurs
 * @return un \e QVector de \e QString, la liste des noms des opérateurs
 */
QVector<QString> Rov::getListePrenomsOperateurs()
{
    return listePrenomsOperateurs;
}
