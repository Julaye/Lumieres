// Lumieres::SimuNeon.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la suite des allumages et extinctions qu'il 
// faut envoyer à une led pour simuler un néon
//

const blink blinkNeon[] = {
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

// numéro de la transition pour reprendre l'enchainement sur un glitch de néon
const byte INDEX_GLITCHPWRUP = 22;
