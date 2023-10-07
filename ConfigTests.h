// Lumieres::ConfigTests.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration de scènes de tests
//
// ---------------------------------------------------------------------
// --- ICI COMMENCE LA CONFIGURATION EN FONCTION DE VOS TESTS       ---

// Affectation des types d'éclairage ou de fonctions à chaque sortie
const byte ledCnf[] = {
 /* D2 */ ETYPE_NEONNEUF,   /* S1 (1)  : néon neuf sur une sortie standard (IO) *
 /* D3 */ ETYPE_SOUDURE,    /* S2 (2)  : poste de soudure (PWM) */
 /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : sortie standard (IO) *//Users/juliedumortier/Documents/L'héritière d'Obourg/Arduino/Lumieres/ConfigTestsFiltres.h
 /* D5 */ ETYPE_GYROPHARE,  /* S4 (8)  : gyrophare (PWM) */
 /* D6 */ ETYPE_FIRE,       /* S5 (16) : feu bougie ou braseo (PWM) */
 /* D7 */ ETYPE_NOTUSED,    /* S6 (32) : not used (IO) */
 /* D8 */ ETYPE_NOTUSED,    /* S7 (64) : not used (IO) */
 /* D9 */ ETYPE_NEONVIEUX,  /* S8 (128) ou SM1 : néon vieux avec glitchs (PWM)  */
 /* D10 */ ETYPE_SERVO,     /* S9 (256) ou SM2 : Servo moteur (PWM) */
 /* D11 */ ETYPE_BUZZER     /* S10 (512) : Buzzer (PWM) */
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
  MARK                /* place une MARK pour le LOOP */
  PERM(S4)            /* Allume D5 le gyrophare */
   4,      10, _SET,  /* Allume D4 (éclairage standard) pendant 10 secondes */
   2,      10, _SET,  /* Eteint D4 et Allume D2 (néon neuf) pendant 10 secondes */
   128,    10, _SET,  /* Eteint D2 et Allume D9 (néon ancien) pendant 10 secondes */
   0,       5, _WAIT, /* pause pendant 5 secondes, ne touche pas au lumières allumées */
   0,      10, _SET,  /* Eteint D9 et attends 10 secondes */
   2+4+128,10, _SET,  /* Allume D3, D5 et D6 pendant 10 secondes */
  UNSET(S4)           /* Arrete le gyrophare */
  LOOP                /* Tout est éteint. Boucle sur le début MARK */
FINSEQ(mySeq1)

// Séquence 2 :  un chenillard sur l'ensemble des sorties, c'est encore mieux !
// A noter que la sortie S3 (broche D4) est reliée à l'entrée E1B, mettre cette 
// entrée à 0 pour lancer le gyrophare
DEBUTSEQ(mySeq2)
    4,     E1B, _ATTACH,  /* utile pour tester l'anti-rebond ... */
    MARK
    1,      10, _SET,
    2,      10, _SET,
    8,      10, _SET,
    16,     10, _SET,
    32,     10, _SET,
    64,     10, _SET,
    128,    10, _SET,
    256,    10, _SET,
    512+2,  10, _SET,   /* Buzzer doit etre activité avec le poste soudure sinon bug */
    0,      10, _WAIT,  /* Attend pendant 10 secondes */
    1023-4-256,   10, _SET,   /* Allume tout pendant 10 secondes, sauf le servo et la sortie attachée */
    0,0,_WSTOP,
    LOOP
FINSEQ(mySeq2)

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOS TESTS      ---
// ---------------------------------------------------------------------
