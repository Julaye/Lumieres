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

// === Prog00x ===
// seq1: chenillard (séquences de tests)
// seq2: filtre anti-rebond (ExB -> Sx , seq2: ExH -> Sx+4)

/* PROG 0 */
DEBUTSEQ(Prog000)
  MARK                /* Point de retour */
    1,    1, _SET,
    2,    1, _SET,
    4,    1, _SET,
    8,    1, _SET,
    16,   1, _SET,
    32,   1, _SET,
    64,   1, _SET,
    128,  1, _SET,
    0,    1, _WAIT,    /* Eteint tout et Attend pendant 1 secondes */
    255,  2, _SET,     /* Allume tout pendant 2 secondes */
    ESTARTH,0,_WSTOP,  /* Vérifie que la startPin est toujours à 1 */
  LOOP                 /* Retoune au point de retour */
FINSEQ(Seq1_Prog000)

/* PROG 1 */
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
// seq1: néons neufs (S1,S3), paparazzi (E1B -> S2 flash), gyrophare (E2B -> S4 gyrophare), brasero (E3B -> S5), néon vieux (E4B -> S6, S8), soudure (S7)
// seq2: néons neufs (S1,S3), paparazzi (E1B -> S2 flash), gyrophare (E2B -> S4 gyrophare), brasero (S5), néon vieux(E4H -> S6, S8), soudure (E3B -> S7)

/* PROG 2 */
DEBUTSEQ(Prog010)
  SETMODE(S1+S3,ETYPE_NEONNEUF)
  SETMODE(S2,ETYPE_FLASH)
  SETMODE(S4,ETYPE_GYROPHARE)
  SETMODE(S5,ETYPE_FIRE)
  SETMODE(S6+S8,ETYPE_NEONVIEUX)
  SETMODE(S7,ETYPE_SOUDURE)
  PERM(S1)
  PERM(S3)
  ATTACH(S4,E2B)
  ATTACH(S5,E3B)
  ATTACH(S6,E4B)
  PERM(S8)
  PERM(S7)
  MARK                /* Point de retour */
    WSTOP(E1B,1)        /* Attend que l'entrée E1 passe à 0, échantillonage à la seconde */
    ALEA(S2,4)          /* lance le flash configuré sur la sortie S2 aléatoirement (0..3) */
  LOOP                /* Retoune au point de retour */
FINSEQ(Prog010)

/* PROG 3 */
DEBUTSEQ(Prog011)
  SETMODE(S1+S3,ETYPE_NEONNEUF)
  SETMODE(S2,ETYPE_FLASH)
  SETMODE(S4,ETYPE_GYROPHARE)
  SETMODE(S5,ETYPE_FIRE)
  SETMODE(S6+S8,ETYPE_NEONVIEUX)
  SETMODE(S7,ETYPE_SOUDURE)
  PERM(S1)
  PERM(S3)
  ATTACH(S4,E2B)
  PERM(S5)
  ATTACH(S6,E4H)
  PERM(S8)
  ATTACH(S7,E3B)
  MARK          /* Point de retour */
    WSTOP(E1B,2)  /* Attend que l'entrée E1 passe à 0 pendant 2 secondes */
    ALEA(S2,4)    /* lance le flash configuré sur la sortie S2 aléatoirement (0..3) */
  LOOP          /* Retoune au point de retour */
FINSEQ(Prog011)

// === Prog10x ===
// seq1: signal SNCB (TBD) + 
// seq2: Passage à niveau (E1/E2 -> servo S6/S7 + clignotement S8)

/* PROG 4 */
DEBUTSEQ(Prog100)
  MARK                /* Point de retour */
  LOOP                /* Retoune au point de retour */
FINSEQ(Prog100)

/* PROG 5 */
DEBUTSEQ(Prog101)
  SETMODE(S6,ETYPE_SERVO)       /* S6 (32) ou SM1 : servo moteur de la barrière (PWM)  */
  SETMODE(S7,ETYPE_SERVO)       /* S7 (64) ou SM2 : servo moteur de la barrière (PWM) */
  SETMODE(S8,ETYPE_CLIGNOTANT)  /* S8 (128) : feu clignotant (PWM) */
  MARK                /* Point de retour */
    WSTOP(E1B,1)       /* attend que E1 passe à 0 -> détection convoi en amont */
    SET(S8,1)          /* lance le feu clignotant */
    PERM(S6)           /* ferme la barrière 1 */
    PERM(S7)           /* ferme la barrière 2 */
    WSTOP(E2B,1)       /* attend que E2 passe à 0 -> détection convoi proche du PN */
    PERM(S8)           /* clignotant devient fixe */
    WSTOP(E2H,1)       /* attend que E2 repasse à 1 -> convoi est passé */
    UNSET(S8)          /* eteint le feu clignotant */
    UNSET(S7)          /* ouvre la barrière 2 */
    UNSET(S6)          /* ouvre la barrière 1 */
  LOOP                /* Retoune au point de retour */
FINSEQ(Prog101)
