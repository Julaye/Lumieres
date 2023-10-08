// Lumieres::ConfigLumieres.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration de votre scène, soit de 
// l'ensemble des éclairages ou sorties que vous souhaitez commander.
//
// il contient le mapping entre sorties physiques de l'Arduino et sorties
// logiques (éclairages, servo, buzzer ...) ainsi que les deux séquences 
// executées par l'automate à état fini.
//
// Le programme est livré avec la configuration du batiment administratif
// du dépot diesel appartenant à notre Mlle Pélerin, projet Obourg
//
// Plus d'exemples dans la documention, notamment pour les fonctions les
// avancées comme le poste de soudure, le flash du photographe, ...
//
// === Ce fichier est à adapter en fonction de vos automatismes ========

// ---------------------------------------------------------------------
// --- ICI COMMENCE LA CONFIGURATION EN FONCTION DE VOTRE SCENE      ---

// Affectation des types d'éclairage ou de fonctions à chaque sortie
const byte ledCnf[] = {
 /* D2 */ ETYPE_NEONNEUF,    /* S1 (1)  : bureau administratif (IO) *
 /* D3 */ ETYPE_SOUDURE,     /* S2 (2)  : accueil (PWM) */
 /* D4 */ ETYPE_STANDARD,    /* S3 (4)  : bureau M. CLaude (IO) */
 /* D5 */ ETYPE_GYROPHARE,   /* S4 (8)  : couloir haut (PWM) */
 /* D6 */ ETYPE_FIRE,        /* S5 (16) : escalier (PWM) */
 /* D7 */ ETYPE_NEONNEUF,    /* S6 (32) : couloir bas (IO) */
 /* D8 */ ETYPE_NEONNEUF,    /* S7 (64) : bureau M. Gaston (IO) */
 /* D9 */ ETYPE_NEONVIEUX,   /* S8 (128) ou SM1 : bureau secrétaire (PWM) */
 /* D10 */ ETYPE_STANDARD,   /* S9 (256) ou SM2 : bureau Mlle Pélerin (PWM) */
 /* D11 */ ETYPE_GYROPHARE   /* S10 (512) : gyrophare (PWM) */
};

// Pour la commande d'un servo moteur sur la sortie D9 ou D10 uniquement
// LIGNE CI-APRÈS À METTRE EN COMMENTAIRE SI VOUS N'UTILISEZ AUCUN SERVO
//#include <Servo.h>

// Séquence est une liste de couple (sorties, durée d'allumage en secondes, commande)
// Cf le fichier FSMLumieres.h pour la signification de chaque commande utilisée

// Je peux me définir des symboles pour faciliter la lecture de mon automatisme
#define Administratif S1
#define Accueil       S2
#define Claude        S3
#define CouloirEtage  S4
#define Escalier      S5
#define CouloirRDC    S6
#define Gaston        S7
#define Secretaire    S8
#define LaMiss        S9
#define Incendie      S10

/* Séquence 1 : Miss Pélerin passe (avec ou sans M. Gaston) */
DEBUTSEQ(mySeq1)
  ATTACH(Incendie,E1B)  /* déclenche le gyrophare si l'entrée 1 passe à l'état bas */
  SET(Accueil,10)    /* allume l'accueil (néon neuf) pendant 10 secondes */
  SET(Accueil+CouloirRDC,10) /* laisse l'accueil allumé et allume le couloir RDC (néon neuf) pendant 10 secondes */
  SET(Escalier+CouloirRDC,10) /* éteint l'accueil et allume le couloir RDC (néon neuf) et l'escalier (néon ancien) pendant 10 secondes */
  SET(Escalier+CouloirEtage,10) /* ETC :)  */
  SET(CouloirEtage+LaMiss,10)
  ALEA(Gaston,2)    /* parfois M. gaston accompagne la miss (50% de chance) */  
  ALEA(Secretaire,2)    /* parfois la secrétaire accompagne la miss (50% chance) */
  STANDBY(LaMiss,10) /* la miss reste dans son bureau un certain temps (1 à 10 minutes) */
  SET(CouloirEtage+LaMiss,2)
  SET(CouloirEtage,10)
  SET(CouloirEtage+Escalier,10)
  SET(Escalier+CouloirRDC,10)
  SET(CouloirRDC+Accueil,10)
  SET(Accueil,10)   /* éteint le couloir du RDC, allume l'accueil pendant 10 secondes */
  END          /* fin de la séquence, tout est éteint */
FINSEQ(mySeq1)

/* Séquence 2 : le dépot fonctionne de nuit */
DEBUTSEQ(mySeq2)
  ATTACH(Incendie,E1B) /* déclenche le gyrophare si l'entrée 1 passe à l'état bas */
  SET(Accueil,10)
  PERM(Accueil)
  SET(CouloirRDC,10)  
  SET(CouloirRDC+Administratif,2)
  SET(Administratif,2)
  PERM(Administratif)
  SET(Escalier,10)
  SET(Escalier+CouloirEtage,10)
  PERM(Secretaire)
  ALEA(LaMiss,4)      /* La Miss n'est pas toujours présente la nuit ! */
  ALEA(Gaston,2)  
  ALEA(Claude,2)
  WSTOP(ESTARTB,3)
  UNSET(LaMiss+Secretaire+Gaston+Claude) /* éteint les bureaux de l'étage */
  SET(Escalier,10)
  UNSET(CouloirEtage+Administratif)  /* eteint couloir haut et administratif */
  WAIT(10)
  UNSET(CouloirRDC)   /* éteint couloir bas */
  WAIT(10)
  UNSET(Accueil)     /* éteint accueil */
  END                /* fin de la séquence */
FINSEQ(mySeq2)

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOTRE SCENE    ---
// ---------------------------------------------------------------------
