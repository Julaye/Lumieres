// Batiment01::FSMbatiment.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la machine à état fini pour gérer le batiment
//

// Pour définir une séquence, on doit pour chaque éclairage définir une durée
// d'allumage avant extinction, et il existe des commandes particulières :

const int UNSET  = -1; /* force l'extinction d'un éclairage, notamment précédemment marqué comme permanent */
const int SET     = 0; /* positionne les éclairages selon la durée définie */
const int STANDBY = 1; /* durée par tirage aléatoire en minutes (1 à n minutes) */
const int PERM    = 2; /* durée permanente : jusqu'à l'arrêt de l'Arduino ou le changement de séquence */
const int ALEA    = 3; /* durée aléatoire : permet de rendre aléatoire la présence d'une personne dans un bureau la nuit, aligné sur la commande précédente : STANDBY ou PERM */
const int WAIT    = 4; /* pause d'une durée définie, ne touche à aucun éclairage */
const int WSTOP   = 5; /* pause d'une durée indéfinie, ne touche à aucun éclairage, attends que l'entrée startPin passe à l'état LOW */
const int END    = 99; /* marqueur de fin de séquence */
const int LOOP   = 88; /* marqueur de boucle pour redémarrer sur le MARK */
const int MARK   = 77; /* place le marqueur pour le LOOP */

// Pointeur vers la séquence en cours, une séquence étant une suite de couple (éclairages, durée, commande)
long int* gpSeq;

// Pointeur vers la mark
long int* gpMarkSeq;

// machine à état des séquences
const int START    = 0; /* état initial */
const int RUNNING  = 1; /* état courant */
const int STOP    = 99; /* état final, atteint lorsaue la FSM rencontre le marqueur END */

// état de la FSM : START, RUNNING ou STOP
int gSeqState;

// état de l'entrée notifiant qu'il faut démarrer ou stopper la FSM
bool gCurrentStateStartPin;

// parametres de l'automate
struct {
    long int duration;     /* durée avant la transition */
    long int prevDuration; /* durée précédente */
    long int leds;         /* leds concernés par la transition */
    int command;           /* commande associée */
} gSeq;

// ---------------------------------------------------------------------
// Des séquences activées en mode debug, pour tester le programme et/ou
// tester les éclairages du batiment
// ---------------------------------------------------------------------

/* Séquence 1 : un simple chenillard sur les sorties D3, D4 et D7 , c'est bien ! */
long int seqDebug1[] = {
   0,      0,  MARK, /* place une MARK pour le LOOP */
   2,      10, SET,  /* Allume D3 (éclairage standard) pendant 10 secondes */
   4,      10, SET,  /* Eteint D3 et Allume D4 (néon neuf) pendant 10 secondes */
   32,     10, SET,  /* Eteint D4 et Allume D7 (néon ancien) pendant 10 secondes */
   0,       5, WAIT, /* pause pendant 5 secondes, ne touche pas au lumières allumées */
   0,      10, SET,  /* Eteint D7 et attends 10 secondes */
   2+4+32, 10, SET,  /* Allume D3, D4 et D7 pendant 10 secondes */
   0,      0,  LOOP  /* Tout est éteint. Boucle sur le début MARK */
};

/* Séquence 2 :  un chenillard sur l'ensemble des sorties, c'est encore mieux ! */
long int seqDebug2[] = {
    0,      0,  MARK, /* place une MARK pour le LOOP */
    1,      10, SET,
    2,      10, SET,
    4,      10, SET,
    8,      10, SET,
    16,     10, SET,
    32,     10, SET,
    64,     10, SET,
    128,    10, SET,
    256,    10, SET,
    512,    10, SET,
    0,      10, SET,   /* Eteint tout pendant 10 secondes */
    1023,   10, SET,   /* Allume tout pendant 10 secondes */
    0,      0,  LOOP  /* Tout est éteint. Boucle sur le début MARK */
};
