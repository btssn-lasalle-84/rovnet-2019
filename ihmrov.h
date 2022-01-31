#ifndef IHMROV_H
#define IHMROV_H

/**
  * @file ihmrov.h
  *
  * @brief Déclaration de la classe Rov.
  *
  * @author Nicolas BOFFREDO & Jacques REYNIER
  *
  * @version 1.1
  *
  * @date Jeudi 13 Juin 2019
  *
  */

#include "camera.h"
#include "controlerov.h"
#include "rov.h"

#include <QtWidgets>
#include <QComboBox>
#include <QDialog>
#include <QTimer>
#include <QApplication>
#include <QListView>
#include <QFileSystemModel>
#include <QStackedWidget>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <qwt_thermo.h>

#include <QMainWindow>

#define APPLICATION_TITRE           "Projet Rov'net - BTS SN IR 2019 (E6.2)"
#define APPLICATION_INFORMATIONS    "Version : 1.0 \nJacques Reynier & Boffredo Nicolas"
#define PERIODE                     1000
#define WIDGET_CAMERA_INDISPONIBLE  0
#define WIDGET_CAMERA_DISPONIBLE    1
#define DISTANCE_LOIN               40
#define DISTANCE_PROCHE             25

class BaseDeDonnees;
class QAction;
class QActionGroup;
class QMenu;

/**
  * @class IHMRov
  *
  * @brief Déclaration de la classe Rov.
  *
  * @author Nicolas BOFFREDO & Jacques REYNIER
  *
  * @version 1.1
  *
  * @date Jeudi 13 Juin 2019
  *
  */

class IHMRov : public QDialog
{
    Q_OBJECT

private:
    // objets principaux
    Camera *camera; //!< association vers la caméra
    ControleRov *controleRov; //!< association du contrôle du Rov
    Archives *archives; //!< association vers Archives
    Rov *rov; //!< association vers le Rov
    QTimer *chronometre; //!< timer pour chronomètrer la campagne

    // attributs principaux
    int idOperateur; //!< id de l'opérateur de la campagne encours
    QString nomNouvelleCampagne; //!< nom de la campagne en cours
    long valeurChronometre; //!< temps

    // widgets
    QLabel *labelCampagneEnCours;
    QLabel *labelCamera;
    QLabel *labelEtatCamera;
    QLabel *labelManette;
    QLabel *labelEtatManette;
    QLabel *labelConnexionRov;
    QLabel *labelEtatConnexionRov;
    QLabel *labelChronometre;
    QLabel *labelCameras;
    QLabel *labelCameraDeconnectee;
    QStackedWidget *widgetEmpilement;

    // widgets info radiation
    QLabel *labelIconeRadiation;
    QLabel *labelMesureRadiation;
    QwtThermo *barRadiation;

    // widgets info temperature
    QLabel *labelIconeTemperature;
    QLabel *labelMesureTemperature;
    QwtThermo *barTemperature;

    // widgets info distance
    QLabel *labelIconeDistance;
    QLabel *labelMesureDistance;
    QLabel *labelIndicationDistance;

    // widgets boutons
    QComboBox *listeCamerasDispo;
    QPushButton *boutonQuitter;
    QPushButton *boutonArchives;
    QPushButton *boutonCapture;

    // menu
    QMenuBar *barreMenu;
    QMenu *menuFichier;
    QMenu *menuAide;
    QMenu *menuOutils;
    QAction *actionNouvelleCampagne;
    QAction *actionParametre;
    QAction *actionDebug;
    QAction *actionAide;

    // fenetre - debug
    QDialog *fenetreDebug;

    QLabel *labelDebug;
    QPlainTextEdit *plainTextEditDebug;

    QPushButton *boutonEnvoyees;
    QPushButton *boutonRecues;
    QPushButton *boutonSQL;
    QPushButton *boutonEtats;

    // fenetre - parametres
    QDialog *fenetreParametres;

    // Parametres
    QLabel *labelArchivageMesures;
    QCheckBox *checkboxArchivage;

    QLabel *labelIntervalArchivage;
    QSlider *sliderIntervalArchivage;
    QSpinBox *spinBoxIntervalArchivage;

    QLabel *labelAppareils;
    QComboBox *comboboxAppareils;

    QLabel *labelBaudRate;
    QComboBox *comboboxBaudRate;

    // fenetre creerNouvelleCampagne
    QDialog *fenetreNouvelleCampagne;
    QDir *dossierNouvelleCampagne;

    // boutons
    QPushButton *boutonValider;
    QPushButton *boutonAnnuler;

    // campagne
    QLabel *labelCampagne;
    QLabel *labelNomCampagne;
    QLineEdit *lineEditNomCampagne;
    QLabel *labelDescriptionCampagne;
    QLineEdit *lineEditDescriptionCampagne;

    // operateur
    QLabel *labelOperateur;
    QComboBox *comboBoxListeOperateurs;

    // divers
    QMessageBox *messageBoxAide;

    void initialiserMenu();
    void remplirComboBoxListeOperateurs();
    void setIdOperateur(int idOperateur);
    int getIdOperateur();
    void creerFenetreNouvelleCampagne();
    void creerFenetreParametres();
    void creerFenetreDebug();
    void initialiserChronometre();
    long getSecondes();
    long getMinutes();
    long getHeure();
    QString getTemps();
    void actualiseIconesEtat();
    void actualiserIndicationDistance(double distance);

public:
    IHMRov(QWidget *parent = nullptr);
    ~IHMRov();

signals:
    void creationCampagne();
    void nouvelleFrequenceArchivage(int);
    void nouveauPortCom(QString);
    void nouveauBaudRate(QString);
    void parametresSauvegardes();

public slots:
    void quitter();
    void tic();
    void initialiserListeCamera();
    void executerFenetreNouvelleCampagne();
    void executerFenetreParametres();
    void executerFenetreDebug();
    void ouvrirAide();
    void actualiserIrradiation(double irradiation);
    void actualiserTemperature(double temperature);
    void actualiserDistance(double distance);
    void enregistrerParametresCampagne();
    void enregistrerParametres();
};

#endif // IHMROV_H
