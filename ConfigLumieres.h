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

// Séquence est une liste de couple (sorties, durée d'allumage en secondes, commande)
// Cf le fichier FSMLumieres.h pour la signification de chaque commande utilisée
#define Lampe1  S1
#define Lampe2  S2

/* Prog 6 (Séquence 1) : */
DEBUTSEQ(myProg_Seq1)
  SET(Lampe1,10)    
  SET(Lampe2,10)
  STANDBY(Lampe2,10) /* la miss reste dans son bureau un certain temps (1 à 10 minutes) */
  END                /* fin de la séquence, tout est éteint */
FINSEQ(mySeq1)

/* Prog 7 (Séquence 2) : */
DEBUTSEQ(myProg_Seq2)
  SET(Lampe1,10)    
  ALEA(Lampe2,4)     /* La Miss n'est pas toujours présente la nuit ! */
  END                /* fin de la séquence */
FINSEQ(mySeq2)

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOTRE SCENE    ---
// ---------------------------------------------------------------------
