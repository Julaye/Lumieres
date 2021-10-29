// Batiment01::ConfigNano.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration matérielle utilisée pour gérer 
// les éclairages d'un batiment avec un Arduino Nano
//
// Les éléments qui suivent sont spécifiques à l'Arduino Nano et ne
// doivent être modifié que pour supporter une carte différente

// mode des sorties : PWM ou IO 
// -> attention l'intensité n'est pas réglable en IO, protéger avec une 
//    résistance en fonction de la led utilisée

const int MODE_PWM = 0;
const int MODE_IO = 1;

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
