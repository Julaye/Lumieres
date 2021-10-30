// Lumieres::SimuFire.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la suite des allumages et extinctions qu'il 
// faut envoyer à une led pour simuler un feu
//

const int maxBlinkFire = 32;

// le tableau va devoir etre construit avec l'algorithme 
blink blinkFire[maxBlinkFire];

// cette fonction construit le tableau est retourne le nombre d'événements à générer
int construitTableauFire()
{
  for (int i = 0 ; i < maxBlinkFire ; i++)
  {
    blinkFire[i].duration = random(100);
    blinkFire[i].intensity = random(LIGHT_FAD8,LIGHT_ON);
  } 
  
  return maxBlinkFire;
}
