// Lumieres::FSMeclairage.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la machine à état fini pour gérer un éclairage
//

// type d'éclairage
const byte ETYPE_STANDARD  = 0;
const byte ETYPE_NEONNEUF  = 1;
const byte ETYPE_NEONVIEUX = 2; /* glitch aléatoire */
const byte ETYPE_GYROPHARE = 3;
const byte ETYPE_FLASH     = 4;
const byte ETYPE_SOUDURE   = 5; /* un seul Poste de soudure par Arduino */
const byte ETYPE_FIRE      = 6; /* une seule simulation de feu par Arduino */
const byte ETYPE_CLIGNOTANT= 7; /* éclairage qui ne clignote que s'il n'est pas permanent !*/

const byte ETYPE_SERVO   = 128; /* un seul servomoteur sur D9 ou D10  */

const byte ETYPE_NOTUSED = 255;

// state of the FSM : "stateRunning"
// chaque éclairage a un état particulier et un graphe de transition

const byte estate_OFF       = 0; /* éclairage éteint */

const byte estate_STPWRUP   = 1; /* éclairage en début d'allumage --> démarre la séquence PWRUP sauf si l'éclairage est déjà allumé */

const byte estate_PWRUP     = 2; /* éclairage en allumage --> ON quand la séquence PWRUP sera terminée*/

const byte estate_ON        = 3; /* éclairage allumé */

// blink transition pour un éclairage
typedef struct blink {
  int duration;
  byte intensity;
} blink;

// par défaut, un éclairage n'est connecté à aucune entrée
const int LightNotLinked = 255;

// parametres d'un éclairage
typedef struct {
    byte stateRunning; /* état (OFF, STPWRUP, PWRUP ou ON) */
    byte statePwrup;   /* dans l'état PWRUP, mémoriser la transition en cours */
    int stateDelay;   /* delay avant la prochaine transition ON OFF */
    blink*  pblink;   /* pointeur sur la table des transitions */
    byte maxblink;    /* taille de la table des transitions */
    byte nextState;   /* état une fois que la table des transitions a été parcourue */
    byte param;
    byte link;        /* si l'éclairage est lié à une entrée */
} paramLight;

// Pour chaque éclairage
paramLight gLight[maxLights];
