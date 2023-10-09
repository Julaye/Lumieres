// Lumieres::ConfigAutomatismes.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration d'automatismes de base selon la
// cartographie suivante (et en cela, on a une sorte de M83 plus puissant !):
//
// 00 : seq1: chenillard (séquences de tests), seq2: anti-rebond (seq1: ExB -> Sx+1 , seq2: ExH -> Sx+1)
// 01 : seq1: paparazzi (E1B -> S2 flash), seq2: néons ateliers + soudure (ExB -> Sx+1)
// 10 : signal SNCB (TBD)
// 11 : automatisme utilisateur (programmé dans ConfigLumieres.h) (mode par défaut)
// ================================================================

// === Pour la commande d'un servo moteur sur la sortie D9 ou D10 uniquement ===
#include <Servo.h>

// === Prog00x ===
// seq1: chenillard (séquences de tests)
// seq2: anti-rebond (seq1: ExB -> Sx+1 , seq2: ExH -> Sx+1)

const byte ledCnf_Prog00x[] = {
 /* D2 */ ETYPE_STANDARD,   /* S1 (1)  : sortie standard (IO) *
 /* D3 */ ETYPE_STANDARD,   /* S2 (2)  : sortie standard (PWM) */
 /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : sortie standard (IO) */
 /* D5 */ ETYPE_STANDARD,   /* S4 (8)  : sortie standard (PWM) */
 /* D6 */ ETYPE_STANDARD,   /* S5 (16) : sortie standard (PWM) */
 /* D7 */ ETYPE_STANDARD,   /* S6 (32) : sortie standard (IO) */
 /* D8 */ ETYPE_STANDARD,   /* S7 (64) : sortie standard (IO) */
 /* D9 */ ETYPE_STANDARD,   /* S8 (128) ou SM1 : sortie standard (PWM)  */
 /* D10 */ ETYPE_STANDARD,  /* S9 (256) ou SM2 : sortie standard (PWM) */
 /* D11 */ ETYPE_STANDARD   /* S10 (512) : sortie standard (PWM) */
};

DEBUTSEQ(Prog000)
  MARK                /* Point de retour */
    1,      1, _SET,
    2,      1, _SET,
    4,      1, _SET,
    8,      1, _SET,
    16,     1, _SET,
    32,     1, _SET,
    64,     1, _SET,
    128,    1, _SET,
    256,    1, _SET,
    512,    1, _SET,
    0,      1, _WAIT,  /* Eteint tout et Attend pendant 1 secondes */
    1023,   2, _SET,   /* Allume tout pendant 2 secondes */
    ESTARTH,0,_WSTOP,  /* Vérifie que la startPin est toujours à 1 */
  LOOP                 /* Retoune au point de retour */
FINSEQ(Seq1_Prog000)

DEBUTSEQ(Prog001)
  1, E1B, _ATTACH,  
  2, E2B, _ATTACH,  
  4, E3B, _ATTACH,  
  8, E4B, _ATTACH, 
  16, E1H, _ATTACH,  
  32, E2H, _ATTACH,  
  64, E3H, _ATTACH,  
  128, E4H, _ATTACH, 
  END /* fin de la séquence */
FINSEQ(Prog001)

// === Prog01x ===
// seq1: néons neufs (S1,S3, S6, S7), paparazzi (E1B -> S2 flash), gyrophare (E2B -> S4 gyrophare), brasero (E3B -> S5), néon (E4B -> S8, S10), soudure (S9)
// seq2: néons neufs (S1,S3, S6, S7), paparazzi (E1B -> S2 flash), gyrophare (E2B -> S4 gyrophare), brasero (S5), néon (E4B -> S8, S10), soudure (E3B -> S9)

const byte ledCnf_Prog01x[] = {
 /* D2 */ ETYPE_NEONNEUF,   /* S1 (1)  : néon neuf (IO) *
 /* D3 */ ETYPE_FLASH,      /* S2 (2)  : flash de photographe (PWM) */
 /* D4 */ ETYPE_NEONNEUF,   /* S3 (4)  : néon neuf (IO) */
 /* D5 */ ETYPE_GYROPHARE,  /* S4 (8)  : gyrophare (PWM) */
 /* D6 */ ETYPE_FIRE,       /* S5 (16) : feu bougie ou brasero (PWM) */
 /* D7 */ ETYPE_NEONNEUF,   /* S6 (32) : néon neuf (IO) */
 /* D8 */ ETYPE_NEONNEUF,   /* S7 (64) : néon neuf (IO) */
 /* D9 */ ETYPE_NEONVIEUX,  /* S8 (128) ou SM1 : néon vieux avec glitchs (PWM)  */
 /* D10 */ ETYPE_SOUDURE,   /* S9 (256) ou SM2 : poste soudure (PWM) */
 /* D11 */ ETYPE_NEONVIEUX  /* S10 (512) : néon vieux avec glitchs (PWM) */
};

DEBUTSEQ(Prog010)
  PERM(S1)
  PERM(S3)
  PERM(S6)
  PERM(S7)
  ATTACH(S4,E2B)
  ATTACH(S5,E3B)
  ATTACH(S8,E4B)
  PERM(S10)
  PERM(S9)
  MARK                /* Point de retour */
    WSTOP(E1B,1)        /* Attend que l'entrée E1 passe à 0, échantillonage à la seconde */
    ALEA(S2,4)          /* lance le flash configuré sur la sortie S2 aléatoirement (0..3) */
  LOOP                /* Retoune au point de retour */
FINSEQ(Prog010)

DEBUTSEQ(Prog011)
  PERM(S1)
  PERM(S3)
  PERM(S6)
  PERM(S7)
  ATTACH(S4,E2B)
  PERM(S5)
  ATTACH(S8,E4B)
  PERM(S10)
  ATTACH(S9,E3B)
  MARK                /* Point de retour */
    WSTOP(E1B,1)        /* Attend que l'entrée E1 passe à 0, échantillonage à la seconde */
    ALEA(S2,4)          /* lance le flash configuré sur la sortie S2 aléatoirement (0..3) */
  LOOP                /* Retoune au point de retour */
FINSEQ(Prog011)

// === Prog10x ===
// seq1: signal SNCB (TBD) + 
// seq2: Passage à niveau (E1/E2 -> servo S8/S9 + clignotement S10)

const byte ledCnf_Prog10x[] = {
 /* D2 */ ETYPE_STANDARD,   /* S1 (1)  : sortie standard (IO) *
 /* D3 */ ETYPE_STANDARD,   /* S2 (2)  : sortie standard (PWM) */
 /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : sortie standard (IO) */
 /* D5 */ ETYPE_STANDARD,   /* S4 (8)  : sortie standard (PWM) */
 /* D6 */ ETYPE_STANDARD,   /* S5 (16) : sortie standard (PWM) */
 /* D7 */ ETYPE_STANDARD,   /* S6 (32) : sortie standard (IO) */
 /* D8 */ ETYPE_STANDARD,   /* S7 (64) : sortie standard (IO) */
 /* D9 */ ETYPE_SERVO,      /* S8 (128) ou SM1 : servo moteur de la barrière (PWM)  */
 /* D10 */ ETYPE_SERVO,     /* S9 (256) ou SM2 : servo moteur de la barrière (PWM) */
 /* D11 */ ETYPE_CLIGNOTANT /* S10 (512) : feu clignotant (PWM) */
};

/* PROG 4 */
DEBUTSEQ(Prog100)
  MARK                /* Point de retour */
  LOOP                /* Retoune au point de retour */
FINSEQ(Prog100)

/* PROG 5 */
DEBUTSEQ(Prog101)
  MARK                /* Point de retour */
    WSTOP(E1B,1)        /* attend que E1 passe à 0 -> détection convoi en amont */
    SET(S10,1)          /* lance le feu clignotant */
    PERM(S8)            /* ferme la barrière 1 */
    PERM(S9)            /* ferme la barrière 2 */
    WSTOP(E2B,1)        /* attend que E2 passe à 0 -> détection convoi proche du PN */
    PERM(S10)           /* clignotant devient fixe */
    WSTOP(E2H,1)        /* attend que E2 repasse à 1 -> convoi est passé */
    UNSET(S10)          /* eteint le feu clignotant */
    UNSET(S9)           /* ouvre la barrière 2 */
    UNSET(S8)           /* ouvre la barrière 1 */
  LOOP                /* Retoune au point de retour */
FINSEQ(Prog101)