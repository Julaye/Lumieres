// Lumieres::ConfigLumieres.h
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// Ce fichier propose un macro langage pour faciliter l'écriture d'automatismes
// Se reporter à la documentation
// ---------------------------------------------------------------------------

// DEBUT / FIN d'une séquence
#define DEBUTSEQ(_seq)   const byte _seq[] = {
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
#define DETACH(IO)            IO,0,_DETACH,
#define UNTIL(IO,TIME)        IO,TIME,_UNTIL,
#define END                   0,0,_END,
#define MARK                  0,0,_MARK,
#define LOOP                  0,0,_LOOP,
#define RESET                 0,0,_RESET,
#define SETMODE(IO,MODE)      IO,MODE,_SETMODE,          

// Il est aussi utile de définir les sorties et les entrées avec des noms explicites
#define S1    1
#define S2    2
#define S3    4
#define S4    8
#define S5    16
#define S6    32
#define S7    64
#define S8    128
#define SALL  255

// entrées à l'état Bas
#define ESTARTB   0
#define E1B       1
#define E2B       2
#define E3B       3
#define E4B       4
#define E1andE2B  5
#define E1orE2B   6
#define E1xorE2B  7

// entrées à l'état Haut
#define ESTARTH 0x80
#define E1H     0x81
#define E2H     0x82
#define E3H     0x83
#define E4H     0x84
#define E1andE2H 0x85
#define E1orE2H  0x86
#define E1xorE2H 0x87
