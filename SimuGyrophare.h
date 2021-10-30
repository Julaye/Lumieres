// Lumieres::SimuGyrophare.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la suite des allumages et extinctions qu'il 
// faut envoyer à une led pour simuler un gyrophare ou un flash
//

const blink blinkGyrophare[] = {
    125, LIGHT_FAD8, 
    100, LIGHT_FAD4,
    125, LIGHT_FAD2,
    150, LIGHT_ON,
    125, LIGHT_FAD2,
    100, LIGHT_FAD4,
    125, LIGHT_FAD8,
    150, LIGHT_OFF    
}; 

const blink blinkFlash[] = {
    10,  LIGHT_FAD8,
    10,  LIGHT_OFF,
    10,  LIGHT_FAD8,
    10,  LIGHT_OFF,
    10,  LIGHT_FAD8,
    10,  LIGHT_OFF,
    30 , LIGHT_FLASH, 
    10,  LIGHT_OFF,  
}; 

const blink blinkClignotant[] = {
    100, LIGHT_OFF,
    30,  LIGHT_FAD8,
    40,  LIGHT_FAD4,
    30,  LIGHT_FAD2,
    200, LIGHT_ON,
    30,  LIGHT_FAD2,
    40,  LIGHT_FAD4,
    30,  LIGHT_FAD8,
    100,  LIGHT_OFF,
}; 
