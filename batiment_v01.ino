// Batiment01
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// La gestion d'un batiment entier avec un petit Arduino Nano
//  v20211027 - première implémentation, mise au point, mode debug
//  v20211028 - mise au point, ajout de commandes (WAIT, WSTOP, UNSET), nettoyage du code, mode debug / verbose
//  v20211029 - utilisation de la BUILTIN led pour indiquer le bon fonctionnement du séquenceur / automate, nettoyage des traces verbose
//  v20211029.2 - Fixe un problème sur la commande ALEA suite à des tests intensifs
//
//  brancher des micro-leds de type 2,9 V sur GND et D2 à D11, protégée par une résistance svp ! 
//  utilisez le calculateur de résistance svp  https://www.digikey.fr/fr/resources/conversion-calculators/conversion-calculator-led-series-resistor
//  pas plus de 40 mA par sortie, pas plus de 200 mA au total, au risque de griller une sortie et/ou l'Arduino Nano
//
//  l'intensité globale est réglable sur les sorties D3, D5, D6, D9, D10 et D11 (cf PWM_FOR_LED)
//
//  le mode de fonctionnement (néon ancien, néon récent, lampe à incadescence standard) est configurable pour chaque sortie (cf ledCnf[])
//
//  D14 est une entrée qui permet de choisir au démarrage entre la séquence 1 (cf seq1) et la séquence 2 (cf seq2)
//  D15 est une entrée qui permet de lancer ou relancer la séquence
//
//  les séquences sont configurables avec une petite logique temporelle
//  si vous touchez un autre paramètre, c'est à vos risques et périls :) 
//
// Retrouvez ce tutoriel sur le lien : https://forum.3rails.fr/t/une-debutante-dans-le-decor-ep13-que-la-lumiere-jaillisse/20889
// La documentation est accessible ici : https://docs.google.com/document/d/1chZwYiFPMKlHFLLsJ7WHy0EHtaLVpebEK52L9wi9J30/

#include <Arduino.h>

// Ce fichier concerne la configuration matérielle Arduino utilisée
#include "ConfigNano.h"

// Ce fichier concerne la machine à état fini pour gérer un éclairage
#include "FSMeclairage.h"

// Ce fichier permet de simuler un néon
#include "SimuNeon.h"

// Ce fichier concerne la machine à état fini pour gérer le batiment
#include "FSMbatiment.h"

// mettre à 1 pour un debug dans la console série, 2 pour full debug
const int debug = 0;

// mettre à 1 pour rendre l'exécution de l'automate verbeux
const int verbose = 1;

// la configuration de votre batiment
#include "ConfigBatiment.h"

// ---------------------------------------------------------------------
// --- initFSM()
// ---------------------------------------------------------------------

void initFSM()
{ 
  int seq;

  // Initialise l'automate de chaque sortie
  for (int i = 0 ; i < maxLights; i++) gEStateRunning[i] = estate_OFF;

  // récupère la séquence pour le mode RUNNING
  seq = digitalRead(seqPin);
  Serial.print("Séquence : ");
  Serial.print(seq+1);
  
  if (debug) {
    Serial.println(" - Debug");
    (seq==LOW)?gpSeq=seqDebug1:gpSeq=seqDebug2;
  } else {
    Serial.println(" - Normal");
    (seq==LOW)?gpSeq=mySeq1:gpSeq=mySeq2;
  }

  // mark par défaut sur le début de la séquence
  gpMarkSeq = gpSeq;

  // initialise les différents états
  gSeqState = START;
  gCurrentStateStartPin = false;

  gSeq.duration = millis();
  gSeq.leds = 0;
  gSeq.command = SET;
}

// ---------------------------------------------------------------------
// --- setup()
// ---------------------------------------------------------------------

void setup() {
  // code de démarrage executé une seule fois à la mise sous tension de la carte
  
  // ouvre le port série (console de l'outil) avec la vitesse 57600 bauds
  // attention que le paramètre sur la console soit bien 57600 !
  Serial.begin(57600);

  // indicateur que la séquence est vivante !
  pinMode(LED_BUILTIN,OUTPUT);

  // Ports digitaux programmables (PWM) : D3, D5, D6, D9, D10 et D11
  for (int i = 0 ; i < maxLights; i++) pinMode(i+2,OUTPUT);

  // pin pour choisir la séquence
  pinMode(seqPin,INPUT_PULLUP);

  // pin pour démarrer / stopper la FSM générale
  pinMode(startPin,INPUT_PULLUP);

  // la germe du générateur aléatoire
  randomSeed(analogRead(0));

  // Annonce la version
  Serial.println("Batiment v01 20211029 - (c) Julie Dumortier - Licence GPL");

  // initialize la FSM
  Serial.print("HW RESET -> INIT ");
  initFSM();
 }

// ---------------------------------------------------------------------
// Fonctions de support pour afficher l'état des leds impactés par une
// commande et l'état courant des leds.
// ---------------------------------------------------------------------

long int mapLeds = 0;
long int prevMapLeds = 0;

void printCmd(long int leds,bool timing=false)
{
  int pos = 1;

  if (timing) {
    long int t = millis();
    Serial.print("t");
    Serial.print(t/1000);
    Serial.print(".");
    Serial.print((t/10) % 100);
  }
  Serial.print(" [ ");
  for (int i=0; i<maxLights; i++) {
    if (leds&pos) Serial.print("X "); else Serial.print("_ ");
    pos = pos << 1 ;
  }
  Serial.println("] ");
}

// pour pouvoir tester les automates en l'absence des branchements :)
void displayLeds(bool timing=false)
{
  // ne pas afficher si rien ne change !
  if (prevMapLeds==mapLeds) return;
  prevMapLeds = mapLeds;

  // sinon affiche
  printCmd(mapLeds,timing);
}

// ---------------------------------------------------------------------
// Fonctions de support pour allumer / éteindre une led
// ---------------------------------------------------------------------

void unset(int led)
{
  switch (outputMode[led]) {
    case MODE_IO: digitalWrite(2+led,LOW); break;
    case MODE_PWM: analogWrite(2+led,0); break;
    default: break;
  }

  if (verbose) {
    int pos = (1 << led);
    mapLeds &= ~pos;
    displayLeds(true);
  }
}

void set(int led, int value)
{
  if (value==LIGHT_OFF) {
    // indicateur que la séquence est vivante !
    digitalWrite(LED_BUILTIN,HIGH);

    unset(led);
    return;
  }

  // indicateur que la séquence est vivante !
  digitalWrite(LED_BUILTIN,LOW);
  
  switch (outputMode[led]) {
    case MODE_IO: digitalWrite(2+led,HIGH); break;
    case MODE_PWM: analogWrite(2+led,value); break;
    default: break;
  }

  if (verbose) {
    int pos = (1 << led);
    mapLeds |= pos;
    displayLeds(true);
  }
}

// ---------------------------------------------------------------------
// Fonctions de support pour la FSM éclairage d'une led
// ---------------------------------------------------------------------

// eteint les lumières
void lightOff(int led)
{
  if ((debug>1) && (gEStateRunning[led] != estate_OFF)) Serial.println("estate_OFF");
  
  // eteint la led
  unset(led);

  // passe en mode démarrage (au cas où)
  gEStateRunning[led] = estate_OFF;
}

// démarre l'allumage des lumières
void lightStartPowerUp(int led)
{
  if (debug>1) Serial.println("state_STPWRUP");

  // si la led est déjà allumée, ne rien faire
  if (gEStateRunning[led] == estate_ON) return;

  // passe en mode démarrage
  gEStateRunning[led] = estate_PWRUP;
  if (debug>1) Serial.print("stateRunning(");
  if (debug>1) Serial.print(led);
  if (debug>1) Serial.println(")<-- PWRUP");

  // prépare la séquence
  gEStatePwrup[led] = 0;
  gEStateDelay[led] = 0;
}

// allume les lumières
void lightOn(int led)
{
    int alea;
    
    set(led,PWM_FOR_LED); 

    if (ledCnf[led]==ETYPE_NEONVIEUX) {
      // fait un tirage aléatoire et refait un glitch à partir d'une séquence prédéterminée
      // ajuster la valeur random en fonction de la fréquence d'apparition (en ms)
      alea = random(0,10000);
      if (alea < 1) {
        if (verbose) {
          Serial.print("Glitch led:");
          Serial.println(led);
        }
        gEStateRunning[led] = estate_PWRUP;
        gEStatePwrup[led] = INDEX_GLITCHPWRUP;
        gEStateDelay[led] = 0;
      }
    }
}

// gère une séquence d'allumage
void lightPowerUp(int led)
{
    if (debug>1) {
      Serial.print("state_PWRUP(led:");
      Serial.print(led);
      Serial.print(",delay:");
      Serial.print(gEStateDelay[led]);
    }

    // un eclairage standard : il suffit de l'allumer en le passant à l'état ON
    if (ledCnf[led]==ETYPE_STANDARD) {
         gEStateRunning[led] = estate_ON;
         if (debug>1) Serial.println(") STANDARD -> state_ON");
         return; 
    } 

    // vérifier si le delai de la transition est écoulé
    if (gEStateDelay[led]<=0) 
    { 
        if (debug>1) {
          Serial.print(") - Blink sequence:");
          Serial.print(gEStatePwrup[led]);
          Serial.print(" intensity: ");
        }
        
        if (debug>1) Serial.print(blinkOn[gEStatePwrup[led]].intensity);

        gEStateDelay[led] = blinkOn[gEStatePwrup[led]].duration;
        if (debug>1) Serial.print(" duration: ");
        if (debug) Serial.println(gEStateDelay[led]);

        set(led,blinkOn[gEStatePwrup[led]].intensity);
        
        gEStatePwrup[led] += 1;
        if (gEStatePwrup[led]>=maxStateBlink) { 
          gEStatePwrup[led] = 0;
          gEStateRunning[led] = estate_ON;
          if (debug>1) Serial.println("state_ON");
        }
  }
  if (debug>1) Serial.print(".");
  gEStateDelay[led] = gEStateDelay[led] - 1;
}

// ---------------------------------------------------------------------
// fsmEclairage() est l'automate pour gérer un éclairage selon son état
//  OFF     : éteint
//  STPWRUP : début d'allumage si pas déjà allumé
//  PWRUP   : séquence d'allumage en cours, pour les néons
//  ON      : allumé, avec glitch aléatoire pour les vieux néons
//
// L'automate déroule OFF -> STPWRUP -> PWRUP -> ON
// 
// Eventuellement, de l'état ON on peut repasser en état PWRUP pour 
// simuler un glitch sur un vieux néon.
//
// A noter aussi que les éclairages standards passent directement de 
// l'état STPWRUP à l'état ON puisqu'il n'y a pas de séquence d'allumage
//
// De même, un éclairage déjà allumé passe directement de STPWRUP à ON
// car nul besoin de relancer une séquence d'allumage sur un néon déjà
// allumé ...
// ---------------------------------------------------------------------

void fsmEclairage() 
{
  for (int led=0; led<maxLights; led++) {

    switch (gEStateRunning[led]) {
      
      case estate_OFF : lightOff(led); break;

      case estate_STPWRUP : lightStartPowerUp(led); break;
    
      case estate_PWRUP : lightPowerUp(led); break;
    
      case estate_ON : lightOn(led); break;
    
    } // fin du switch
    
  } // pour chaque sortie

  // base de temps de nos FSM : la milliseconde (précision sommaire)
  delay(1);
}

// ---------------------------------------------------------------------
// PowerUpLeds
// Met en séquence d'allumage les leds qui ont besoin de l'être
// En utilisant le tableau de bits correspondant au mapping 
// ---------------------------------------------------------------------

void PowerUpLeds(long int leds)
{
  int pos = 1;
          
  for (int led=0; led<maxLights; led++) {
    if (leds&pos) lightStartPowerUp(led);
    pos = pos << 1;
  }
}

// ---------------------------------------------------------------------
// PowerDownLeds
// Met en off les leds qui ont besoin de l'être
// ---------------------------------------------------------------------

void PowerDownLeds(long int leds)
{
  int pos = 1;
          
  for (int led=0; led<maxLights; led++) {
    if (leds&pos) lightOff(led);
    pos = pos << 1;
  }
}

// ---------------------------------------------------------------------
// running Mode
// ---------------------------------------------------------------------

void runningFSM()
{
  long int  leds;
  long int  duration;
  int commande;
  long int  r;

  long int  ledsoff;

  // vérifie si le compteur de rappel est écoulé
  if (gSeq.duration<millis()) {
    // il est écoulé
    Serial.print("NEXT ");
    
    // préserve les leds qu'il faudra peut être éteindre
    ledsoff = gSeq.leds;

     gSeq.leds = 0;
    
    /* lit la séquence suivante */
    leds = *gpSeq++;
    duration = *gpSeq++;
    commande = *gpSeq++;     
    
    /* decode the instruction */
    switch (commande) {

      case END:
          Serial.println("END -> STOP");
          gSeqState = STOP;

          PowerDownLeds(ledsoff&~leds);
          break;

      case MARK: /* place la mark pour un futur LOOP */
          Serial.println("MARK");
          gpMarkSeq = gpSeq-3;
        
          PowerDownLeds(ledsoff&~leds);
          break;

      case LOOP: /* retourne sur la marque */
          Serial.println("LOOP");
          gpSeq = gpMarkSeq;

          PowerDownLeds(ledsoff&~leds);
          break;        
      
      case STANDBY: /* tirage aléatoire en minutes (1 à MAX_STANDBY minutes) */
          r = random(1,duration)*60;
          gSeq.duration = millis() + r*1000;
          
          Serial.print("STANDBY duration:");
          Serial.print(r);
          Serial.print(" s cmd:");
          printCmd(leds);

          // prévoir les extinction en même temps que le standby en cours
          gSeq.leds = leds | ledsoff;
          
          PowerUpLeds(leds);
         break; 

      case PERM: 
          /* permanent : jusqu'à l'arrêt de l'Arduino ou le changement de séquence ou UNSET */
          Serial.print("PERM cmd:");
          printCmd(leds);
                    
          PowerUpLeds(leds);   
          PowerDownLeds(ledsoff&~leds);
          break;

      case ALEA:
          /* aléatoirement : permet de rendre aléatoire la présence d'une personne dans un bureau la nuit, aligné sur la commande STANDBY suivante ou PERM précédente */
          if (random(0,duration)==0) {
            Serial.print("ALEA cmd:");
            printCmd(leds);
            
            PowerUpLeds(leds);
            gSeq.leds = leds;
          } else {
            Serial.print("ALEA NOPE cmd: ");
            printCmd(leds);

         }
          
          // pour standby à suivre, prévoir la future extinction en même temps que le standby en cours
          gSeq.leds |= ledsoff;

          break;

      case UNSET:
          gSeq.duration = millis() + duration*1000;
 
          Serial.print("UNSET ");
          Serial.print(duration);
          Serial.print("s cmd: ");
          printCmd(leds);
         
          PowerDownLeds(leds | ledsoff);
          break;
            
      case SET:
          gSeq.duration = millis() + duration*1000;
          gSeq.leds = leds;

          Serial.print("SET ");
          Serial.print(duration);
          Serial.print("s cmd: ");
          printCmd(leds);
          
          PowerUpLeds(leds);
          PowerDownLeds(ledsoff&~leds);
          break;

      case WAIT:
          gSeq.duration = millis() + duration*1000;
          Serial.print("WAIT ");
          Serial.println(duration);
          
          // prévoir les extinction en même temps que le wait en cours
          gSeq.leds = ledsoff;
          break;
 
      case WSTOP:
          gSeq.duration = millis() + duration*1000;
          r = (digitalRead(startPin) == HIGH);
          
          Serial.print("WSTOP ");
          Serial.print(duration);
          Serial.print(" pin: ");
          Serial.println(r);

          // prévoir les extinction en même temps que le stop en cours
          gSeq.leds = ledsoff;
          
          if (r) {
            gpSeq -=3; /* reste sur la commande en cours */
          }
          break;
          
      default:
        break;
    }
  }
}

// ---------------------------------------------------------------------
// loop()
//
// Implémente la FSM de gestion du batiment
// ---------------------------------------------------------------------

void loop() {

  fsmEclairage();

  switch (gSeqState) {
    case STOP : /* la machine est stoppée et ne peut redémarrer que sur une nouvelle sollicitation */
      {
        bool rst = (digitalRead(startPin) == HIGH);
      
        if (rst!=gCurrentStateStartPin) {
          if (rst==false) gCurrentStateStartPin=false;
          if (rst==true) {
            Serial.print("SW RESET -> INIT ");
            initFSM();
          }
        }
      }
      return;
      
    case START: /* Vérifie que c'est un départ sous controle de l'entrée D14 ! */
      if ((digitalRead(startPin)==HIGH) && (gCurrentStateStartPin==false)) {
        Serial.println("START");
        // fsmEclairage(); fait en entrée de loop() la première fois

        // on peut mettre la FSM dans le mode RUNNING
        gSeqState=RUNNING;

        // et indiquer que la séquence de démarrage a bien été détectée
        gCurrentStateStartPin=true;
      }
      break;

    case RUNNING:
      runningFSM();
      break;

     default: 
      break;
  }
       
}
