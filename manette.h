#ifndef MANETTE_H
#define MANETTE_H

/**
  * \file manette.h
  *
  * \brief Déclaration de la classe Manette. Liaison avec la manette, permettant d'en recevoir les signaux.
  *
  * \author REYNIER Jacques & Nicolas BOFFREDO
  *
  * \version 1.1
  *
  * \date Jeudi 13 Juin 2019
  *
  */

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "rov.h"
#include "deplacement.h"
#include "bras.h"

QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

//#define DEBUG_MANETTE

/**
  * \class Manette
  *
  * \brief Déclaration de la classe Manette. Liaison avec la manette, permettant d'en recevoir les signaux.
  *
  * \author REYNIER Jacques & Nicolas BOFFREDO
  *
  * \version 1.1
  *
  * \date Jeudi 13 Juin 2019
  *
  */

class Manette : public QObject
{
    Q_OBJECT

    private:
        QGamepad *manette;  //!< Contient la manette actuellement connectée.
        bool detecte;       //!< Manette connectée, ou non.

    public:
        explicit Manette(QObject *parent = nullptr);
        ~Manette();
        /**
         * @brief estDetecte
         * @return detecte bool Etat de l'attribut detecte de l'objet.
         */
        bool estDetecte() const;
        /**
         * @brief getManette
         * @return manette QGamepad Manette actuellement connectée.
         */
        QGamepad * getManette();
};

#endif // MANETTE_H
