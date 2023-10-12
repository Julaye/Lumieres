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
// =====================================================================

// === Ce fichier est à adapter en fonction de vos automatismes ========

// ---------------------------------------------------------------------
// --- ICI COMMENCE LA CONFIGURATION EN FONCTION DE VOTRE SCENE      ---

// Affectation des types d'éclairage ou de fonctions à chaque sortie
const byte myLedCnf[] = {
  /* D2 */ ETYPE_STANDARD,   /* S1 (1)  : sortie standard (IO) *
 /* D3 */ ETYPE_STANDARD,   /* S2 (2)  : sortie standard (PWM) */
 /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : sortie standard (IO) */
 /* D5 */ ETYPE_STANDARD,   /* S4 (8)  : sortie standard (PWM) */
 /* D6 */ ETYPE_STANDARD,   /* S5 (16) : sortie standard (PWM) */
 /* D9 */ ETYPE_STANDARD,   /* S6 (32) ou SM1 : sortie standard (PWM)  */
 /* D10 */ ETYPE_STANDARD,  /* S7 (64) ou SM2 : sortie standard (PWM) */
 /* D11 */ ETYPE_STANDARD   /* S8 (128) : sortie standard (PWM) */
};

// Séquence est une liste de couple (sorties, durée d'allumage en secondes, commande)
// Cf le fichier FSMLumieres.h pour la signification de chaque commande utilisée
#define Lampe1  S1
#define Lampe2  S2

/* Séquence 1 : */
DEBUTSEQ(myProg_Seq1)
  SET(Lampe1,10)    
  SET(Lampe2,10)
  STANDBY(Lampe2,10) /* la miss reste dans son bureau un certain temps (1 à 10 minutes) */
  END                /* fin de la séquence, tout est éteint */
FINSEQ(mySeq1)

/* Séquence 2 : le dépot fonctionne de nuit */
DEBUTSEQ(myProg_Seq2)
  SET(Lampe1,10)    
  ALEA(Lampe2,4)     /* La Miss n'est pas toujours présente la nuit ! */
  END                /* fin de la séquence */
FINSEQ(mySeq2)

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOTRE SCENE    ---
// ---------------------------------------------------------------------
