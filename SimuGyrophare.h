// Batiment01::SimuGyrophare.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la suite des allumages et extinctions qu'il 
// faut envoyer à une led pour simuler un gyrophare
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
