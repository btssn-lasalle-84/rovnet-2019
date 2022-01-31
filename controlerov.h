/**
* \file controlerov.h
*
* \brief Déclaration de la classe ControleRov. Permet le contrôle des éléments du rov, en reliant la manette aux méthodes de déplacement.
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/

#ifndef CONTROLEROV_H
#define CONTROLEROV_H

#include <QObject>

#include "manette.h"
#include "deplacement.h"
#include "bras.h"
#include "controlecamera.h"
#include "rov.h"

#define MODE_DEPLACEMENT    0
#define MODE_BRAS           1

/**
* \class ControleRov
*
* \brief Déclaration de la classe ControleRov. Permet le contrôle des éléments du rov, en reliant la manette aux méthodes de déplacement.
*
* \author REYNIER Jacques
*
* \version 1.1
*
* \date Jeudi 13 Juin 2019
*
*/

class ControleRov : public QObject
{
    Q_OBJECT

    private:
        Manette *manette; //!< manette utilisée
        Deplacement *deplacement; //!< objet contenant les méthodes utilisées pour le déplacement du rov
        Bras *bras; //!< objet contenant les méthodes utilisées pour les mouvements du bras articulé
        ControleCamera *controleCamera; //!< objet contenant les méthodes utilisées pour les mouvements de la caméra
        Rov *rov; //!< objet Rov contenant l'accès à la communication

        unsigned int mode; //!< Mode de la manette (0 pour déplacement, 1 pour bras)
        /**
         * @brief Permet de modifier les connexions entre la manette et les actions selon le mode de la manette.
         * @fn changeConnexions
         * @param mode int Nouveau mode.
         */
        void changeConnexions(int mode);

    public:
        explicit ControleRov(QObject *parent = nullptr, Rov * rov = nullptr);
        ~ControleRov();

        /**
         * @brief Indique si le Rov est contrôlable, ou non.
         * @fn estControleRovDisponible
         * @return bool Etat de la manette (connectée, ou non).
         */
        bool estControleRovDisponible() const;

    signals:
        void trameCree(QString trame); //!< Signal émis lorsqu'une trame a été crée et est destinée à l'envoi.

    public slots:
        void changeMode(bool appuye); //!< change le mode de la manette (Deplacement / Bras).
};

#endif // CONTROLEROV_H
