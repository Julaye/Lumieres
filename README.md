# Lumieres

FR: Gestion d'un bâtiment, d'une scène ou d'un ensemble d'éclairage avec un Arduino Nano (échelle HO 1/87 ou supérieure)

EN : Management of a building, a scene or a set of lighting with an Arduino Nano (scale HO 1/87 or higher)

## Resumé

Objectif du projet : Le programme Lumières est une tentative pour proposer aux adeptes du modélisme un automate de gestion des éclairages d’un bâtiment, d'une scène, ou plus généralement d’un ensemble d’éclairages, qui soit facile d’accès pour un non initié à la programmation d’un Arduino, et fortement paramétrable pour couvrir les besoins les plus courants.

L'utilisateur adapte le fichier *ConfigLumieres.h* selon ses besoins et il obtient un automate opérationnel, sans écrire une seule ligne de code, seulement des données dans le fichier susmentionné. C'est une approche NO CODE - LOW CODE pour surfer sur le buzz des outils de programmation actuels.

Les types d'éclairages actuellement supportés sont : 
- lampe standard (allumé ou éteint)
- néon neuf avec sa séquence d’allumage, 
- néon fatigué avec des glitchs aléatoires, 
- flash de photographe avec pre-flash, 
- poste de soudure à l’arc, 
- servo moteur 0-90° pour l’ouverture/fermeture d’une porte
- gyrophare
- feu clignotant qui ne clignote que s’il n’est pas permanent.
 

Sur les sorties PWM (D3,D5,D6,D9,D10 et D11), l'automate gère la variation d'intensité lumineuse pour plus de réalisme.

Licence : Le logiciel est sous licence GPL et peut être librement utilisé et modifié dans le cadre de notre passion de modélisme, vos éventuelles modifications doivent être reversées à la communauté.

Langage de programmation : C for Arduino Nano

Langue du projet : Français / French

Version pre-release : v20211030.6

Documentation : https://docs.google.com/document/d/1chZwYiFPMKlHFLLsJ7WHy0EHtaLVpebEK52L9wi9J30/

La documentation proposée présente aussi dans son chapitre 9 de nombreux exemples d'automatismes qui sont faciles à réaliser avec cet automate :
- Photographe paparazzi dont le flash se déclenche au passage d'un train
- Gestion de la séquence d'éclairage d'une fosse d'inspection au stationnement d'une locomotive
- Animation d'un poste de soudure en présence d'une locomotive dans l'atelier
- Animation de feux tricolores à un croisement routier, inclu les feux en panne (orange clignotant)
- L'ouverture et la fermeture d'une porte de remise avec un gyrophare de sécurité lorsqu'une locomotive se présente devant la porte
- Passage à niveau avec feu clignotant et barrière commandée par un servo
- ...

Forum 3rails : https://forum.3rails.fr/t/une-debutante-dans-le-decor-ep13-que-la-lumiere-jaillisse/20889/

## Notes de version

### v20211031
- introduction d'un macro langage pour simplifier la lecture d'une séquence d'automatisme
- optimise encore un peu la mémoire utilisée (int -> byte quand possible)

### v20211030.6
- Ajout de la commande PWM
- Fixe une régression sur STANDBY à cause de l'optimisation des 'long int'

### v20211030.5
- Ajout du type Clignotant qui ne clignote que s'il n'est pas permanent
- Fixe une régression de l'automate à cause de l'optimisation des 'long int'

### v20211030.4
- Ajout du type Servo pour commander un servo moteur 0-90° pour ouvrir ou fermer une porte 
- La valeur 0 dans le paramètre duration permet la demi-seconde pour les commandes SET, WAIT et WSTOP
- une valeur différente de 0 dans le paramètre duration de la commande UNSET introduit une attente de nombre de secondes spécifié
- optimisation de la mémoire dynamique (chasse aux 'long int' inutiles)
- Ajout du type Fire pour simuler une bougie, un brasero

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
