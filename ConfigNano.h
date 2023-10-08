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

// Configuration du mode de chaque sortie (Arduino Nano)
const byte outputMode[] = {
    /* D2 : S1 */ MODE_IO,
    /* D3 : S2 */ MODE_PWM,
    /* D4 : S3 */ MODE_IO,
    /* D5 : S4 */ MODE_PWM,
    /* D6 : S5 */ MODE_PWM,
    /* D7 : S6 */ MODE_IO,
    /* D8 : S7 */ MODE_IO,
    /* D9 : S8 ou SM1 */ MODE_PWM,
    /* D10 : S9 ou SM2  */ MODE_PWM,
    /* D11 : S10 */ MODE_PWM,
};

// Nombre maximum d'éclairages dans le tableau précédent
#define maxLights sizeof(outputMode)/sizeof(byte)

// Entrée digital D15 permettant de choisir la séquence 1 ou la séquence 2 (Pull-up)
const byte seqPin = 15;

// Entrée digital D14 permettant de passer en RUN (1) ou en STOP (0)  (Pull-up)
const int startPin = 14;       /* ESTARTB/ESTARTH selon l'état bas (STOP) ou haut (RUN) */

// Entrées digitals utilisateur (Pull-Up)
const byte inputUserPin1 = 16; /* E1B / E1H selon l'état bas ou haut */
const byte inputUserPin2 = 17; /* E2B / E2H selon l'état bas ou haut */
const byte inputUserPin3 = 18; /* E3B / E3H selon l'état bas ou haut */
const byte inputUserPin4 = 19; /* E4B / E3H selon l'état bas ou haut */

// Anti-rebond optimisé pour la rétro-signalisation Marklin C / Platine Obourg v2
const byte maxInputPins  = 5; /* nombre d'entrées à gérer en anti-rebond */
const byte maxFiltreH = 8;    /* en millisecondes, le temps que l'entrée doit être stabilisée avant de passer de 1 -> 0 */
const byte maxFiltreB = 512;  /* en millisecondes, le temps que l'entrée doit être stabilisée avant de passer de 0 -> 1 */

// Entrée pour le seed Generator
const byte seedPin = 7;
