// Lumieres::FSMlumieres.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier concerne la machine à état fini pour gérer l'automatisme, la scène
// et plus généralement un ensemble d'éclairages

// === En principe vous n'avez pas à modifier ce fichier =======================

// Pour définir une séquence, on doit pour chaque éclairage définir une durée
// d'allumage avant extinction, et il existe des commandes particulières :

const byte _UNSET  = -1; /* force l'extinction d'un éclairage, notamment précédemment marqué comme permanent */
const byte _SET     = 0; /* lance les éclairages selon la durée définie */
const byte _STANDBY = 1; /* durée par tirage aléatoire en minutes (1 à n minutes) */
const byte _PERM    = 2; /* durée permanente : jusqu'à l'arrêt de l'Arduino ou le changement de séquence */
const byte _ALEA    = 3; /* durée aléatoire : permet de rendre aléatoire la présence d'une personne dans un bureau la nuit, aligné sur la commande précédente : STANDBY ou PERM */
const byte _WAIT    = 4; /* pause d'une durée définie, ne touche à aucun éclairage */
const byte _WSTOP   = 5; /* pause d'une durée indéfinie, ne touche à aucun éclairage, attends que l'entrée startPin passe à l'état LOW */
const byte _PWM     = 6; /* envoie le signal sur la sortie si compatible : D3, D5, D6, D9, D10 et D11 */
const byte _ATTACH  = 7; /* attache sortie(s) et entrée et gère les éclairages selon l'état de cette entrée */
const byte _DETACH  = 8; /* supprime le lien existant entre éclairages et l'état d'une entrée */
const byte _SETMODE = 9; /* change le mode de configuration d'une sortie - cf ETYPE_xxx */
const byte _END    = 99; /* marqueur de fin de séquence */
const byte _LOOP   = 88; /* marqueur de boucle pour redémarrer sur le MARK */
const byte _MARK   = 77; /* place le marqueur pour le LOOP */

// Pointeur vers la séquence en cours, une séquence étant une suite de couple (éclairages, durée, commande)
byte* gpSeq;

// Configuration des E/S (valeurs par défaut)
byte gCnf[maxOutputs] = {
 /* D2 */ ETYPE_STANDARD,   /* S1 (1)  : sortie standard (IO) *
 /* D3 */ ETYPE_STANDARD,   /* S2 (2)  : sortie standard (PWM) */
 /* D4 */ ETYPE_STANDARD,   /* S3 (4)  : sortie standard (IO) */
 /* D5 */ ETYPE_STANDARD,   /* S4 (8)  : sortie standard (PWM) */
 /* D6 */ ETYPE_STANDARD,   /* S5 (16) : sortie standard (PWM) */
 /* D9 */ ETYPE_STANDARD,   /* S6 (32) ou SM1 : sortie standard (PWM)  */
 /* D10 */ ETYPE_STANDARD,  /* S7 (64) ou SM2 : sortie standard (PWM) */
 /* D11 */ ETYPE_STANDARD   /* S8 (128) : sortie standard (PWM) */
};

// Pointeur vers la mark
byte* gpMarkSeq;

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
    byte leds;          /* leds concernés par la transition */
    byte command;       /* commande associée */
} gSeq;

// anti rebond des entrées
byte inputState[maxInputPins];
byte inputCount[maxInputPins];
