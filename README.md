# Lumieres

FR: Gestion d'un bâtiment, d'une scène ou d'un ensemble d'éclairage avec un Arduino Nano (échelle HO 1/87 ou supérieure)

EN : Management of a building, a scene or a set of lighting with an Arduino Nano (scale HO 1/87 or higher)

## Resumé

Objectif du projet : Le programme Lumières est une tentative pour proposer aux adeptes du modélisme un automate de gestion des éclairages d’un bâtiment, d'une scène, ou plus généralement d’un ensemble d’éclairages, qui soit facile d’accès pour un non initié à la programmation d’un Arduino, et fortement paramétrable pour couvrir les besoins les plus courants.

L'utilisateur adapte le fichier *ConfigLumieres.h* selon ses besoins et il obtient un automate opérationnel, sans écrire une seule ligne de code, seulement des données dans le fichier susmentionné. C'est une approche NO CODE - LOW CODE pour surfer sur le buzz des outils de programmation actuels.

Les types d'éclairages supportés actuellement sont : lampe standard, néon neuf, néon fatigué (glitchs), flash de photographe, poste de soudure et gyrophare. Sur les sorties PWM (D3,D5,D6,D9,D10 et D11), l'automate gère la variation d'intensité lumineuse pour plus de réalisme.

En roadmap : le brasero, la lampe à pétrole, le servomoteur d'une porte ...

Licence : Le logiciel est sous licence GPL et peut être librement utilisé et modifié dans le cadre de notre passion de modélisme, vos éventuelles modifications doivent être reversées à la communauté.

Langage de programmation : C for Arduino Nano

Langue du projet : Français / French

Version pre-release : v20211030.2

Documentation : https://docs.google.com/document/d/1chZwYiFPMKlHFLLsJ7WHy0EHtaLVpebEK52L9wi9J30/

Forum 3rails : https://forum.3rails.fr/t/une-debutante-dans-le-decor-ep13-que-la-lumiere-jaillisse/20889/

## Notes de version

### v20211030.3
- Ajout du type Poste de soudure, pour animer très simplement un atelier ...
- Ajout du type d'éclairage Flash, qui permet de réaliser un automatisme de type photographe Paparazzi sur la détection d'une locomotive
- Ajout de 4 entrées utilisateurs en Pull-Up, modification de la commande WSTOP en conséquence

### v20211030.2
- changement de nom Batiment_v01 -> Lumieres, parce que l'automate s'enrichi progressivement de nombreuses fonctions

### v20211030
- ajout du mode gyrophare (fichier simugyro.h)
- reprise de la FSM eclairage pour une plus grande généricité, l'ajout d'autres types d'éclairage est simplifié
- mise à jour de la documentation sur google docs

### v20211029.3
- Probleme avec le germe (seed) qui n'était pas très aléatoire
- Séance de test intense autour des fonctionnalités avancées (soft reset, commande WSTOP, ...)

### v20211029.2
- La commande ALEA ne fonctionnait pas correctement concernant les éclairages déjà allumés
- Nettoyage du code autour des commandes ALEA, STANDBY et WAIT
- Documentation en ligne 

### v20211029
- Premier commit sur Github pour mettre à disposition la version
