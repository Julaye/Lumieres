// Lumieres::ConfigAutomatismes.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration d'automatismes de base selon la
// cartographie suivante :
//
// 00 : seq1: chenillard (séquences de tests), seq2: anti-rebond (seq1: ExB -> Sx+1 , seq2: ExH -> Sx+1)
// 01 : seq1: paparazzi (E1B -> S2 flash), seq2: néons ateliers + soudure (ExB -> Sx+1)
// 10 : signal SNCB (TBD)
// 11 : automatisme utilisateur (programmé dans ConfigLumieres.h) (mode par défaut)
// ================================================================

// === Prog00 ===
// seq1: chenillard (séquences de tests), seq2: anti-rebond (seq1: ExB -> Sx+1 , seq2: ExH -> Sx+1)

const byte ledCnf_Prog00[] = {
 /* D2 */ ETYPE_NEONNEUF,   /* S1 (1)  : néon neuf sur une sortie standard (IO) *
 /* D3 */ ETYPE_SOUDURE,    /* S2 (2)  : poste de soudure (PWM) */
 /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : sortie standard (IO) */
 /* D5 */ ETYPE_GYROPHARE,  /* S4 (8)  : gyrophare (PWM) */
 /* D6 */ ETYPE_FIRE,       /* S5 (16) : feu bougie ou braseo (PWM) */
 /* D7 */ ETYPE_NOTUSED,    /* S6 (32) : not used (IO) */
 /* D8 */ ETYPE_NOTUSED,    /* S7 (64) : not used (IO) */
 /* D9 */ ETYPE_NEONVIEUX,  /* S8 (128) ou SM1 : néon vieux avec glitchs (PWM)  */
 /* D10 */ ETYPE_SERVO,     /* S9 (256) ou SM2 : Servo moteur (PWM) */
 /* D11 */ ETYPE_BUZZER     /* S10 (512) : Buzzer (PWM) */
};

DEBUTSEQ(Seq1_Prog00)
  MARK                /* Point de retour */
  LOOP                /* Retoune au point de retour */
FINSEQ(Seq1_Prog00)

DEBUTSEQ(Seq2_Prog00)
  MARK                /* Point de retour */
  LOOP                /* Retoune au point de retour */
FINSEQ(Seq2_Prog00)

// === Prog01 ===
// seq1: paparazzi (E1B -> S2 flash), seq2: néons ateliers + soudure (ExB -> Sx+1)

const byte ledCnf_Prog01[] = {
 /* D2 */ ETYPE_NEONNEUF,   /* S1 (1)  : néon neuf sur une sortie standard (IO) *
 /* D3 */ ETYPE_SOUDURE,    /* S2 (2)  : poste de soudure (PWM) */
 /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : sortie standard (IO) */
 /* D5 */ ETYPE_GYROPHARE,  /* S4 (8)  : gyrophare (PWM) */
 /* D6 */ ETYPE_FIRE,       /* S5 (16) : feu bougie ou braseo (PWM) */
 /* D7 */ ETYPE_NOTUSED,    /* S6 (32) : not used (IO) */
 /* D8 */ ETYPE_NOTUSED,    /* S7 (64) : not used (IO) */
 /* D9 */ ETYPE_NEONVIEUX,  /* S8 (128) ou SM1 : néon vieux avec glitchs (PWM)  */
 /* D10 */ ETYPE_SERVO,     /* S9 (256) ou SM2 : Servo moteur (PWM) */
 /* D11 */ ETYPE_BUZZER     /* S10 (512) : Buzzer (PWM) */
};

DEBUTSEQ(Seq1_Prog01)
  MARK                /* Point de retour */
  LOOP                /* Retoune au point de retour */
FINSEQ(Seq1_Prog01)

DEBUTSEQ(Seq2_Prog01)
  MARK                /* Point de retour */
  LOOP                /* Retoune au point de retour */
FINSEQ(Seq2_Prog01)

// === Prog10 ===
// signal SNCB (TBD)

const byte ledCnf_Prog10[] = {
 /* D2 */ ETYPE_NEONNEUF,   /* S1 (1)  : néon neuf sur une sortie standard (IO) *
 /* D3 */ ETYPE_SOUDURE,    /* S2 (2)  : poste de soudure (PWM) */
 /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : sortie standard (IO) */
 /* D5 */ ETYPE_GYROPHARE,  /* S4 (8)  : gyrophare (PWM) */
 /* D6 */ ETYPE_FIRE,       /* S5 (16) : feu bougie ou braseo (PWM) */
 /* D7 */ ETYPE_NOTUSED,    /* S6 (32) : not used (IO) */
 /* D8 */ ETYPE_NOTUSED,    /* S7 (64) : not used (IO) */
 /* D9 */ ETYPE_NEONVIEUX,  /* S8 (128) ou SM1 : néon vieux avec glitchs (PWM)  */
 /* D10 */ ETYPE_SERVO,     /* S9 (256) ou SM2 : Servo moteur (PWM) */
 /* D11 */ ETYPE_BUZZER     /* S10 (512) : Buzzer (PWM) */
};

DEBUTSEQ(Seq1_Prog10)
  MARK                /* Point de retour */
  LOOP                /* Retoune au point de retour */
FINSEQ(Seq1_Prog10)

DEBUTSEQ(Seq2_Prog10)
  MARK                /* Point de retour */
  LOOP                /* Retoune au point de retour */
FINSEQ(Seq2_Prog10)

// === Pour la commande d'un servo moteur sur la sortie D9 ou D10 uniquement ===
#include <Servo.h>

