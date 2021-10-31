// Lumieres::ConfigLumieres.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration de votre scène, soit de 
// l'ensemble des éclairages ou sorties que vous souhaitez commander.
//
// il contient le mapping entre sorties de l'Arduino et type d'éclairages 
// ainsi que les deux séquences executées par l'automate fini.
//
// Le programme est livré avec la configuration du batiment administratif
// du dépot diesel appartenant à notre Mlle Pélerin
//
// Plus d'exemples dans la documention, notamment pour les fonctions les
// avancées comme le poste de soudure, le flash du photographe, ...
//

// ---------------------------------------------------------------------
// --- ICI COMMENCE LA CONFIGURATION EN FONCTION DE VOTRE BATIMENT ---

// Affectation des types d'éclairage ou de fonctions à chaque sortie
const byte ledCnf[] = {
            /* D2 */ ETYPE_NEONNEUF,   /* S1 (1)  : bureau administratif (IO) *
            /* D3 */ ETYPE_NEONNEUF,   /* S2 (2)  : accueil (PWM) */
            /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : bureau M. CLaude (IO) */
            /* D5 */ ETYPE_NEONNEUF,   /* S4 (8)  : couloir haut (PWM) */
            /* D6 */ ETYPE_NEONVIEUX,  /* S5 (16) : escalier (PWM) */
            /* D7 */ ETYPE_NEONNEUF,   /* S6 (32) : couloir bas (IO) */
            /* D8 */ ETYPE_NEONNEUF,   /* S7 (64) : bureau M. Gaston (IO) */
            /* D9 */ ETYPE_NEONNEUF,   /* S8 (128) ou SM1 : bureau secrétaire (PWM) */
            /* D10 */ ETYPE_STANDARD,  /* S9 (256) ou SM2 : bureau Mlle Pélerin (PWM) */
            /* D11 */ ETYPE_GYROPHARE  /* S10 (512) : gyrophare (PWM) */
};

// Pour la commande d'un servo moteur sur la sortie D9 ou D10 uniquement
// LIGNE CI-APRÈS À METTRE EN COMMENTAIRE SI VOUS N'UTILISEZ AUCUN SERVO
//#include <Servo.h>

// Séquence est une liste de couple (sorties, durée d'allumage en secondes, commande)
// Cf le fichier FSMLumieres.h pour la signification de chaque commande utilisée

/* Séquence 1 : Miss Pélerin passe (avec ou sans M. Gaston) */
DEBUTSEQ(mySeq1)
  SET(S2,10)    /* allume l'accueil (néon neuf) pendant 10 secondes */
  SET(S2+S6,10) /* laisse l'accueil allumé et allume le couloir RDC (néon neuf) pendant 10 secondes */
  SET(S5+S6,10) /* éteint l'accueil et allume le couloir RDC (néon neuf) et l'escalier (néon ancien) pendant 10 secondes */
  SET(S5+S4,10) /* ETC :)  */
  SET(S4+S9,10)
  ALEA(S7,2)    /* parfois M. gaston accompagne la miss (50% de chance) */  
  ALEA(S8,2)    /* parfois la secrétaire accompagne la miss (50% chance) */
  STANDBY(S9,10) /* la miss reste dans son bureau un certain temps (1 à 10 minutes) */
  SET(S4+S9,2)
  SET(S4,10)
  SET(S4+S5,10)
  SET(S5+S6,10)
  SET(S6+S2,10)
  SET(S2,10)   /* éteint le couloir du RDC, allume l'accueil pendant 10 secondes */
  END          /* fin de la séquence, tout est éteint */
FINSEQ(mySeq1)

/* Séquence 2 : le dépot fonctionne de nuit */
DEBUTSEQ(mySeq2)
  SET(S2,10)
  PERM(S2)
  SET(S6,10)  
  SET(S6+S1,2)
  SET(S1,2)
  PERM(S1)
  SET(S5,10)
  SET(S5+S4,10)
  PERM(S8)
  ALEA(S9,4)
  ALEA(S7,2)  
  ALEA(S3,2)
  ALEA(S8,2)
  WSTOP(ESTARTB,3)
  UNSET(S9+S8+S7+S3) /* éteint les bureaux de l'étage */
  SET(S5,10)
  UNSET(S4+S1)       /* eteint couloir haut et administratif */
  WAIT(10)
  UNSET(S6)          /* éteint couloir bas */
  WAIT(10)
  UNSET(S1)          /* éteint accueil */
  END                /* fin de la séquence */
FINSEQ(mySeq2)

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOTRE SCENE    ---
// ---------------------------------------------------------------------
