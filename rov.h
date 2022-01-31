#ifndef ROV_H
#define ROV_H

/**
  * \file rov.h
  *
  * \brief Déclaration de la classe Rov. Gestion des liaisons entre les différentes classes, et paramètres de la campagne et de la BDD.
  *
  * \author REYNIER Jacques & Nicolas BOFFREDO
  *
  * \version 1.1
  *
  * \date Jeudi 13 Juin 2019
  *
  */

#include <QObject>

#include "communicationrov.h"
#include "mesures.h"
#include "camera.h"
#include "archives.h"

class BaseDeDonnees;

/**
  * \class Rov
  *
  * \brief Déclaration de la classe Rov. Gestion des liaisons entre les différentes classes, et paramètres de la campagne et de la BDD.
  *
  * \author REYNIER Jacques & Nicolas BOFFREDO
  *
  * \version 1.1
  *
  * \date Jeudi 13 Juin 2019
  *
  */
class Rov : public QObject
{
    Q_OBJECT

    private:
        CommunicationRov * communicationRov;  //!< Communication via le port série avec le rov.
        Mesures *mesures;                     //!< Les mesures des capteurs.
        Camera *camera;                       //!< La caméra.
        Archives *archives;                   //!< Les archives.
        BaseDeDonnees *baseDeDonnees;         //!< La base de données.

        QString idCampagne;                      //!< Numéro d'id de la campagne en cours.
        QVector<QString> listeNomsOperateurs;    //!< Liste des noms des opérateurs.
        QVector<QString> listePrenomsOperateurs; //!< Liste des prenoms des opérateurs.
        bool archivageActif;                     //!< L'archivage des mesures est demandé.

        bool creerDossierArchives(QString cheminDossierCampagne);
        void recupererListeNomsOperateurs();
        void recupererListePrenomsOperateurs();

    public:
        explicit Rov(QObject *parent = nullptr); //!< Constructeur de la classe Rov.
        ~Rov();                                  //!< Destructeur de la classe Rov.

        CommunicationRov * getCommunicationRov() const; //!< Renvoie l'objet réalisant la connexion entre le rov et le programme.
        Mesures* getMesures() const;                    //!< Renvoie l'objet contenant les mesures et les méthodes affiliées.
        Camera* getCamera() const;                      //!< Renvoie l'objet Camera.

        QVector<QString> getListeNomsOperateurs();
        QVector<QString> getListePrenomsOperateurs();

        void setCamera(Camera *camera);

        Archives* getArchives() const;
        void setArchives(Archives *archives);

        void setIdCampagne(QString idCampagne); //!< Remplace l'idCampagne
        QString getIdCampagne(); //!< Renvoie idCampagne

        bool creerNouvelleCampagne(QString nom, QString description, QString idOperateur);
        bool creerDossiersNouvelleCampagne(QString nomNouvelleCampagne);

        void setArchivageActif(bool);   //!< Modifie la valeur de archivageActif.
        bool getArchivageActif() const; //!< Retourne l'état de archivageActif.

    signals:

    public slots:        
        void stockeMesuresBDD(double temperature, double irradiation);  //!< Stocke les mesures dans la table mesures.
        void connecteArchivageMesures(bool archivageActif);             //!< Active, ou désactive l'archivage des mesures dans la BDD.
};

#endif // ROV_H
