# Batiment_v01

FR: Gestion d'un bâtiment ou d'un ensemble d'éclairage avec un Arduino Nano (échelle HO 1/87 ou supérieure)

EN : Management of a building or a set of lighting with an Arduino Nano (scale HO 1/87 or higher)

## Resumé

Objectif du projet : Le programme Batiment_v01 est une tentative pour proposer aux adeptes du modélisme un automate de gestion des éclairages d’un bâtiment, ou plus généralement d’un ensemble d’éclairages, qui soit facile d’accès pour un non initié à la programmation d’un Arduino, et fortement paramétrable pour couvrir les besoins les plus courants.

Licence : GPL v3

Langage de programmation : C for Arduino Nano

Langue du projet : Français / French

Version pre-release : v20211030

Documentation : https://docs.google.com/document/d/1chZwYiFPMKlHFLLsJ7WHy0EHtaLVpebEK52L9wi9J30/

Forum 3rails : https://forum.3rails.fr/t/une-debutante-dans-le-decor-ep13-que-la-lumiere-jaillisse/20889/

## Notes de version

### v20211030
- ajout du mode gyrophare (fichier simugryo.h)
- reprise de la FSM eclairage pour une plus grande généricité, l'ajout d'autres types d'éclairage est simplifié

### v20211029.3
- Probleme avec le germe (seed) qui n'était pas très aléatoire
- Séance de test intense autour des fonctionnalités avancées (soft reset, commande WSTOP, ...)

### v20211029.2
- La commande ALEA ne fonctionnait pas correctement concernant les éclairages déjà allumés
- Nettoyage du code autour des commandes ALEA, STANDBY et WAIT
- Documentation en ligne 

### v20211029
- Premier commit sur Github pour mettre à disposition la version
