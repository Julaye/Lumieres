// Lumieres::FSMeclairage.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la machine à état fini pour gérer un éclairage
//

// type d'éclairage
const int ETYPE_STANDARD  = 0;
const int ETYPE_NEONNEUF  = 1;
const int ETYPE_NEONVIEUX = 2; /* glitch aléatoire */
const int ETYPE_GYROPHARE = 3;
const int ETYPE_FLASH     = 4;
const int ETYPE_SOUDURE   = 5; /* un seul Poste de soudure par Arduino */
const int ETYPE_FIRE      = 6; /* une seule simulation de feu par Arduino */
const int ETYPE_CLIGNOTANT= 7; /* éclairage qui ne clignote que s'il n'est pas permanent !*/

const int ETYPE_SERVO   = 128; /* un seul servomoteur sur D9 ou D10  */

const int ETYPE_NOTUSED = 255;

// state of the FSM : "stateRunning"
// chaque éclairage a un état particulier et un graphe de transition

const int estate_OFF       = 0; /* éclairage éteint */

const int estate_STPWRUP   = 1; /* éclairage en début d'allumage --> démarre la séquence PWRUP sauf si l'éclairage est déjà allumé */

const int estate_PWRUP     = 2; /* éclairage en allumage --> ON quand la séquence PWRUP sera terminée*/

const int estate_ON        = 3; /* éclairage allumé */

// blink transition pour un éclairage
typedef struct blink {
  int duration;
  int intensity;
} blink;

// parametres d'un éclairage
typedef struct {
    int stateRunning; /* état (OFF, STPWRUP, PWRUP ou ON) */
    int statePwrup;   /* dans l'état PWRUP, mémoriser la transition en cours */
    int stateDelay;   /* delay avant la prochaine transition ON OFF */
    blink*  pblink;   /* pointeur sur la table des transitions */
    int maxblink;     /* taille de la table des transitions */
    int nextState;    /* état une fois que la table des transitions a été parcourue */
    byte param;
} paramLight;

// Pour chaque éclairage
paramLight gLight[maxLights];
