#ifndef ARCHIVES_H
#define ARCHIVES_H

/**
  * @file archives.h
  *
  * @brief Déclaration de la classe Archives.
  *
  * @author Nicolas BOFFREDO
  *
  * @version 1.1
  *
  * @date Mercredi 12 Juin 2019
  */

#include <QApplication>
#include <QObject>
#include <QDialog>
#include <QDir>
#include <QtWidgets>
#include <QListView>
#include <QFileSystemModel>

#define LARGEUR_IMAGE 640
#define HAUTEUR_IMAGE 480

#define HAUTEUR_INFORMATIONS HAUTEUR_IMAGE/2
#define LARGEUR_VUE_ARCHIVES LARGEUR_IMAGE/2

#define LARGEUR_MAX LARGEUR_IMAGE + LARGEUR_ARCHIVES
#define HAUTEUR_MAX HAUTEUR_IMAGE + HAUTEUR_INFORMATIONS

/**
  *
  * @class Archives
  *
  * @brief Déclaration de la classe Archives.
  *
  * @author Nicolas BOFFREDO
  *
  * @version 1.1
  *
  * @date Mercredi 12 Juin 2019
  */
class Archives : public QObject
{
    Q_OBJECT
public:
    explicit Archives(QObject *parent = nullptr);
    ~Archives();

    QString getImage(const QModelIndex &indexArchives);
    QString getDateImage(const QModelIndex &indexArchives);
    QString getHeureImage(const QModelIndex &indexArchives);
    QFileSystemModel *getModeleArchives();
    QModelIndex getIndexArchives();
    QString getCheminArchives();
    void setCheminArchives(QString nouveauCheminArchives);
    void actualiserVueArchives();

private:
    QString cheminDossierArchives; //!< le chemin du dossier de stockage des photos
    QFileSystemModel *modeleArchives; //!< le modèle de données
    QModelIndex indexArchives; //!< l'index du modèle de données
    QDialog *fenetreArchives; //!< fenetre pour naviguer dans les archives
    QListView *vueArchives; //!< la vue de la liste des archives
    bool estFenetreArchivesOuverte; //!< booléen indiquant si la fenêtre est ouverte

    // attributs fenetre archives
    QLabel *labelImage; //!< label pour l'affichage de la photo sélectionnée
    QLabel *labelImageDate; //!< label pour l'affichage de la date de la photo sélectionnée
    QLabel *labelImageHeure; //!< label pour l'affichage de l'heure de la photo sélectionnée
    QPushButton *boutonFermerArchives; //!< bouton de fermeture de la fenêtre

    void initialiserFenetreArchives();

signals:

public slots:
    void fermerArchives();
    void ouvrirFenetreArchives();
    void afficherImage(const QModelIndex &indexArchives);
};

#endif // ARCHIVES_H
