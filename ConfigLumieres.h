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
// === L'exemple fournit en standard concerne l'animation des lampes ===
// === d'une fosse d'inspection ... ====================================
//
// En voici les spécifications :
// 1. fosse normalement éteinte sauf si forçage par entrée digital (M83 - bouton vert - au commun==GND)
// 2. une locomotive passe sans s’arrêter, la fosse reste éteinte sauf si le forçage est en cours
// 3. une locomotive stationne, au bout d’un temps programmable, la fosse s’allume
// 4. la fosse allumée s’éteint au bout d’un certain temps programmable, économie d’énergie oblige
// 5. une locomotive repart, la fosse allumée s’éteint au bout d’un temps programmable
// 6. entrée digital pour un forçage éteint même si des locomotives stationnent (M83 - bouton rouge - au commun==GND)

// ---------------------------------------------------------------------
// --- ICI COMMENCE LA CONFIGURATION EN FONCTION DE VOTRE SCENE      ---

/* Prog 6 (Séquence 1) : */
DEBUTSEQ(myProg_Seq1)
  SETMODE(S2,ETYPE_STANDARD)  /* relai pour allumer les lampes */
  
  ATTACH(E2B,S2)    /* force l'allumage sur un lien */
  ATTACH(E3H,S2)    /* force l'extinction sur un autre lien */

  WSTOP(E1B,10)     /* loco avérée pendant 10 secondes */
  UNTIL(S2,2)       /* allume S2 pendant 2 minutes */
  WSTOP(E1H,10)     /* loco s'en va, attend 10 secondes */
  UNSET(S2)         /* au cas où S2 soit encore allumé */

  END                /* fin de la séquence, tout est éteint */
FINSEQ(mySeq1)

/* Prog 7 (Séquence 2) : */
DEBUTSEQ(myProg_Seq2)
  END                /* fin de la séquence */
FINSEQ(mySeq2)

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOTRE SCENE    ---
// ---------------------------------------------------------------------
