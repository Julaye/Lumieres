// Lumieres::ConfigTestsFiltres.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration de scènes de tests pour Filtres
//
// ---------------------------------------------------------------------
// --- ICI COMMENCE LA CONFIGURATION EN FONCTION DE VOS TESTS       ---

// Affectation des types d'éclairage ou de fonctions à chaque sortie
const byte ledCnf[] = {
 /* D2 */ ETYPE_STANDARD,   /* S1 (1)  : sortie standard (IO) *
 /* D3 */ ETYPE_FLASH,      /* S2 (2)  : poste de soudure (PWM) */
 /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : sortie standard (IO) */
 /* D5 */ ETYPE_GYROPHARE,  /* S4 (8)  : gyrophare (PWM) */
 /* D6 */ ETYPE_NOTUSED,    /* S5 (16) : not used (PWM) */
 /* D7 */ ETYPE_NOTUSED,    /* S6 (32) : not used (IO) */
 /* D8 */ ETYPE_NOTUSED,    /* S7 (64) : not used (IO) */
 /* D9 */ ETYPE_NOTUSED,    /* S8 (128) ou SM1 : not used (PWM)  */
 /* D10 */ ETYPE_NOTUSED,   /* S9 (256) ou SM2 : not used (PWM) */
 /* D11 */ ETYPE_NOTUSED    /* S10 (512) : not used (PWM) */
};

// Pour la commande d'un servo moteur sur la sortie D9 ou D10 uniquement
// LIGNE CI-APRÈS À METTRE EN COMMENTAIRE SI VOUS N'UTILISEZ AUCUN SERVO
#include <Servo.h>

// ---------------------------------------------------------------------
// Des séquences a pour tester le programme et/ou tester les éclairages
// ---------------------------------------------------------------------

// Séquence 1 : un simple chenillard sur les sorties D2, D4 et D9 , c'est bien !
// Je mixe volontairement macro langage et structure de données
DEBUTSEQ(mySeq1)
  1, E1B, _ATTACH,  /* utile pour tester l'anti-rebond ... */
  4, E3B, _ATTACH,  /* utile pour tester l'anti-rebond ... */
  8, E4B, _ATTACH,  /* utile pour tester l'anti-rebond ... */
  MARK          /* point de retour de l’automatisme */  
  WSTOP(E2B,1)  /* attend la détection d’une locomotive sur l’entrée E2 */
  ALEA(S2,4)    /* lance le flash configuré sur la sortie S2 aléatoirement (0..3) */
  LOOP          /* boucle pour flasher à nouveau */
FINSEQ(mySeq1)

// Séquence 2 :  un chenillard sur l'ensemble des sorties, c'est encore mieux !
// A noter que la sortie S3 (broche D4) est reliée à l'entrée E1B, mettre cette 
// entrée à 0 pour lancer le gyrophare
DEBUTSEQ(mySeq2)
  1, E1B, _ATTACH,  /* utile pour tester l'anti-rebond ... */
  4, E3B, _ATTACH,  /* utile pour tester l'anti-rebond ... */
  8, E4B, _ATTACH,  /* utile pour tester l'anti-rebond ... */
  MARK          /* point de retour de l’automatisme */  
  WSTOP(E2B,1)  /* attend la détection d’une locomotive sur l’entrée E2 */
  ALEA(S2,4)    /* lance le flash configuré sur la sortie S2 aléatoirement (0..3) */
  LOOP          /* boucle pour flasher à nouveau */
FINSEQ(mySeq2)

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOS TESTS      ---
// ---------------------------------------------------------------------
