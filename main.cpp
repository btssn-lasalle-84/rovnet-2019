#include "ihmrov.h"
#include <QApplication>

/**
 * @file main.cpp
 *
 * @brief Programme principal Rov'net
 *
 * @details Crée et affiche la fenêtre principale de l'application Rov'net
 *
 * \author Nicolas BOFFREDO <nboffredo@gmail.com>
 * \author Jacques REYNIER <reynier.jacques@gmail.com>
 *
 * @version 1.1
 *
 * @fn main(int argc, char *argv[])
 * @param argc
 * @param argv[]
 * @return int
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IHMRov ihm;
    ihm.show();

    return a.exec();
}
