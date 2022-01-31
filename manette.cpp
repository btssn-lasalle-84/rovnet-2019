/**
  *
  * @file manette.cpp
  *
  * @brief Définition de la classe Manette.
  *
  * @author REYNIER Jacques
  *
  * @version 1.1
  *
  */

#include "manette.h"
#include "deplacement.h"
#include "communicationrov.h"

#include <QLoggingCategory>
#include <QtGamepad/QGamepad>
#include <QDebug>

/**
 * @brief Constructeur de la classe GestionnaireManette. La connexion entre les évènements de la manette et les méthodes cibles sont réalisées ici.
 * @param parent
 */
Manette::Manette(QObject *parent) : QObject(parent), manette(nullptr), detecte(false)
{
    #ifndef QT_NO_DEBUG_OUTPUT
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gamepad.debug=true"));
    #endif

    auto manettes = QGamepadManager::instance()->connectedGamepads();

    if (manettes.isEmpty())
    {
        qDebug() << Q_FUNC_INFO << "Aucune manette détectée !";
        detecte = false;
    }
    else
    {
        manette = new QGamepad(*manettes.begin(), this);
        detecte = true;
    }
}

Manette::~Manette()
{

}

/**
 * @brief Renvoie l'état de la manette (connectée, ou non).
 * @fn Manette::estDetecte
 * @return detecte bool Indique si une manette est connectée ou non.
 */
bool Manette::estDetecte() const
{
    return detecte;
}

/**
 * @brief Renvoie la manette actuellement connectée.
 * @fn Manette::getManette
 * @return manette QGamepad* Manette connectée.
 */
QGamepad * Manette::getManette()
{
    return manette;
}
