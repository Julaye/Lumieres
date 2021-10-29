// Batiment01::ConfigNano.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la configuration du batiment ou de l'ensemble
// d'éclairages que vous souhaitez commander.
//
// il contient le mapping entre sorties et type d'éclairages ainsi
// que les deux séquences à utiliser.
//
// Le programme est livré avec la configuration du batiment administratif
// du dépot diesel appartenant à notre Mlle Pélerin

// ---------------------------------------------------------------------
// --- ICI COMMENCE LA CONFIGURATION EN FONCTION DE VOTRE BATIMENT ---

// Affectation des types d'éclairage à chaque sortie
const int ledCnf[] = {
            /* D2 */ ETYPE_STANDARD,   /* 1 : bureau administratif *
            /* D3 */ ETYPE_STANDARD,   /* 2 : bureau M. Gaston */
            /* D4 */ ETYPE_NEONNEUF,   /* 4 : accueil */
            /* D5 */ ETYPE_STANDARD,   /* 8 : bureau M. CLaude */
            /* D6 */ ETYPE_NEONNEUF,   /* 16 : couloir bas */
            /* D7 */ ETYPE_NEONVIEUX,  /* 32 : escalier */
            /* D8 */ ETYPE_NEONNEUF,   /* 64 : couloir haut */
            /* D9 */ ETYPE_STANDARD,   /* 128 : bureau secrétaire */
            /* D10 */ ETYPE_STANDARD,  /* 256 : bureau Mlle Pélerin */
            /* D11 */ ETYPE_NOTUSED,   /* 512 : non utilisé */
};

// Séquence est une liste de couple (sorties, durée d'allumage en secondes, commande)
// Cf le fichier FSMBatiment.h pour la signification de chaque marqueur

/* Séquence 1 : Miss Pélerin passe (avec ou sans M. Gaston) */
long int mySeq1[] = {
                      4,     10, SET, /* allume l'accueil (néon neuf) pendant 10 secondes */
                      4+16,  10, SET, /* laisse l'accueil allumé et allume le couloir RDC (néon neuf) pendant 10 secondes */
                      16+32, 10, SET, /* éteint l'accueil et allume le couloir RDC (néon neuf) et l'escalier (néon ancien) pendant 10 secondes */
                      32+64, 10, SET, /* ETC :)  */
                      64+256,10, SET,
                      2,     2, ALEA, /* parfois M. gaston accompagne la miss (50% de chance) */  
                      128,   2, ALEA, /* parfois la secrétaire accompagne la miss (50% chance) */
                      256,   10, STANDBY, /* la miss reste dans son bureau un certain temps (1 à 10 minutes) */
                      64+256, 2, SET,
                      64,    10, SET,
                      64+32, 10, SET,
                      32+16, 10, SET,
                      16+4,  10, SET,
                      4,     10, SET, /* éteint le couloir du RDC, allume l'accueil pendant 10 secondes */
                      0,     0,  END  /* fin de la séquence, tout est éteint */
};

/* Séquence 2 : le dépot fonctionne de nuit */
long int mySeq2[] = {
                      4,     10, SET,
                      4,     0,  PERM,
                      16,    10, SET,
                      16+1,  0,  PERM,
                      32,    10, SET,
                      32,    0,  PERM,
                      64,    10, SET,
                      64+128,0,  PERM,  
                      256,   2,  ALEA,
                      8,     2,  ALEA,
                      2,     2,  ALEA,
                      0,     3,  WSTOP, /* attend que le bouton Jour/Nuit passe à LOW, test toutes les 3 secondes */
                      1023,  0,  UNSET, /* eteint toutes les lumieres */
                      0,     0,  END    /* fin de la séquence */
};

// --- ICI SE TERMINE LA CONFIGURATION EN FONCTION DE VOTRE BATIMENT ---
// ---------------------------------------------------------------------
