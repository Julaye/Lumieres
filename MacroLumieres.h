// Lumieres::ConfigLumieres.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier propose un macro langage pour facilier l'écriture d'automatismes
// Se reporter à la documentation
// ---------------------------------------------------------------------------

// DEBUT / FIN d'une séquence
#define DEBUTSEQ(_seq)   const int _seq[] = {
#define FINSEQ(_seq)     }; /* _seq[] */

// Toutes les macros instructions ont la structure suivante :
// _COMMANDE(IO,DURATION);
//
// COMMANDE : UNSET, SET, PERM, ... END
// IO : paramètre de la ou des sorties concernées, parfois une entrée (commande WSTOP par exemple)
// DURATION : paramètre de la commande, le plus souvent un délai en secondes mais pas toujours

#define UNSET(IO)             IO,0,_UNSET,
#define SET(IO,DURATION)      IO,DURATION,_SET,
#define STANDBY(IO,DURATION)  IO,DURATION,_STANDBY,
#define PERM(IO)              IO,0,_PERM,
#define ALEA(IO,MAX)          IO,MAX,_ALEA,
#define WAIT(DELAI)           0,DELAI,_WAIT,
#define WSTOP(IO,TIME)        IO,TIME,_WSTOP,
#define PWM(IO,DUTY)          IO,DUTY,_PWM,
#define ATTACH(IO,IN)         IO,IN,_ATTACH,
#define DETACH(IO)            IO,0,_DETACH
#define END                   0,0,_END,
#define MARK                  0,0,_MARK,
#define LOOP                  0,0,_LOOP,          

// Il est aussi utile de définir les sorties et les entrées avec des noms explicites
#define S1    1
#define S2    2
#define S3    4
#define S4    8
#define S5    16
#define S6    32
#define S7    64
#define S8    128
#define S9    256
#define S10   512
#define SALL  1023

// entrées à l'état Bas
#define ESTARTB 0
#define E1B     1
#define E2B     2
#define E3B     3
#define E4B     4

// entrées à l'état Haut
#define ESTARTH 0x80
#define E1H     0x81
#define E2H     0x82
#define E3H     0x83
#define E4H     0x84
