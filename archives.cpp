/**
  *
  * @file archives.cpp
  *
  * @brief Définition de la classe Archives.
  *
  * @author Nicolas BOFFREDO
  *
  * @version 1.1
  *
  */

#include "archives.h"
#include <QDebug>

/**
 * @fn Archives::Archives
 * @details Constructeur de la classe Archives
 * @param parent QObject*
 */
Archives::Archives(QObject *parent) : QObject(parent), cheminDossierArchives(" "), modeleArchives(nullptr)
{
    initialiserFenetreArchives();
}

/**
 * @fn Archives::~Archives
 * @details Destructeur de la classe Archives
 */
Archives::~Archives()
{
}

/**
 * @fn Archives::getCheminArchives
 * @brief Accesseur renvoyant le chemin du dossier de stockage des photos.
 * @return Un \e QString indiquant le chemin du dossier de stockage des photos.
 */
QString Archives::getCheminArchives()
{
    qDebug() << Q_FUNC_INFO;
    return cheminDossierArchives;
}

/**
 * @fn Archives::setCheminArchives
 * @brief Accesseur permettant de modifier le chemin vers les Archives.
 * @param nouveauCheminArchives QString le nouveau chemin des Archives.
 */
void Archives::setCheminArchives(QString nouveauCheminArchives)
{
    qDebug() << Q_FUNC_INFO << nouveauCheminArchives;
    cheminDossierArchives = nouveauCheminArchives;
}

/**
 * @fn Archives::getImage
 * @brief Renvoie l'image des archives
 * @param indexArchives QModelIndex & index sur le modèle des fichiers contenus dans le QListView
 * @return un \e QString indiquant le nom de l'image
 */
QString Archives::getImage(const QModelIndex &indexArchives)
{
    QString cheminImage = getCheminArchives() + indexArchives.data().toString();
    qDebug() << Q_FUNC_INFO << "cheminImage" << cheminImage;
    return cheminImage;
}

/**
 * @fn Archives::getModeleArchives
 * @brief Accesseur renvoyant le modèle de données.
 * @return Un \e QFileSystemModel* indiquant le modèle de données.
 */
QFileSystemModel *Archives::getModeleArchives()
{
    return modeleArchives;
}

/**
 * @fn Archives::getIndexArchives
 * @brief Accesseur renvoyant l'index du modèle de données.
 * @return Un \e QModelIndex indiquant l'index du modèle de données.
 */
QModelIndex Archives::getIndexArchives()
{
    return indexArchives;
}

/**
 * @fn Archives::getDateImage
 * @brief Retourne la date de prise de l'image sélectionnée
 * @param indexArchives const QModelIndex &, L'index de l'image des archives
 * @return Un \e QString indiquant la date de prise de l'image sélectionnée
 */
QString Archives::getDateImage(const QModelIndex &indexArchives)
{
    QString dateImage = indexArchives.data().toString().left(10);
    QStringList mois;

    dateImage.replace(2, 1, " ");
    dateImage.replace(5, 1, " ");

    mois << QString::fromUtf8("Janvier") << QString::fromUtf8("Février") << QString::fromUtf8("Mars") << QString::fromUtf8("Avril") << QString::fromUtf8("Mai") << QString::fromUtf8("Juin") << QString::fromUtf8("Juillet") << QString::fromUtf8("Août") << QString::fromUtf8("Septembre") << QString::fromUtf8("Octobre") << QString::fromUtf8("Novembre") << QString::fromUtf8("Décembre");
    dateImage.replace(3, 2, mois.at(dateImage.left(5).right(3).toInt() - 1));

    return dateImage;
}

/**
 * @fn Archives::getHeureImage
 * @brief Retourne l'heure de prise de l'image sélectionnée
 * @param indexArchives const QModelIndex &, L'index de l'images des archives
 * @return Un \e QString indiquant l'heure de prise de l'image sélectionnée
 */
QString Archives::getHeureImage(const QModelIndex &indexArchives)
{
    QString heureImage = indexArchives.data().toString().remove(19, 4).right(8);
    heureImage.replace(2, 1, "h ");
    heureImage.replace(6, 1, "m ");
    heureImage.append('s');

    return heureImage;
}

/**
 * @fn Archives::initialiserFenetreArchives
 * @brief Initialise la fenetre pour naviguer dans les archives.
 * @details La taille de la fenetre est fixer en fonction de la resolution des photos prises.
 */
void Archives::initialiserFenetreArchives()
{
    modeleArchives = new QFileSystemModel;
    modeleArchives->setRootPath(getCheminArchives());
    indexArchives = modeleArchives->index(getCheminArchives());

    fenetreArchives = new QDialog();
    vueArchives = new QListView;
    labelImage = new QLabel;
    labelImageDate = new QLabel;
    labelImageHeure = new QLabel;
    boutonFermerArchives = new QPushButton("&Fermer");

    const int hauteurImage = 480;
    const int largeurImage = 640;
    const int hauteurInformations = (hauteurImage/10);
    const int largeurMAX = largeurImage + largeurImage/2;
    const int hauteurMAX = hauteurImage + hauteurInformations;

    vueArchives->setModel(this->getModeleArchives());
    vueArchives->setRootIndex(this->getIndexArchives());

    QHBoxLayout *hLayoutPrincipalArchives = new QHBoxLayout;
    QVBoxLayout *vLayoutSelections = new QVBoxLayout;
    QVBoxLayout *vLayoutImage = new QVBoxLayout;
    QHBoxLayout *hLayoutInformationsImage = new QHBoxLayout;

    hLayoutPrincipalArchives->addLayout(vLayoutImage);
    hLayoutPrincipalArchives->addLayout(vLayoutSelections);
    vLayoutSelections->addWidget(vueArchives);
    vLayoutSelections->addWidget(boutonFermerArchives);
    vLayoutImage->addLayout(hLayoutInformationsImage);
    vLayoutImage->addWidget(labelImage);
    hLayoutInformationsImage->addWidget(labelImageDate);
    hLayoutInformationsImage->addWidget(labelImageHeure);

    fenetreArchives->setWindowTitle("Archives Photo");
    fenetreArchives->setFixedSize(largeurMAX, hauteurMAX);
    fenetreArchives->setLayout(hLayoutPrincipalArchives);
    labelImage->setFixedHeight(hauteurImage);
    labelImage->setFixedWidth(largeurImage);
    labelImageDate->setFixedHeight(hauteurInformations);
    labelImageHeure->setFixedHeight(hauteurInformations);

    connect(vueArchives, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(afficherImage(const QModelIndex)));
    connect(boutonFermerArchives, SIGNAL(clicked()), this, SLOT(fermerArchives()));

    fenetreArchives->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

/**
 * @fn Archives::actualiserVueArchives
 * @brief Actualise la vue des archives
 * @details Est appellée à chaque fois que la fenêtre de navigation dans les archives est ouverte.
 */
void Archives::actualiserVueArchives()
{
    modeleArchives->setRootPath(getCheminArchives());
    indexArchives = modeleArchives->index(getCheminArchives());

    vueArchives->setModel(this->getModeleArchives());
    vueArchives->setRootIndex(this->getIndexArchives());
}

/**
 * @fn Archives::ouvrirFenetreArchives
 * @brief Ouvre une fenetre menant aux archives des captures d'écrans.
 */
void Archives::ouvrirFenetreArchives()
{
    actualiserVueArchives();
    fenetreArchives->exec();
}

/**
 * @fn Archives::afficherImage
 * @brief Affiche l'image séléctionnée et les informations correspondantes
 * @param indexArchives QModelIndex
 */
void Archives::afficherImage(const QModelIndex &indexArchives)
{
    labelImage->setPixmap(QPixmap(this->getImage(indexArchives)));
    labelImage->setScaledContents(true);
    labelImageDate->setText("Date : " + this->getDateImage(indexArchives));
    labelImageHeure->setText("Heure : " + this->getHeureImage(indexArchives));
}

/**
 * @fn Archives::fermerArchives
 * @brief Ferme la fenetre des archives
 */
void Archives::fermerArchives()
{
    fenetreArchives->close();
}
