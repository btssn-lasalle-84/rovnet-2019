# Le projet ROV'NET 2019

Le système ROV'NET est capable d'obtenir et gérer des données d'entrée afin de définir un environnement TQC (Tel Que Construit) en se déplaçant dans un milieu contaminé.

- [Le projet ROV'NET 2019](#le-projet-rovnet-2019)
  - [Auteurs](#auteurs)
  - [Présentation](#présentation)
  - [Productions](#productions)
  - [Vidéo](#vidéo)
  - [Base de données SQLite](#base-de-données-sqlite)
  - [Licence GPL](#licence-gpl)

## Auteurs

- REYNIER Jacques <reynier.jacques@gmail.com>
  - Démarrer une campagne
  - Visualiser les mesures des capteurs
  - Recevoir les mesures du robot
  - Déplacer le robot
  - Piloter le bras articulé
  - Envoyer les ordres de déplacement au robot
  - Archiver les mesures

- BOFFREDO Nicolas <nboffredo@gmail.com>
  - Démarrer une campagne
  - Visualiser l’environnement (le flux vidéo de la caméra et les données de télémétrie)
  - Recevoir les données de télémétrie
  - Prendre une photo
  - Piloter la caméra
  - Archiver les photos

## Présentation

Il s’agit de réaliser un ROV capable d’obtenir et gérer des données d’entrée afin de définir un environnement TQC (Tel Que Construit). Il devra se déplacer dans un milieu contaminé afin de faire des prises de vues :

- Le déplacement se fera à partir d’un châssis en liaison filaire à 4 roues motorisées indépendamment.
- Le ROV sera équipé d’une caméra d’aide au déplacements et/ou de capteurs d’obstacles.
- D’un capteur de température et de radioactivité.
- D’un dispositif de prise de vue motorisé.
- D’un bras de robotique avec pince de préhension

## Productions

- [dossier-rovnet-ir-2019.pdf](dossier-rovnet-ir-2019.pdf)
- [refman-rovnet.pdf](refman-rovnet.pdf)
- [presentation-reynier-2019.pdf](presentation-reynier-2019.pdf)
- [presentation-boffredo-2019.pdf](presentation-boffredo-2019.pdf)

## Vidéo

Lien : https://www.youtube.com/watch?v=uaxQGUP5dYY

## Base de données SQLite

```sql
CREATE TABLE 'campagnes' ( 'nom' VARCHAR, 'description' VARCHAR, 'date' DATETIME, 'cheminArchives' VARCHAR, 'idOperateur' INTEGER, 'idCampagne' INTEGER PRIMARY KEY AUTOINCREMENT, FOREIGN KEY(idOperateur) REFERENCES operateurs(idOperateur) )

CREATE TABLE 'mesures' ( 'date' DATETIME, 'temperature' DOUBLE, 'irradiation' DOUBLE, 'idCampagne' INTEGER, FOREIGN KEY(idCampagne) REFERENCES campagnes(idCampagne) )

CREATE TABLE 'operateurs' ( 'nom' VARCHAR, 'prenom' VARCHAR, 'idOperateur' INTEGER PRIMARY KEY AUTOINCREMENT )
```

## Licence GPL

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
