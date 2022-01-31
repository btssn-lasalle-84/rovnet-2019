#include "controlerov.h"
#include <QLoggingCategory>
#include <QtGamepad/QGamepad>
#include "deplacement.h"
#include "bras.h"
#include "controlecamera.h"

/**
* \file controlerov.cpp
*
* \brief Définition de la classe ControleRov
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/

ControleRov::ControleRov(QObject *parent, Rov *rov) : QObject(parent), rov(rov), mode(MODE_DEPLACEMENT)
{
    this->bras = new Bras(this);
    this->deplacement = new Deplacement(this);
    this->controleCamera = new ControleCamera(this);
    this->manette = new Manette(this);

    if(manette->estDetecte())
    {
        connect(manette->getManette(), &QGamepad::axisLeftXChanged, deplacement, &Deplacement::tourneDroiteGauche);
        connect(manette->getManette(), &QGamepad::axisLeftYChanged, deplacement, &Deplacement::rouleAvantArriere);
        connect(manette->getManette(), &QGamepad::buttonLeftChanged, controleCamera, &ControleCamera::tourneCameraGauche);
        connect(manette->getManette(), &QGamepad::buttonRightChanged, controleCamera, &ControleCamera::tourneCameraDroite);
        connect(manette->getManette(), &QGamepad::buttonSelectChanged, this, &ControleRov::changeMode);
    }

    // Connexion entre le signal émis d'une trame à envoyer au slot d'envoi de trame de la classe CommunicationRov.
    connect(bras, &Bras::trameCree, this->rov->getCommunicationRov(), &CommunicationRov::envoieTrame);
    // Connexion entre le signal émis d'une trame à envoyer au slot d'envoi de trame de la classe CommunicationRov.
    connect(deplacement, &Deplacement::trameCree, this->rov->getCommunicationRov(), &CommunicationRov::envoieTrame);

    connect(controleCamera, &ControleCamera::trameCree, this->rov->getCommunicationRov(), &CommunicationRov::envoieTrame);

    connect(this, &ControleRov::trameCree, this->rov->getCommunicationRov(), &CommunicationRov::envoieTrame);
}

ControleRov::~ControleRov()
{
}

/**
 * @brief Renvoi l'état de connexion de la manette.
 * @fn ControleRov::estControleRovDisponible
 * @return bool Etat de la manette (connectée, ou non).
 */
bool ControleRov::estControleRovDisponible() const
{
    return manette->estDetecte();
}

/**
 * @brief Permet de changer le mode de la manette, à la réception du signal d'appui de la touche Select.
 * @fn ControleRov::changeMode
 */
void ControleRov::changeMode(bool appuye)
{
    if (appuye == 1 && mode == MODE_DEPLACEMENT)
    {
        qDebug() << Q_FUNC_INFO << "Passage en mode Bras";
        mode = MODE_BRAS;
        emit trameCree("$CBR1\n");
        changeConnexions(MODE_BRAS);
    }
    else if (appuye == 1 && mode == MODE_BRAS)
    {
        qDebug() << Q_FUNC_INFO << "Passage en mode Deplacement";
        mode = MODE_DEPLACEMENT;
        emit trameCree("$CRO1\n");
        changeConnexions(MODE_DEPLACEMENT);
    }
    else if (mode != MODE_DEPLACEMENT && mode != MODE_BRAS)
    {
        qDebug() << Q_FUNC_INFO << "Erreur de mode ! Retour au mode de déplacement";
        mode = MODE_DEPLACEMENT;
        changeConnexions(MODE_DEPLACEMENT);
    }
}

/**
 * @brief Permet de modifier les connexions entre la manette et les actions selon le mode de la manette.
 * @fn ControleRov::changeConnexions
 * @param mode int
 */
void ControleRov::changeConnexions(int mode)
{
    if (mode == MODE_BRAS)
    {
        disconnect(manette->getManette(), &QGamepad::axisLeftXChanged, deplacement, &Deplacement::tourneDroiteGauche);
        disconnect(manette->getManette(), &QGamepad::axisLeftYChanged, deplacement, &Deplacement::rouleAvantArriere);

        connect(manette->getManette(), &QGamepad::axisLeftXChanged, bras, &Bras::tournePoignet);
        connect(manette->getManette(), &QGamepad::axisLeftYChanged, bras, &Bras::levePoignet);

        connect(manette->getManette(), &QGamepad::axisRightXChanged, bras, &Bras::tourneEpaule);
        connect(manette->getManette(), &QGamepad::axisRightYChanged, bras, &Bras::leveEpaule);

        connect(manette->getManette(), &QGamepad::buttonUpChanged, bras, &Bras::leveCoude);
        connect(manette->getManette(), &QGamepad::buttonDownChanged, bras, &Bras::baisseCoude);

        connect(manette->getManette(), &QGamepad::buttonR1Changed, bras, &Bras::serrePince);
        connect(manette->getManette(), &QGamepad::buttonL1Changed, bras, &Bras::lachePince);

        connect(manette->getManette(), &QGamepad::buttonStartChanged, bras, &Bras::depose);
    }
    else if (mode == MODE_DEPLACEMENT)
    {
        disconnect(manette->getManette(), &QGamepad::axisLeftXChanged, bras, &Bras::tournePoignet);
        disconnect(manette->getManette(), &QGamepad::axisLeftYChanged, bras, &Bras::levePoignet);

        disconnect(manette->getManette(), &QGamepad::axisRightXChanged, bras, &Bras::tourneEpaule);
        disconnect(manette->getManette(), &QGamepad::axisRightYChanged, bras, &Bras::leveEpaule);

        disconnect(manette->getManette(), &QGamepad::buttonUpChanged, bras, &Bras::leveCoude);
        disconnect(manette->getManette(), &QGamepad::buttonDownChanged, bras, &Bras::baisseCoude);

        disconnect(manette->getManette(), &QGamepad::buttonR1Changed, bras, &Bras::serrePince);
        disconnect(manette->getManette(), &QGamepad::buttonL1Changed, bras, &Bras::lachePince);

        disconnect(manette->getManette(), &QGamepad::buttonStartChanged, bras, &Bras::depose);

        connect(manette->getManette(), &QGamepad::axisLeftXChanged, deplacement, &Deplacement::tourneDroiteGauche);
        connect(manette->getManette(), &QGamepad::axisLeftYChanged, deplacement, &Deplacement::rouleAvantArriere);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "ERREUR ! mode inconnu";
    }
}
