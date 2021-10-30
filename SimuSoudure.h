// Lumieres::SimuSoudure.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la suite des allumages et extinctions qu'il 
// faut envoyer à une led pour simuler une soudure à l'arc
//
// Je me suis inspirée du programme proposé par Locoduino ici :
// https://www.locoduino.org/spip.php?article90
//

//
const long Duree_flash_mini = 10 ;
const long Duree_flash_maxi = 101 ;
const long Duree_int_mini = 10 ;
const long Duree_int_maxi = 31 ;
const long Nbre_even_mini = 10 ;
const long Nbre_even_maxi = 21 ;
const long P_repos_mini = 1500 ;
const long P_repos_maxi = 7001 ;

// le tableau va devoir etre construit avec l'algorithme 
// proposé par Locoduino ... facile
blink blinkSoudure[Nbre_even_maxi*2+1];

// cette fonction construit le tableau est retourne le nombre d'événements à générer
int construitTableauSoudure()
{
  int  Nbre_even = random (Nbre_even_mini, Nbre_even_maxi) ;
  int  n = 0;
  
  for (int i = 1 ; i <= Nbre_even ; i++)
  {
    int Duree_flash = random (Duree_flash_mini, Duree_flash_maxi) ;
    int Duree_int = random (Duree_int_mini, Duree_int_maxi) ;

    blinkSoudure[n].duration = Duree_flash;
    blinkSoudure[n].intensity = LIGHT_ON;   /* plus sympa en mode PWM de régler l'intensité */
    n++;
        
    blinkSoudure[n].duration = Duree_int;
    blinkSoudure[n].intensity = LIGHT_OFF;
    n++;
  }

  int P_repos = random (P_repos_mini, P_repos_maxi) ;
  blinkSoudure[n].duration = P_repos;
  blinkSoudure[n].intensity = LIGHT_OFF;
  n++;
  
  return n+1;
}
