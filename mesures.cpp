/**
  *
  * @file mesures.cpp
  *
  * @brief Définition de la classe Mesures.
  *
  * @author REYNIER Jacques & Nicolas BOFFREDO
  *
  * @version 1.1
  *
  */

#include "mesures.h"
#include <QDebug>
#include <QTimer>

/**
 * @brief Constructeur de la classe Mesures.
 * @details Crée un compteur émettant un signal toutes les 30 secondes (modifiable par l'utilisateur), permettant l'envoi des mesures dans la BDD.
 * @fn Mesures::Mesures
 * @param parent QObject*
 */
Mesures::Mesures(QObject *parent) : QObject(parent), temperature(-999.0), irradiation(-999.0), distance(-999.0)
{
    compteur = new QTimer(this);
    connect(compteur, SIGNAL(timeout()), this, SLOT(envoieMesuresBDD()));
    compteur->start(30000);
}

/**
 * @brief Destructeur de la classe Mesures.
 * @fn Mesures::~Mesures
 */
Mesures::~Mesures()
{

}

/**
 * @brief Retourne la température stockée dans l'objet.
 * @fn Mesures::getTemperature
 * @return temperature int Dernière température relevée.
 */
double Mesures::getTemperature() const
{
    return this->temperature;
}

/**
 * @brief Retourne le taux d'irradiation stocké dans l'objet.
 * @fn Mesures::getIrradiation
 * @return irradiation int Dernier taux d'irradiation relevé.
 */
double Mesures::getIrradiation() const
{
    return this->irradiation;
}

/**
 * @brief Renvoie le dernier relevé du capteur de proximité.
 * @fn Mesures::getDistance
 * @return distance double Dernier relevé du capteur de proximité.
 */
double Mesures::getDistance() const
{
    return this->distance;
}

/**
 * @brief Renvoie la fréquence d'archivage des mesures dans la BDD en secondes.
 * @fn Mesures::getFrequenceArchivage
 * @return frequence int Fréquence d'archivage en secondes.
 */
int Mesures::getFrequenceArchivage() const
{
    return (compteur->interval() / 1000);
}

/**
 * @brief Vérifie la validité, et découpe la trame reçue.
 * @fn Mesures::traiteTrame
 * @param trame QString Trame reçue.
 */
void Mesures::traiteTrame(QString trame)    // Exemple de trame : "$T32.5\n"
{
    bool trameValide = true;

    if (trame.startsWith("$") && trame.endsWith("\n"))
    {
        trame.remove(QChar('$'));
        trame.remove(QChar('\n'));

        for(int i = 1; i < trame.length(); i++)     // Parcours la trame et vérifie si le contenu après le caractère de type est bien un chiffre (filtre les erreurs exemple : "$TX4\n").
        {
            if(!trame[i].isDigit() && trame[i] != "." && trame[i] != "-")
                trameValide = false;
        }
    }
    else
        trameValide = false;

    if(trameValide)
    {
        if(trame.startsWith('T'))
            stockeDonnees("temperature", trame.remove('T'));
        else if(trame.startsWith('R'))
            stockeDonnees("irradiation", trame.remove('R'));
        else if(trame.startsWith('D'))
            stockeDonnees("distance", trame.remove('D'));
        else
            trameValide = false;
    }

    if(!trameValide)
        qDebug() << Q_FUNC_INFO << "ERREUR ! Trame invalide";
}

/**
 * @brief Stocke les donnees passés en argument dans l'objet mesures.
 * @fn Mesures::stockeDonnees
 * @param type QString Type de donnée à stocker.
 * @param donnee QString Valeur de la donnée à stocker.
 */
void Mesures::stockeDonnees(QString type, QString donnee)
{
    if (type == "irradiation")
    {
        this->irradiation = donnee.toDouble();
        emit irradiationActualisee(this->irradiation);
    }

    if (type == "temperature")
    {
        this->temperature = donnee.toDouble();
        emit temperatureActualisee(this->temperature);
    }

    if (type == "distance")
    {
        this->distance = donnee.toDouble();
        emit distanceActualisee(this->distance);
    }
}

/**
 * @brief Envoie un signal comprenant les mesures de température et d'irradiation à destination de la BDD, émis toutes les x secondes (fréquence d'archivage).
 * @fn Mesures::envoieMesuresBDD
 */
void Mesures::envoieMesuresBDD()
{
    qDebug() << "YOUSK";
    emit mesuresBDDPrete(this->temperature, this->irradiation);
}

/**
 * @brief Modifie la fréquence
 * @fn Mesures::modifieFrequenceArchivage
 * @param frequence
 */
void Mesures::modifieFrequenceArchivage(int frequence)
{
    compteur->setInterval(frequence * 1000);
}
