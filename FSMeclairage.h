// Batiment01::FSMeclairage.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la machine à état fini pour gérer un éclairage
//

// type d'éclairage
const int ETYPE_STANDARD  = 0;
const int ETYPE_NEONNEUF  = 1;
const int ETYPE_NEONVIEUX = 2;
const int ETYPE_NOTUSED  = 99;

// state of the FSM : "stateRunning"
// chaque éclairage a un état particulier et un graphe de transition

const int estate_OFF       = 0; /* éclairage éteint */

const int estate_STPWRUP   = 1; /* éclairage en début d'allumage --> démarre la séquence PWRUP sauf si l'éclairage est déjà allumé */

const int estate_PWRUP     = 2; /* éclairage en allumage --> ON quand la séquence PWRUP sera terminée*/

const int estate_ON        = 3; /* éclairage allumé */

// Pour chaque éclairage, il faut garder son état (OFF, STPWRUP, PWRUP ou ON)
int gEStateRunning[maxLights];

// Pour chaque éclairage dans l'état PWRUP, mémoriser la transition en cours
int gEStatePwrup[maxLights];

// delay avant la prochaine transition ON OFF
int gEStateDelay[maxLights];

// numéro de la transition pour reprendre l'enchainement sur un glitch de néon
const int INDEX_GLITCHPWRUP = 22;
