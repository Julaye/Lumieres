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

// Prog 6 : Lumières d'une fosse d'inspection
// En voici les spécifications :
// 1. fosse normalement éteinte sauf si forçage par entrée digital (M83 - bouton vert - au commun==GND)
// 2. une locomotive passe sans s’arrêter, la fosse reste éteinte sauf si le forçage est en cours
// 3. une locomotive stationne, au bout d’un temps programmable, la fosse s’allume
// 4. la fosse allumée s’éteint au bout d’un certain temps programmable, économie d’énergie oblige
// 5. une locomotive repart, la fosse allumée s’éteint au bout d’un temps programmable
// 6. entrée digital pour un forçage éteint même si des locomotives stationnent (M83 - bouton rouge - au commun==GND)

DEBUTSEQ(myProg_Seq1)
  SETMODE(S2,ETYPE_STANDARD)  /* relai pour allumer les lampes */
  
  ATTACH(E2B,S2)    /* force l'allumage sur un lien */
  ATTACH(E3B,S2)    /* force l'extinction sur un autre lien */

  0,5,_MARK,
    WSTOP(E1B,10)     /* loco avérée pendant 10 secondes */
    UNTIL(S2,4)       /* allume S2 pendant 4 minutes */
    WSTOP(E1H,10)     /* loco s'en va, attend 10 secondes */
    UNSET(S2)         /* au cas où S2 soit encore allumé */
  LOOP
FINSEQ(mySeq1)

// Prog 7 : Automatismes d'un atelier

DEBUTSEQ(myProg_Seq2)
  SETMODE(S1,ETYPE_NEONNEUF) /* S1 : lampe de l’atelier (IO) */
  SETMODE(S2,ETYPE_NEONNEUF) /* S2 : néon de l’atelier (PWM) */

  SETMODE(S4,ETYPE_SOUDURE)  /* S4 : poste de soudure de l’atelier (PWM) */

  SETMODE(S6,ETYPE_BUZZER)    /* S6 : buzzer du poste de soudure (PWM) */
  SETMODE(S7,ETYPE_SERVO)     /* S7 : servomoteur de la porte (PWM) */
  SETMODE(S8,ETYPE_GYROPHARE) /* S8 : gyrophare de la porte (PWM) */

  PERM(S1+S2) /* l’atelier se met au travail au démarrage de l’Arduino … */

  0,5,_MARK,  /* point de retour de l’automatisme (et timeout à 5 minutes) */  
    WSTOP(E1orE2B,1)  /* attend la détection d’une locomotive sur la voie 1 ou voie 2 */
    UNTIL(S8,2)   /* lance le gyrophare configuré sur la sortie D11 pour deux minutes */
    WAIT(5)       /* attend 5 secondes */
    UNTIL(S7,2)   /* envoie 90° sur la sortie D10 du servomoteur pour deux minutes */
    WSTOP(E1andE2H,1)  /* attend la non détection de la locomotive sur la voie 1 ou voie 2 */
    UNSET(S7)     /* envoie 0° sur la sortie D10 du servomoteur */
    WAIT(5)       /* attend 5 secondes */
    UNSET(S8)     /* arrête le gyrophare configuré sur la sortie D11 */

    WSTOP(E2B,1)  /* attend la détection d’une locomotive sur la voie 2 */

    0,5,_MARK,    /* point de retour de l’automatisme (et timeout à 5 minutes) */ 
      WAIT(5)       /* 5 secondes */
      SET(S4+S6,15) /* un peu de soudure sur la locomotive qui vient de rentrer pendant 15 secondes */
      UNTIL(S7,2)   /* envoie 90° sur la sortie D10 du servomoteur pour deux minutes */
    
    WSTOP(E2H,1)  /* attend la non détection de la locomotive sur la voie 2 */
    UNSET(S4+S6)  /* arrête le poste soudure, la loco est partie ! */
    UNSET(S7)     /* envoie 0° sur la sortie D10 du servomoteur */
    WAIT(5)       /* 5 secondes */
    
  RESET    /* boucle attendre la locomotive suivante */
FINSEQ(mySeq2)

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOTRE SCENE    ---
// ---------------------------------------------------------------------
