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
const int ledCnf[] = {
            /* D2 */ ETYPE_NEONNEUF,   /* 1 : bureau administratif (IO) *
            /* D3 */ ETYPE_NEONNEUF,   /* 2 : accueil (PWM) */
            /* D4 */ ETYPE_STANDARD,   /* 4 : bureau M. CLaude (IO) */
            /* D5 */ ETYPE_NEONNEUF,   /* 8 : couloir haut (PWM) */
            /* D6 */ ETYPE_NEONVIEUX,  /* 16 : escalier (PWM) */
            /* D7 */ ETYPE_NEONNEUF,   /* 32 : couloir bas (IO) */
            /* D8 */ ETYPE_NEONNEUF,   /* 64 : bureau M. Gaston (IO) */
            /* D9 */ ETYPE_NEONNEUF,   /* 128 : bureau secrétaire (PWM) */
            /* D10 */ ETYPE_STANDARD,  /* 256 : bureau Mlle Pélerin (PWM) */
            /* D11 */ ETYPE_FIRE        /* 512 : gyrophare (PWM) */
};

// Séquence est une liste de couple (sorties, durée d'allumage en secondes, commande)
// Cf le fichier FSMLumieres.h pour la signification de chaque commande utilisée

/* Séquence 1 : Miss Pélerin passe (avec ou sans M. Gaston) */
const int mySeq1[] = {
                      2,     10, SET, /* allume l'accueil (néon neuf) pendant 10 secondes */
                      2+32,  10, SET, /* laisse l'accueil allumé et allume le couloir RDC (néon neuf) pendant 10 secondes */
                      16+32, 10, SET, /* éteint l'accueil et allume le couloir RDC (néon neuf) et l'escalier (néon ancien) pendant 10 secondes */
                      16+8,  10, SET, /* ETC :)  */
                      8+256, 10, SET,
                      64,    2, ALEA, /* parfois M. gaston accompagne la miss (50% de chance) */  
                      128,   2, ALEA, /* parfois la secrétaire accompagne la miss (50% chance) */
                      256,   10, STANDBY, /* la miss reste dans son bureau un certain temps (1 à 10 minutes) */
                      8+256, 2, SET,
                      8,     10, SET,
                      8+16,  10, SET,
                      16+32, 10, SET,
                      32+2,  10, SET,
                      2,     10, SET, /* éteint le couloir du RDC, allume l'accueil pendant 10 secondes */
                      0,     0,  END  /* fin de la séquence, tout est éteint */
};

/* Séquence 2 : le dépot fonctionne de nuit */
const int mySeq2[] = {
                      2,     10, SET,
                      2,     0,  PERM,
                      32,    10, SET,
                      32+1,  2,  SET,
                      1,     2,  SET,
                      1,     0,  PERM,
                      16,   10,  SET,
                      16+8, 10,  SET,
                      8,     0,  PERM,  
                      256,   4,  ALEA,
                      64,    2,  ALEA,
                      4,     2,  ALEA,
                      128,   2,  ALEA,
                      0,     3,  WSTOP, /* attend que le bouton Jour/Nuit passe à LOW, test toutes les 3 secondes */
                      256+128+64+4, 0, UNSET, /* éteint les bureaux de l'étage */
                      16,   10,  SET,
                      8+1,   0, UNSET,  /* eteint couloir haut et administratif */
                      0,     10, WAIT,
                      32,    0, UNSET, /* éteint couloir bas */
                      0,     10, WAIT,
                      1,     0, UNSET, /* éteint accueil */
                      0,     0,  END   /* fin de la séquence */
};

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOTRE SCENE    ---
// ---------------------------------------------------------------------
