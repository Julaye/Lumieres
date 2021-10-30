// Lumieres::ConfigNano.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration matérielle utilisée pour gérer 
// les éclairages d'un batiment avec un Arduino Nano
//
// Les éléments qui suivent sont spécifiques à l'Arduino Nano et ne
// doivent être modifié que pour supporter une carte différente

// l'intensité maximum de chaque sortie PWM 
const int PWM_FOR_LED = 16;

// mode des sorties : PWM ou IO 
// -> attention l'intensité n'est pas réglable en IO, protéger avec une 
//    résistance en fonction de la led utilisée

const int MODE_PWM = 0;
const int MODE_IO = 1;

// ON ou OFF ou autre valeur (fading)
const int LIGHT_FLASH = 255;
const int LIGHT_ON = PWM_FOR_LED;
const int LIGHT_FAD2 = PWM_FOR_LED/2;
const int LIGHT_FAD4 = PWM_FOR_LED/4;
const int LIGHT_FAD8 = PWM_FOR_LED/8;
const int LIGHT_OFF = 0;

// Configuration du mode de chaque sortie (Arduino Nano)
const int outputMode[] = {
            /* D2 */ MODE_IO,
            /* D3 */ MODE_PWM,
            /* D4 */ MODE_IO,
            /* D5 */ MODE_PWM,
            /* D6 */ MODE_PWM,
            /* D7 */ MODE_IO,
            /* D8 */ MODE_IO,
            /* D9 */ MODE_PWM,
            /* D10 */ MODE_PWM,
            /* D11 */ MODE_PWM,
};

// Nombre maximum d'éclairages dans le tableau précédent
#define maxLights sizeof(outputMode)/sizeof(int)

// Entrée digital D15 permettant de choisir la séquence 1 ou la séquence 2 (Pull-up)
const int seqPin = 15;

// Entrée digital D14 permettant de passer en RUN (1) ou en STOP (0)  (Pull-up)
const int startPin = 14; 

// Entrées digitals utilisateur (Pull-Up)
const int inputUserPin1 = 16;
const int inputUserPin2 = 17;
const int inputUserPin3 = 18;
const int inputUserPin4 = 19;

// Entrée pour le seed Generator
const int seedPin = 7;
