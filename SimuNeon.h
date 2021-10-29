// Batiment01::SimuNeon.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la suite des allumages et extinctions qu'il 
// faut envoyer à une led pour simuler un néon
//

// l'intensité maximum de chaque sortie PWM 
const int PWM_FOR_LED = 12;

// ON ou OFF ou autre valeur (fading)
const int LIGHT_ON = PWM_FOR_LED;
const int LIGHT_FAD2 = PWM_FOR_LED/2;
const int LIGHT_FAD4 = PWM_FOR_LED/4;
const int LIGHT_OFF = 0;

// blink transition pour un néon
const struct blink {
  int duration;
  int intensity;
} blinkOn[] = {
    10, LIGHT_ON,   20, LIGHT_OFF,
    20, LIGHT_ON,  240, LIGHT_OFF,
    20, LIGHT_ON,   40, LIGHT_OFF,
    20, LIGHT_ON,  100, LIGHT_OFF,
    20, LIGHT_ON,   20, LIGHT_OFF, 
    20, LIGHT_ON,  260, LIGHT_OFF,
    80, LIGHT_ON,   20, LIGHT_OFF, 
   240, LIGHT_ON,   60, LIGHT_OFF,
   160, LIGHT_ON,   20, LIGHT_OFF,
   240, LIGHT_ON,   20, LIGHT_OFF,
  1000, LIGHT_FAD2, 20, LIGHT_OFF,
    20, LIGHT_ON,   40, LIGHT_OFF,
   100, LIGHT_ON,   20, LIGHT_OFF,
  2740, LIGHT_ON,  340, LIGHT_FAD4,
   860, LIGHT_FAD2, 20, LIGHT_OFF,
  1400, LIGHT_ON,   20, LIGHT_OFF,
    60, LIGHT_ON,   20, LIGHT_OFF
}; 

// nombre de transitions
const int maxStateBlink = sizeof(blinkOn)/sizeof(blink);
