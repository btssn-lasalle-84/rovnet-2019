/**
  *
  * @file ihmrov.cpp
  *
  * @brief Définition de la classe IHMRov.
  *
  * @author Nicolas BOFFREDO & Jacques REYNIER
  *
  * @version 1.1
  *
  */

#include "ihmrov.h"
#include "camera.h"
#include "controlerov.h"
#include "rov.h"
#include "basededonnees.h"
#include <QMessageBox>

/**
 * @brief Constructeur de la classe IHMRov
 */
IHMRov::IHMRov(QWidget *parent)
    : QDialog(parent)
{
    qDebug() << Q_FUNC_INFO;

    this->rov = new Rov(this);

    this->creerFenetreNouvelleCampagne();
    this->creerFenetreParametres();
    this->creerFenetreDebug();

    camera = new Camera(this);    
    controleRov = new ControleRov(this, rov);
    archives = new Archives(this);

    // met en place les associations
    rov->setCamera(camera);
    rov->setArchives(archives);
    camera->setArchives(archives);

    const int largeurMAX = qApp->desktop()->availableGeometry(this).width();
    const int hauteurMAX = qApp->desktop()->availableGeometry(this).height();
    const int largeurVideo = largeurMAX/4 + largeurMAX/2;
    const int hauteurVideo = hauteurMAX - hauteurMAX/8;

    this->initialiserMenu();

    connect(rov->getMesures(), SIGNAL(irradiationActualisee(double)), this, SLOT(actualiserIrradiation(double)));
    connect(rov->getMesures(), SIGNAL(temperatureActualisee(double)), this, SLOT(actualiserTemperature(double)));
    connect(rov->getMesures(), SIGNAL(distanceActualisee(double)), this, SLOT(actualiserDistance(double)));

    // Créer l'IHM
    // fixer le titre de la fenêtre
    this->setWindowTitle(APPLICATION_TITRE);

    // créer les widgets
    labelCampagneEnCours = new QLabel(QString::fromUtf8("Campagne : aucune"), this);
    labelConnexionRov = new QLabel(QString::fromUtf8("Rov"), this);
    labelEtatConnexionRov = new QLabel(this);
    labelCamera = new QLabel(QString::fromUtf8("Caméra"), this);
    labelEtatCamera = new QLabel(this);
    labelManette = new QLabel(QString::fromUtf8("Manette"), this);
    labelEtatManette = new QLabel(this);
    labelChronometre = new QLabel(this);
    labelCameras = new QLabel(QString::fromUtf8("Caméras disponibles : "), this);
    labelCameras->setAlignment(Qt::AlignRight);
    labelCameraDeconnectee = new QLabel(this);
    widgetEmpilement = new QStackedWidget(this);

    QFont policeLabel;
    policeLabel.setPointSize(18);
    labelChronometre->setFont(policeLabel);

    // créer les widgets du layout des mesures
    labelIconeRadiation = new QLabel("&Radiation (µSv/h)", this);
    labelMesureRadiation = new QLabel("", this);
    barRadiation = new QwtThermo();

    labelIconeTemperature = new QLabel(QString::fromUtf8("&Température (°C)"), this);
    labelMesureTemperature = new QLabel("", this);
    barTemperature = new QwtThermo();

    labelIconeDistance = new QLabel(QString::fromUtf8("&Distance (cm)"), this);
    labelMesureDistance = new QLabel("", this);
    labelIndicationDistance = new QLabel(this);

    // créer les widgets du layout boutons
    listeCamerasDispo = new QComboBox();
    listeCamerasDispo->setLayoutDirection(Qt::RightToLeft);
    listeCamerasDispo->setEditable(true);
    listeCamerasDispo->lineEdit()->setReadOnly(true);
    listeCamerasDispo->lineEdit()->setAlignment(Qt::AlignRight);
    initialiserListeCamera();

    boutonCapture = new QPushButton("&Capturer", this);
    boutonArchives = new QPushButton("&Archives", this);
    boutonQuitter = new QPushButton("&Quitter", this);

    // créer les layout
    QHBoxLayout *hFLayoutPrincipal = new QHBoxLayout;
    QHBoxLayout *hLayoutPrincipal = new QHBoxLayout;    
    QVBoxLayout *vLayoutVideo = new QVBoxLayout;
    QVBoxLayout *vLayoutBoutonsEtMesures = new QVBoxLayout;

    // layout des états
    QVBoxLayout *vLayoutEtats = new QVBoxLayout;
    QHBoxLayout *hLayoutEtatCamera = new QHBoxLayout;
    QHBoxLayout *hLayoutEtatManette = new QHBoxLayout;
    QHBoxLayout *hLayoutEtatRov = new QHBoxLayout;

    // layout boutons
    QVBoxLayout *vLayoutChoixCamera = new QVBoxLayout;
    QVBoxLayout *vLayoutBoutons = new QVBoxLayout;

    // layout des mesures
    QVBoxLayout *vLayoutMesures = new QVBoxLayout;
    QHBoxLayout *hLayoutMesuresRadiation = new QHBoxLayout;
    QHBoxLayout *hLayoutMesuresTemperature = new QHBoxLayout;
    QHBoxLayout *hLayoutMesuresDistance = new QHBoxLayout;
    QVBoxLayout *vLayoutIndicationDistance = new QVBoxLayout;

    // layout menu
    QHBoxLayout *hLayoutMenu = new QHBoxLayout;
    hLayoutMenu->setMenuBar(barreMenu);
    hFLayoutPrincipal->addLayout(hLayoutMenu);

    // positionner les widgets dans les layouts

    // le layout du flux vidéo
    vLayoutVideo->addWidget(labelCampagneEnCours);
    vLayoutVideo->addWidget(widgetEmpilement);

    widgetEmpilement->setFixedWidth(largeurVideo);
    widgetEmpilement->setFixedHeight(hauteurVideo);
    widgetEmpilement->addWidget(labelCameraDeconnectee);
    widgetEmpilement->addWidget(camera->getCadreFluxVideo());
    labelCameraDeconnectee->setPixmap(QPixmap(":/logos/Images/camera_deconnectee.png"));

    vLayoutVideo->addWidget(labelChronometre);

    // le layout principal
    hLayoutPrincipal->setContentsMargins(0, 15, 0, 0);
    hLayoutPrincipal->addLayout(vLayoutVideo);
    hLayoutPrincipal->addLayout(vLayoutBoutonsEtMesures);

    // le layout des boutons et des mesures
    vLayoutBoutonsEtMesures->addLayout(vLayoutMesures);
    vLayoutBoutonsEtMesures->addLayout(vLayoutEtats);
    vLayoutBoutonsEtMesures->addLayout(vLayoutChoixCamera);
    vLayoutBoutonsEtMesures->addLayout(vLayoutBoutons);

    // le layout des mesures
    vLayoutMesures->addLayout(hLayoutMesuresRadiation);
    vLayoutMesures->addLayout(hLayoutMesuresTemperature);
    vLayoutMesures->addLayout(hLayoutMesuresDistance);

    // layout radiation
    hLayoutMesuresRadiation->addStretch();
    hLayoutMesuresRadiation->addWidget(labelIconeRadiation);
    labelIconeRadiation->setPixmap(QPixmap(":/logos/Images/logo_radioactive.png"));
    hLayoutMesuresRadiation->addWidget(labelMesureRadiation);
    labelMesureRadiation->setText("Radiation (µSv/h) : ");
    hLayoutMesuresRadiation->addWidget(barRadiation);
    barRadiation->setOrientation(Qt::Vertical);
    barRadiation->setScale(0, 4);
    barRadiation->setValue(0.0);

    // layout temperature
    hLayoutMesuresTemperature->addStretch();
    hLayoutMesuresTemperature->addWidget(labelIconeTemperature);
    labelIconeTemperature->setPixmap(QPixmap(":/logos/Images/logo_temperature.png"));
    hLayoutMesuresTemperature->addWidget(labelMesureTemperature);
    labelMesureTemperature->setText("Temperature (°C) : ");
    hLayoutMesuresTemperature->addWidget(barTemperature);
    barTemperature->setOrientation(Qt::Vertical);
    barTemperature->setScale(-20,60);
    barTemperature->setValue(0.0);

    // layout distance
    hLayoutMesuresDistance->addStretch();
    hLayoutMesuresDistance->addWidget(labelIconeDistance);
    labelIconeDistance->setPixmap(QPixmap(":/logos/Images/logo_distance.png"));
    hLayoutMesuresDistance->addWidget(labelMesureDistance);
    labelMesureDistance->setText("Distance (cm) : ");
    hLayoutMesuresDistance->addLayout(vLayoutIndicationDistance);
    vLayoutIndicationDistance->addWidget(labelIndicationDistance);
    labelIndicationDistance->setPixmap(QPixmap(":/logos/Images/distance_defaut.png"));

    // le layout des états
    hLayoutEtatCamera->addStretch();
    hLayoutEtatCamera->addWidget(labelCamera);
    hLayoutEtatCamera->addWidget(labelEtatCamera);
    hLayoutEtatManette->addStretch();
    hLayoutEtatManette->addWidget(labelManette);
    hLayoutEtatManette->addWidget(labelEtatManette);
    hLayoutEtatRov->addStretch();
    hLayoutEtatRov->addWidget(labelConnexionRov);
    hLayoutEtatRov->addWidget(labelEtatConnexionRov);
    vLayoutEtats->addLayout(hLayoutEtatManette);
    vLayoutEtats->addLayout(hLayoutEtatCamera);
    vLayoutEtats->addLayout(hLayoutEtatRov);

    // le layout du choix de la caméra
    vLayoutChoixCamera->setAlignment(Qt::AlignRight);
    vLayoutChoixCamera->addWidget(labelCameras);
    vLayoutChoixCamera->addWidget(listeCamerasDispo);

    // le layout des boutons
    vLayoutBoutons->addStretch();
    vLayoutBoutons->addWidget(boutonCapture);
    vLayoutBoutons->addWidget(boutonArchives);
    vLayoutBoutons->addWidget(boutonQuitter);

    hFLayoutPrincipal->addLayout(hLayoutPrincipal);

    setLayout(hFLayoutPrincipal);
    //resize(largeurMAX, hauteurMAX-30);
    setFixedSize(largeurMAX, hauteurMAX-30);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    // connecter signals/slots
    connect(boutonQuitter, SIGNAL(clicked()), this, SLOT(quitter()));
    connect(boutonArchives, SIGNAL(clicked()), archives, SLOT(ouvrirFenetreArchives()));
    connect(boutonCapture, SIGNAL(clicked()), camera, SLOT(capture()));
    connect(listeCamerasDispo, SIGNAL(currentIndexChanged(const QString)), camera, SLOT(changerCamera(QString)));

    this->executerFenetreNouvelleCampagne();
    this->executerFenetreParametres();
    this->initialiserChronometre();

    /*if(!message.isEmpty())
        QMessageBox::critical(nullptr, QString::fromUtf8(APPLICATION_TITRE), message);*/
}

/**
 * @brief Destructeur de la classe IHMRov
 */
IHMRov::~IHMRov()
{
    BaseDeDonnees::detruireInstance();
    qDebug() << Q_FUNC_INFO;
}


/**
 * @brief Met à jour l'icone d'état de la communication dans l'IHM
 * @fn IHMRov::actualiseIconesEtat
 */
void IHMRov::actualiseIconesEtat()
{
    QString message;
    if(rov->getCommunicationRov()->estCommunicationRovDisponible())
    {
        labelEtatConnexionRov->setPixmap(QPixmap(":/logos/Images/on.png"));
    }
    else
    {
        message += QString::fromUtf8("Aucune communication avec le Rov !\n");
        labelEtatConnexionRov->setPixmap(QPixmap(":/logos/Images/off.png"));
    }
    if(camera->estCameraDisponible())
    {
        labelEtatCamera->setPixmap(QPixmap(":/logos/Images/on.png"));
        widgetEmpilement->setCurrentIndex(WIDGET_CAMERA_DISPONIBLE);
    }
    else
    {
        message += QString::fromUtf8("Aucune caméra détectée !\n");
        labelEtatCamera->setPixmap(QPixmap(":/logos/Images/off.png"));
        boutonCapture->setEnabled(false);
        widgetEmpilement->setCurrentIndex(WIDGET_CAMERA_INDISPONIBLE);
    }
    if(controleRov->estControleRovDisponible())
    {
        labelEtatManette->setPixmap(QPixmap(":/logos/Images/on.png"));
    }
    else
    {
        message += QString::fromUtf8("Aucune manette détectée !\n");
        labelEtatManette->setPixmap(QPixmap(":/logos/Images/off.png"));
    }
}

/**
 * @brief Permet de fermer l'application
 * @fn IHMRov::quitter
 */
void IHMRov::quitter()
{
    close();
}

/**
 * @brief Actualise l'affichage du temps chaque seconde et actualise l'état des icones de rov, manette, et camera
 * @fn IHMRov::tic
 */
void IHMRov::tic()
{
    valeurChronometre++;
    labelChronometre->setText(this->getTemps());

    actualiseIconesEtat();
}

/**
 * @brief Ajoute les caméras détectées dans une liste déroulante
 * @fn IHMRov::initialiserListeCamera
 */
void IHMRov::initialiserListeCamera()
{
    QList<QCameraInfo> listeCamera = camera->getListeCamera();
    for(int i = 0; i < listeCamera.count(); i++)
    {
        listeCamerasDispo->addItem(listeCamera[i].description() + " (" + listeCamera[i].deviceName() + ")");
        listeCamerasDispo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        listeCamerasDispo->setFixedWidth(listeCamerasDispo->sizeHint().width()+10);
    }
}

/**
 * @brief Slot permettant de creer une nouvelle campagne.
 * @details Connecté à l'action "Nouvelle Campagne" dans le menu "Campagne"
 * @fn IHMRov::executerFenetreNouvelleCampagne
 */
void IHMRov::executerFenetreNouvelleCampagne()
{
    qDebug() << Q_FUNC_INFO;
    int retour = fenetreNouvelleCampagne->exec();
    qDebug() << Q_FUNC_INFO << retour;
    if(retour == QDialog::Rejected)
    {
    }
}

/**
 * @brief Methode émettant l'ordre d'enregistrer les paramètres des la campagne
 * @fn IHMRov::enregistrerParametresCampagne
 */
void IHMRov::enregistrerParametresCampagne()
{   
    qDebug() << Q_FUNC_INFO;
    QString idOperateur = QString::number(comboBoxListeOperateurs->currentIndex() + 1);

    if(lineEditNomCampagne->text().isEmpty())
    {
        QMessageBox::critical(nullptr, QString::fromUtf8(APPLICATION_TITRE), QString::fromUtf8("Il faut donner un nom à la campagne !"));
        return;
    }
    else if(rov->creerNouvelleCampagne(lineEditNomCampagne->text(), lineEditDescriptionCampagne->text(), idOperateur))
    {
        labelCampagneEnCours->setText("Campagne : " + lineEditNomCampagne->text());
        emit creationCampagne();
    }
    else
    {
        QMessageBox::critical(nullptr, QString::fromUtf8(APPLICATION_TITRE), QString::fromUtf8("Impossible de créer la campagne !"));
        qDebug() << Q_FUNC_INFO << lineEditNomCampagne->text() << lineEditDescriptionCampagne->text() << idOperateur;
        return;
    }
}

/**
 * @brief ouvrir une fenetre informative sur l'application.
 * @fn IHMRov::ouvrirAide
 */
void IHMRov::ouvrirAide()
{
    messageBoxAide = new QMessageBox(this);
    messageBoxAide->setWindowTitle("Aide");
    messageBoxAide->setText(APPLICATION_TITRE);
    messageBoxAide->setInformativeText(APPLICATION_INFORMATIONS);
    messageBoxAide->exec();
}

/**
 * @brief Permet d'actualiser l'affichage de l'irradiation sur l'IHM
 * @details S'actualise à chaque reception de mesure
 * @fn IHMRov::actualiserIrradiation
 * @param irradiation double
 */
void IHMRov::actualiserIrradiation(double irradiation)
{
    labelMesureRadiation->setText("Radiation (µSv/h) : " + QString::number(irradiation));
    barRadiation->setValue(int(irradiation));
}

/**
 * @brief Permet d'actualiser l'affichage de la temperature sur l'IHM
 * @details S'actualise à chaque reception de mesure
 * @fn IHMRov::actualiserTemperature
 * @param temperature double
 */
void IHMRov::actualiserTemperature(double temperature)
{
    labelMesureTemperature->setText("Temperature (°C) : " + QString::number(temperature));
    barTemperature->setValue(int(temperature)); 
}

/**
 * @brief Permet d'actualiser l'affichage de la distance sur l'IHM
 * @details S'actualise à chaque reception de mesure
 * @fn IHMRov::actualiserDistance
 * @param distance double
 */
void IHMRov::actualiserDistance(double distance)
{
    if(distance >= 0)
    {
        labelMesureDistance->setText("Distance (cm) : " + QString::number(distance));
        actualiserIndicationDistance(distance);
    }
    else
    {
        labelMesureDistance->setText("Distance (cm) : < 5");
        actualiserIndicationDistance(distance);
    }

}

/**
 * @brief Permet d'actualiser l'indicateur de la distance sur l'IHM
 * @details S'actualise à chaque reception de mesure
 * @fn IHMRov::actualiserIndicationDistance
 * @param distance double
 */
void IHMRov::actualiserIndicationDistance(double distance)
{
    if (distance > DISTANCE_LOIN)
        labelIndicationDistance->setPixmap(QPixmap(":/logos/Images/distance_loin.png"));

    else if ((distance <= DISTANCE_LOIN) && (distance >= DISTANCE_PROCHE))
        labelIndicationDistance->setPixmap(QPixmap(":/logos/Images/distance_proche.png"));

    else if (distance < DISTANCE_PROCHE)
        labelIndicationDistance->setPixmap(QPixmap(":/logos/Images/distance_danger.png"));
}

/**
 * @brief initialise la barre de menu
 * @fn IHMRov::initialiserMenu
 */
void IHMRov::initialiserMenu()
{
    barreMenu = new QMenuBar(this);
    menuFichier = new QMenu(QString::fromUtf8("Fichier"), barreMenu);
    menuOutils = new QMenu(QString::fromUtf8("Outils"), barreMenu);
    menuAide = new QMenu(QString::fromUtf8("À propos"), barreMenu);

    actionNouvelleCampagne = new QAction(QString::fromUtf8("Nouvelle campagne"), this);
    actionParametre = new QAction(QString::fromUtf8("Parametres"), this);
    actionDebug = new QAction(QString::fromUtf8("Debug"), this);
    actionAide = new QAction(QString::fromUtf8("À propos"), this);

    actionNouvelleCampagne->setShortcuts(QKeySequence::New);
    actionNouvelleCampagne->setStatusTip(QString::fromUtf8("Créer une nouvelle campagne"));
    connect(actionNouvelleCampagne, SIGNAL(triggered()), this, SLOT(executerFenetreNouvelleCampagne()));

    actionParametre->setShortcut(tr("Ctrl+p"));
    connect(actionParametre, SIGNAL(triggered()), this, SLOT(executerFenetreParametres()));

    actionDebug->setShortcut(tr("Ctrl+d"));
    connect(actionDebug, SIGNAL(triggered()), this, SLOT(executerFenetreDebug()));

    actionAide->setShortcut(QKeySequence::HelpContents);
    connect(actionAide, SIGNAL(triggered()), this, SLOT(ouvrirAide()));

    barreMenu->addMenu(menuFichier);
    barreMenu->addMenu(menuOutils);
    barreMenu->addMenu(menuAide);
    menuFichier->addAction(actionNouvelleCampagne);
    menuFichier->addAction(actionParametre);
    menuOutils->addAction(actionDebug);
    menuAide->addAction(actionAide);
}

/**
 * @brief Affiche la fenêtre de Debug.
 * @fn IHMRov::executerFenetreDebug
 */
void IHMRov::executerFenetreDebug()
{
    qDebug() << Q_FUNC_INFO;
    int retour = fenetreDebug->exec();
    qDebug() << Q_FUNC_INFO << retour;
    if(retour == QDialog::Rejected)
    {
    }
}

/**
 * @brief Crée la fenêtre de Debug.
 * @fn IHMRov::creerFenetreDebug
 */
void IHMRov::creerFenetreDebug()
{
    const int largeurFenetreDebug = qApp->desktop()->availableGeometry(this).width() / 3;
    const int hauteurFenetreDebug = qApp->desktop()->availableGeometry(this).height() / 4;

    fenetreDebug = new QDialog();
    boutonEnvoyees = new QPushButton("Envoi");
    boutonRecues = new QPushButton("Reception");
    boutonSQL = new QPushButton("SQL");
    boutonEtats = new QPushButton("Status");
    boutonAnnuler = new QPushButton("&Retour");

    labelDebug = new QLabel("En cours de développement.");
    plainTextEditDebug = new QPlainTextEdit("Test");

    QVBoxLayout *vLayoutFenetreDebug = new QVBoxLayout;
    QHBoxLayout *hLayoutEntete = new QHBoxLayout;
    QVBoxLayout *vLayoutSelection = new QVBoxLayout;
    QHBoxLayout *hLayoutBoutonRetour = new QHBoxLayout;

    fenetreDebug->setWindowTitle("Mode Debug");
    fenetreDebug->setFixedSize(largeurFenetreDebug, hauteurFenetreDebug);

    fenetreDebug->setLayout(vLayoutFenetreDebug);

    vLayoutFenetreDebug->addLayout(hLayoutEntete);
    //vLayoutFenetreDebug->addLayout(vLayoutSelection);
    vLayoutFenetreDebug->addLayout(hLayoutBoutonRetour);

    hLayoutEntete->addWidget(labelDebug);

    vLayoutSelection->addWidget(boutonEnvoyees);
    vLayoutSelection->addWidget(boutonRecues);
    vLayoutSelection->addWidget(boutonSQL);
    vLayoutSelection->addWidget(boutonEtats);

    hLayoutBoutonRetour->addWidget(boutonAnnuler);

    connect(boutonAnnuler, SIGNAL(clicked()), fenetreDebug, SLOT(reject()));
}

/**
 * @brief Méthode permettant de remplir le ComboBox de la liste des opérateurs au démarrage de l'IHM
 * @fn IHMRov::remplirComboBoxListeOperateurs
 */
void IHMRov::remplirComboBoxListeOperateurs()
{
    qDebug() << Q_FUNC_INFO;
    QString operateur;

    for(int i=0; i < rov->getListeNomsOperateurs().size(); i++)
    {
        operateur = rov->getListeNomsOperateurs()[i] + " " + rov->getListePrenomsOperateurs()[i];
        comboBoxListeOperateurs->addItem(operateur);
    }
}

/**
 * @brief Mutateur de l'attribut idOperateur
 * @fn IHMRov::setIdOperateur
 */
void IHMRov::setIdOperateur(int idOperateur)
{
    this->idOperateur = idOperateur;
}

/**
 * @brief Accesseur de l'attribut idOperateur
 * @fn IHMRov::getIdOperateur
 */
int IHMRov::getIdOperateur()
{
    return idOperateur;
}

/**
 * @brief Méthode permettant d'initialiser la fenetre de création d'une nouvelle campagne
 * @fn IHMRov::creerFenetreNouvelleCampagne
 */
void IHMRov::creerFenetreNouvelleCampagne()
{
    const int largeurFenetreNouvelleCampagne = qApp->desktop()->availableGeometry(this).width() / 3;
    const int hauteurFenetreNouvelleCampagne = qApp->desktop()->availableGeometry(this).height() / 4;

    // Les widgets
    fenetreNouvelleCampagne = new QDialog(this);
    boutonValider = new QPushButton(QString::fromUtf8("&Créer"));
    boutonAnnuler = new QPushButton(QString::fromUtf8("&Annuler"));

    // Campagne
    labelCampagne = new QLabel;
    labelNomCampagne = new QLabel;
    lineEditNomCampagne = new QLineEdit;
    labelDescriptionCampagne = new QLabel;
    lineEditDescriptionCampagne = new QLineEdit;

    // Operateur
    labelOperateur = new QLabel;
    comboBoxListeOperateurs = new QComboBox;

    // Les layouts
    QVBoxLayout *vLayoutFenetreNouvelleCampagne = new QVBoxLayout;
    QHBoxLayout *hLayoutFenetreCampagne = new QHBoxLayout;
    QVBoxLayout *vLayoutCampagne = new QVBoxLayout;
    QVBoxLayout *vLayoutOperateur = new QVBoxLayout;
    QHBoxLayout *hLayoutNomCampagne = new QHBoxLayout;
    QHBoxLayout *hLayoutDescriptionCampagne = new QHBoxLayout;
    QHBoxLayout *hLayoutBoutonsCampagne = new QHBoxLayout;

    fenetreNouvelleCampagne->setWindowTitle("Démarrer une nouvelle campagne");
    fenetreNouvelleCampagne->setFixedSize(largeurFenetreNouvelleCampagne, hauteurFenetreNouvelleCampagne);
    fenetreNouvelleCampagne->setLayout(vLayoutFenetreNouvelleCampagne);

    // Campagne
    labelCampagne->setText("Campagne");
    labelNomCampagne->setText("Nom : ");
    labelDescriptionCampagne->setText("Description : ");
    hLayoutNomCampagne->addWidget(labelNomCampagne);
    hLayoutNomCampagne->addWidget(lineEditNomCampagne);
    hLayoutDescriptionCampagne->addWidget(labelDescriptionCampagne);
    hLayoutDescriptionCampagne->addWidget(lineEditDescriptionCampagne);
    vLayoutCampagne->addWidget(labelCampagne);
    vLayoutCampagne->addLayout(hLayoutNomCampagne);
    vLayoutCampagne->addLayout(hLayoutDescriptionCampagne);
    vLayoutCampagne->addStretch();

    // Operateur
    labelOperateur->setText(QString::fromUtf8("Opérateur"));
    vLayoutOperateur->addWidget(labelOperateur);
    vLayoutOperateur->addWidget(comboBoxListeOperateurs);
    vLayoutOperateur->addStretch();
    remplirComboBoxListeOperateurs();

    hLayoutFenetreCampagne->addLayout(vLayoutCampagne);
    hLayoutFenetreCampagne->addSpacing(10);
    hLayoutFenetreCampagne->addLayout(vLayoutOperateur);

    // Boutons
    hLayoutBoutonsCampagne->addWidget(boutonValider);
    hLayoutBoutonsCampagne->addWidget(boutonAnnuler);
    hLayoutBoutonsCampagne->addStretch();

    vLayoutFenetreNouvelleCampagne->addLayout(hLayoutFenetreCampagne);
    vLayoutFenetreNouvelleCampagne->addStretch();
    vLayoutFenetreNouvelleCampagne->addLayout(hLayoutBoutonsCampagne);

    // Les connexions
    connect(boutonValider, SIGNAL(clicked()), this, SLOT(enregistrerParametresCampagne()));
    connect(this, SIGNAL(creationCampagne()), fenetreNouvelleCampagne, SLOT(accept()));
    connect(boutonAnnuler, SIGNAL(clicked()), fenetreNouvelleCampagne, SLOT(reject()));
}

/**
 * @brief Démarre le chronomètre au lancement de l'application.
 * @fn IHMRov::initialiserChronometre
 */
void IHMRov::initialiserChronometre()
{
    this->valeurChronometre = 0;
    QTimer *chronometre = new QTimer(this);
    connect(chronometre, SIGNAL(timeout()), this, SLOT(tic()));

    chronometre->start(PERIODE);
}

/**
 * @brief Retourne les secondes depuis le lancement de l'application
 * @fn IHMRov::getSecondes
 */
long IHMRov::getSecondes()
{
    return valeurChronometre%60;
}

/**
 * @brief Retourne les minutes depuis le lancement de l'application
 * @fn IHMRov::getMinutes
 */
long IHMRov::getMinutes()
{
    return (valeurChronometre%3600)/60;
}

/**
 * @brief Retourne les heures depuis le lancement de l'application
 * @fn IHMRov::getHeure
 */
long IHMRov::getHeure()
{
    return valeurChronometre/3600;
}

/**
 * @brief Crée la fenêtre de Paramètres.
 * @fn IHMRov::creerFenetreParametres
 */
void IHMRov::creerFenetreParametres()
{
    const int largeurFenetreParametres = qApp->desktop()->availableGeometry(this).width() / 3;
    const int hauteurFenetreParametres = qApp->desktop()->availableGeometry(this).height() / 4;

    fenetreParametres = new QDialog();
    boutonValider = new QPushButton("&Valider");
    boutonAnnuler = new QPushButton("&Retour");

    labelArchivageMesures = new QLabel("Archivage Mesures :");
    checkboxArchivage = new QCheckBox;
    checkboxArchivage->setChecked(true);

    labelIntervalArchivage = new QLabel("Période d'archivage des mesures (sec) :");
    sliderIntervalArchivage = new QSlider(Qt::Horizontal);
    sliderIntervalArchivage->setRange(12, 120);
    sliderIntervalArchivage->setSliderPosition(rov->getMesures()->getFrequenceArchivage());
    spinBoxIntervalArchivage = new QSpinBox;
    spinBoxIntervalArchivage->setRange(12, 120);
    spinBoxIntervalArchivage->setValue(rov->getMesures()->getFrequenceArchivage());

    labelAppareils = new QLabel("Port :");
    comboboxAppareils = new QComboBox;
    for(int i = 0; i < 4; i++)
        comboboxAppareils->addItem("/dev/ttyUSB" + QString::number(i));

    comboboxAppareils->addItem("/dev/ttyACM0");
    comboboxAppareils->setCurrentText(rov->getCommunicationRov()->getPort());

    labelBaudRate = new QLabel("Baudrate : ");
    comboboxBaudRate = new QComboBox;
    QStringList listeBaudRate = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};
    comboboxBaudRate->addItems(listeBaudRate);
    comboboxBaudRate->setCurrentText(rov->getCommunicationRov()->getBaudRate());

    // LAYOUTS
    QVBoxLayout *vLayoutFenetreParametres = new QVBoxLayout;
    QHBoxLayout *hLayoutArchivageActif = new QHBoxLayout;
    QHBoxLayout *hLayoutIntervalArchivage = new QHBoxLayout;
    QHBoxLayout *hLayoutAppareils = new QHBoxLayout;
    QHBoxLayout *hLayoutBaudRate = new QHBoxLayout;
    QHBoxLayout *hLayoutValiderAnnuler = new QHBoxLayout;

    fenetreParametres->setWindowTitle("Paramètres de la campagne");
    fenetreParametres->setFixedSize(largeurFenetreParametres, hauteurFenetreParametres);

    fenetreParametres->setLayout(vLayoutFenetreParametres);

    vLayoutFenetreParametres->addLayout(hLayoutArchivageActif);
    vLayoutFenetreParametres->addLayout(hLayoutIntervalArchivage);
    vLayoutFenetreParametres->addLayout(hLayoutAppareils);
    vLayoutFenetreParametres->addLayout(hLayoutBaudRate);
    vLayoutFenetreParametres->addLayout(hLayoutValiderAnnuler);

    // Contenu
    hLayoutArchivageActif->addWidget(labelArchivageMesures);
    hLayoutArchivageActif->addWidget(checkboxArchivage);

    hLayoutIntervalArchivage->addWidget(labelIntervalArchivage);
    hLayoutIntervalArchivage->addWidget(spinBoxIntervalArchivage);
    hLayoutIntervalArchivage->addWidget(sliderIntervalArchivage);

    hLayoutAppareils->addWidget(labelAppareils);
    hLayoutAppareils->addWidget(comboboxAppareils);

    hLayoutBaudRate->addWidget(labelBaudRate);
    hLayoutBaudRate->addWidget(comboboxBaudRate);

    hLayoutValiderAnnuler->addWidget(boutonValider);
    hLayoutValiderAnnuler->addWidget(boutonAnnuler);

    // CONNEXIONS
    connect(spinBoxIntervalArchivage, SIGNAL(valueChanged(int)), sliderIntervalArchivage, SLOT(setValue(int)));
    connect(sliderIntervalArchivage, SIGNAL(valueChanged(int)), spinBoxIntervalArchivage, SLOT(setValue(int)));

    connect(checkboxArchivage, SIGNAL(toggled(bool)), sliderIntervalArchivage, SLOT(setEnabled(bool)));
    connect(checkboxArchivage, SIGNAL(toggled(bool)), spinBoxIntervalArchivage, SLOT(setEnabled(bool)));

    connect(boutonValider, SIGNAL(clicked()), this, SLOT(enregistrerParametres()));
    connect(this, SIGNAL(parametresSauvegardes()), fenetreParametres, SLOT(accept()));
    connect(boutonAnnuler, SIGNAL(clicked()), fenetreParametres, SLOT(reject()));

    connect(this, &IHMRov::nouvelleFrequenceArchivage, rov->getMesures(), &Mesures::modifieFrequenceArchivage);
    connect(this, &IHMRov::nouveauPortCom, rov->getCommunicationRov(), &CommunicationRov::changePortCommunication);
    connect(this, &IHMRov::nouveauBaudRate, rov->getCommunicationRov(), &CommunicationRov::changeBaudRate);
}

/**
 * @brief Affiche la fenêtre Paramètres.
 * @fn IHMRov::executerFenetreParametres
 */
void IHMRov::executerFenetreParametres()
{
    qDebug() << Q_FUNC_INFO;
    int retour = fenetreParametres->exec();
    qDebug() << Q_FUNC_INFO << retour;
    if(retour == QDialog::Rejected)
    {
    }
}

/**
 * @brief Applique les paramètres choisis par l'utilisateur suite à la fenêtre Paramètres.
 * @fn IHMRov::enregistrerParametres
 */
void IHMRov::enregistrerParametres()
{
    qDebug() << Q_FUNC_INFO;

    emit nouveauPortCom(comboboxAppareils->currentText());

    rov->setArchivageActif(checkboxArchivage->isChecked());

    emit nouvelleFrequenceArchivage(sliderIntervalArchivage->value());

    emit nouveauBaudRate(comboboxBaudRate->currentText());

    if(!rov->getCommunicationRov()->estCommunicationRovDisponible())
        QMessageBox::critical(nullptr, QString::fromUtf8(APPLICATION_TITRE), QString::fromUtf8("Impossible de se connecter au port choisi !" ));
    else
    {
        emit parametresSauvegardes();
    }
}

/**
 * @brief IHMRov::getTemps
 * @fn IHMRov::getTemps
 * @return QString le temps formaté "hh:mm:ss"
 */
QString IHMRov::getTemps()
{
    QString heure, minutes, secondes;

   if (getHeure() < 10)
      heure = "0" + QString::number(getHeure());
   else heure = QString::number(getHeure());

   if (getMinutes() < 10)
      minutes = "0" + QString::number(getMinutes());
   else minutes = QString::number(getMinutes());

   if (getSecondes() < 10)
      secondes = "0" + QString::number(getSecondes());
   else secondes = QString::number(getSecondes());

   QString temps = heure + ":" + minutes + ":" + secondes;
   return temps;
}
