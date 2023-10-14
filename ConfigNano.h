// Lumieres::ConfigNano.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration matérielle utilisée pour gérer 
// les automatismes (éclairages, servo, ...) avec un Arduino Nano
//
// Les éléments qui suivent sont spécifiques à l'Arduino Nano et ne
// doivent être modifié que pour supporter une carte différente
//
// A noter que ce code est optimisé pour la platine Obourg v2 que
// l'on peut découvrir ici : https://forum.3rails.fr/t/une-debutante-dans-le-decor-ep13-que-la-lumiere-jaillisse/20889/

//
// mode des sorties : PWM ou IO 
// -> attention l'intensité n'est pas réglable en IO, protéger avec une 
//    résistance en fonction de la led utilisée

const byte MODE_PWM = 0;
const byte MODE_IO = 1;

// ON ou OFF ou autre valeur (fading)
const byte LIGHT_FLASH = 255;
const byte LIGHT_ON = PWM_FOR_LED;
const byte LIGHT_FAD2 = PWM_FOR_LED/2;
const byte LIGHT_FAD4 = PWM_FOR_LED/4;
const byte LIGHT_FAD8 = PWM_FOR_LED/8;
const byte LIGHT_OFF = 0;

// mapping sortie logique (0..maxoutputs-1) -> sortie physique
const byte gDx[] = {
    /* D2 : S1 */ 2,
    /* D3 : S2 */ 3,
    /* D4 : S3 */ 4,
    /* D5 : S4 */ 5,
    /* D6 : S5 */ 6,
    /* D9 : S6 ou SM1 */ 9,
    /* D10 : S7 ou SM2  */ 10,
    /* D11 : S8 */ 11
};

// index des servo moteurs dans la table des sorties physiques
#define Index_SM1 5
#define Index_SM2 6

// mode de chaque sortie : IO standard ou PWM
const byte gMode[] = {
    /* D2 : S1 */ MODE_IO,
    /* D3 : S2 */ MODE_PWM,
    /* D4 : S3 */ MODE_IO,
    /* D5 : S4 */ MODE_PWM,
    /* D6 : S5 */ MODE_PWM,
    /* D9 : S6 ou SM1 */ MODE_PWM,
    /* D10 : S7 ou SM2  */ MODE_PWM,
    /* D11 : S8 */ MODE_PWM
};

// Nombre maximum d'éclairages dans le tableau précédent
#define maxOutputs sizeof(gMode)

// Entrée digital D15 permettant de choisir la séquence 1 ou la séquence 2 (Pull-up)
const byte seqPin = 15;

// Entrées D7 et D8 pour extraire le numéro d'automatisme, cumulés avec seqPin !
const byte prog0Pin = 7;
const byte prog1Pin = 8;

// Entrée digitale D14 permettant de passer en RUN (1) ou en STOP (0)  (Pull-up)
const byte startPin = 14;       /* ESTARTB/ESTARTH selon l'état bas (STOP) ou haut (RUN) */

// Entrées digitales utilisateur (Pull-Up)
const byte inputUserPin1 = 16; /* E1B / E1H selon l'état bas ou haut */
const byte inputUserPin2 = 17; /* E2B / E2H selon l'état bas ou haut */
const byte inputUserPin3 = 18; /* E3B / E3H selon l'état bas ou haut */
const byte inputUserPin4 = 19; /* E4B / E3H selon l'état bas ou haut */

// Anti-rebond optimisé pour la rétro-signalisation Marklin C / Platine Obourg v2
const byte maxInputPins  = 5; /* nombre d'entrées à gérer en anti-rebond */
const byte maxInputs = maxInputPins+3;

const byte maxFiltreH = 8;    /* en millisecondes, le temps que l'entrée doit être stabilisée avant de passer de 1 -> 0 */
const byte maxFiltreB = 512;  /* en millisecondes, le temps que l'entrée doit être stabilisée avant de passer de 0 -> 1 */

// Entrée analogique A6 non utilisée
const byte unusedPin = A6;

// Entrée Analogique (A7/D21) pour le seed Generator
const byte seedPin = A7;