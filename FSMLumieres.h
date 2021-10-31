// Lumieres::FSMlumieres.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la machine à état fini pour gérer le batiment, la scène
// et plus généralement un ensemble d'éclairages

// Pour définir une séquence, on doit pour chaque éclairage définir une durée
// d'allumage avant extinction, et il existe des commandes particulières :

const byte _UNSET  = -1; /* force l'extinction d'un éclairage, notamment précédemment marqué comme permanent */
const byte _SET     = 0; /* positionne les éclairages selon la durée définie */
const byte _STANDBY = 1; /* durée par tirage aléatoire en minutes (1 à n minutes) */
const byte _PERM    = 2; /* durée permanente : jusqu'à l'arrêt de l'Arduino ou le changement de séquence */
const byte _ALEA    = 3; /* durée aléatoire : permet de rendre aléatoire la présence d'une personne dans un bureau la nuit, aligné sur la commande précédente : STANDBY ou PERM */
const byte _WAIT    = 4; /* pause d'une durée définie, ne touche à aucun éclairage */
const byte _WSTOP   = 5; /* pause d'une durée indéfinie, ne touche à aucun éclairage, attends que l'entrée startPin passe à l'état LOW */
const byte _PWM     = 6; /* envoie le signal sur la sortie si compatible : D3, D5, D6, D9, D10 et D11 */
const byte _END    = 99; /* marqueur de fin de séquence */
const byte _LOOP   = 88; /* marqueur de boucle pour redémarrer sur le MARK */
const byte _MARK   = 77; /* place le marqueur pour le LOOP */

// Pointeur vers la séquence en cours, une séquence étant une suite de couple (éclairages, durée, commande)
int* gpSeq;

// Pointeur vers la mark
int* gpMarkSeq;

// machine à état des séquences
const byte START    = 0; /* état initial */
const byte RUNNING  = 1; /* état courant */
const byte STOP    = 99; /* état final, atteint lorsaue la FSM rencontre le marqueur END */

// état de la FSM : START, RUNNING ou STOP
byte gSeqState;

// état de l'entrée notifiant qu'il faut démarrer ou stopper la FSM
bool gCurrentStateStartPin;

// parametres de l'automate
struct {
    long int duration;  /* durée avant la transition */
    int leds;           /* leds concernés par la transition */
    byte command;       /* commande associée */
} gSeq;

// ---------------------------------------------------------------------
// Des séquences activées en mode debug, pour tester le programme et/ou
// tester les éclairages du batiment
// ---------------------------------------------------------------------

// Séquence 1 : un simple chenillard sur les sorties D3, D5 et D6 , c'est bien !
// Je mixe volontairement macro langage et structure de données
DEBUTSEQ(seqDebug1)
  MARK            /* place une MARK pour le LOOP */
  PERM(S10)       /* Allume D11 le gyrophare */
   2,      10, _SET,  /* Allume D3 (éclairage standard) pendant 10 secondes */
   8,      10, _SET,  /* Eteint D3 et Allume D5 (néon neuf) pendant 10 secondes */
   16,     10, _SET,  /* Eteint D4 et Allume D6 (néon ancien) pendant 10 secondes */
   0,       5, _WAIT, /* pause pendant 5 secondes, ne touche pas au lumières allumées */
   0,      10, _SET,  /* Eteint D5 et attends 10 secondes */
   2+8+16, 10, _SET,  /* Allume D3, D5 et D6 pendant 10 secondes */
  UNSET(S10)  /* Arrete le gyrophare */
  LOOP        /* Tout est éteint. Boucle sur le début MARK */
FINSEQ(seqDebug1)

/* Séquence 2 :  un chenillard sur l'ensemble des sorties, c'est encore mieux ! */
DEBUTSEQ(seqDebug2)
    MARK
    1,      10, _SET,
    2,      10, _SET,
    4,      10, _SET,
    8,      10, _SET,
    16,     10, _SET,
    32,     10, _SET,
    64,     10, _SET,
    128,    10, _SET,
    256,    10, _SET,
    512,    10, _SET,
    0,      10, _SET,   /* Eteint tout pendant 10 secondes */
    1023,   10, _SET,   /* Allume tout pendant 10 secondes */
    LOOP
FINSEQ(seqDebug2)
