// Lumieres
// Exemple pour le forum 3rails / Julie Dumortier / Licence GPL
// 
// La gestion d'un batiment, d'une scène, d'un ensemble d'éclairages avec un petit Arduino Nano
//
// Historique des versions (résumé)
//  v20211027 - première implémentation, mise au point, mode debug
//  v20211028 - mise au point, ajout de commandes (WAIT, WSTOP, UNSET), nettoyage du code, mode debug / verbose
//  v20211029 - utilisation de la BUILTIN led pour indiquer le bon fonctionnement du séquenceur / automate, nettoyage des traces verbose
//  v20211029.2 - Fixe un problème sur la commande ALEA suite à des tests intensifs
//  v20211029.3 - La germe du générateur n'était pas vraiment aléatoire ... (pratique pour les tests, moins pour l'authenticité)
//  v20211030 - ajout du mode gyrophare + reprise de la FSM eclairage (plus grande généricité)
//  v20211030.2 - Changement du nom de projet -> Lumieres
//  v20211030.3 - Ajout des 4 entrées utilisateurs et enrichissement de la commande WSTOP en conséquence + type Flash + type Soudure
//  v20211030.4 - Ajout du type Fire (brasero/bougie) + optimisation de la mémoire dynamique (chasse aux 'long int' inutiles) + type Servo
//
// Attention
//  brancher des micro-leds de type 2,9 V sur GND et D2 à D11, protégée par une résistance svp ! 
//  utilisez le calculateur de résistance svp  https://www.digikey.fr/fr/resources/conversion-calculators/conversion-calculator-led-series-resistor
//  pas plus de 40 mA par sortie, pas plus de 200 mA au total, au risque de griller une sortie et/ou l'Arduino Nano
//
//  l'intensité globale est réglable sur les sorties D3, D5, D6, D9, D10 et D11 (cf PWM_FOR_LED dans ConfigNano.h)
//
//  le mode de fonctionnement (néon ancien, néon récent, lampe à incadescence standard, ...) est configurable pour chaque sortie (cf ledCnf[])
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

// Ces fichiers permettent de simuler un néon, un gyrophare, ...
#include "SimuNeon.h"
#include "SimuGyrophare.h"
#include "SimuSoudure.h"
#include "SimuFire.h"

// Ce fichier concerne la machine à état fini pour gérer le batiment, la scène, ...
#include "FSMLumieres.h"

// ---------------------------------------------------------------------

// mettre à 1 pour un debug dans la console série, 2 pour full debug
const int debug = 0;

// mettre à 1 pour rendre l'exécution de l'automate verbeux
const int verbose = 1;

// la configuration de votre automatisme
#include "ConfigLumieres.h"

// au besoin, structure pour piloter un servo moteur sur D9 ou D10
#ifdef Servo_h
Servo gServo;
#endif

// ---------------------------------------------------------------------
// --- initFSM()
// ---------------------------------------------------------------------

void initFSM()
{ 
  int seq;

  // Initialise l'automate de chaque sortie
  for (int i = 0 ; i < maxLights; i++) gLight[i].stateRunning = estate_OFF;

  // récupère la séquence pour le mode RUNNING
  seq = digitalRead(seqPin);
  Serial.print("Séquence : ");
  Serial.print(seq+1);
  
  if (debug) {
    Serial.println(" - Debug");
    (seq==LOW)?gpSeq=(int*)&seqDebug1:gpSeq=(int*)&seqDebug2;
  } else {
    Serial.println(" - Normal");
    (seq==LOW)?gpSeq=(int*)&mySeq1:gpSeq=(int*)&mySeq2;
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

  // 4 entrées utilisateurs
  pinMode(inputUserPin1,INPUT_PULLUP);
  pinMode(inputUserPin2,INPUT_PULLUP);
  pinMode(inputUserPin3,INPUT_PULLUP);
  pinMode(inputUserPin4,INPUT_PULLUP);

  // la germe du générateur aléatoire
  randomSeed(analogRead(seedPin));

  // Annonce la version
  Serial.println("Lumieres - version 20211030.4 - (c) Julie Dumortier - Licence GPL");

  // initialize la FSM
  Serial.print("HW RESET -> INIT seed:");
  Serial.println(random());
  initFSM();
 }

// ---------------------------------------------------------------------
// Fonctions de support pour afficher l'état des leds impactés par une
// commande et l'état courant des leds.
// ---------------------------------------------------------------------

int mapLeds = 0;
int prevMapLeds = 0;

void printCmd(int leds,bool timing=false)
{
  int pos = 1;

  if (timing) {
    long int t = millis();
    Serial.print("t");
    Serial.print((float)t/1000.0,2);
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
  if ((debug>1) && (gLight[led].stateRunning != estate_OFF)) Serial.println("estate_OFF");

  if (ledCnf[led]==ETYPE_SERVO) {
    #ifdef Servo_h
    gServo.write(0);
    #endif
  } else {
    // eteint la led
    unset(led);
  }
  
  // passe en mode démarrage (au cas où)
  gLight[led].stateRunning = estate_OFF;
}

// démarre l'allumage des lumières
void lightStartPowerUp(int led)
{
  if (debug) {
    Serial.print("state_STPWRUP led:");
    Serial.println(led);
  }

  // si la led est déjà allumée, ne rien faire
  if (gLight[led].stateRunning == estate_ON) return;

  switch (ledCnf[led]) {
    case ETYPE_STANDARD: 
        gLight[led].stateRunning = estate_ON;
        if (debug>1) Serial.println("STANDARD -> state_ON");
        return;

    case ETYPE_NEONNEUF:
    case ETYPE_NEONVIEUX:
        gLight[led].pblink = (blink*)&blinkNeon;
        gLight[led].maxblink = sizeof(blinkNeon)/sizeof(blink);
        gLight[led].nextState = estate_ON;
        break;

    case ETYPE_GYROPHARE:
        gLight[led].pblink = (blink*)&blinkGyrophare;
        gLight[led].maxblink = sizeof(blinkGyrophare)/sizeof(blink);
        gLight[led].nextState = estate_PWRUP;
        break;

    case ETYPE_FLASH:
        gLight[led].pblink = (blink*)&blinkFlash;
        gLight[led].maxblink = sizeof(blinkFlash)/sizeof(blink);
        gLight[led].nextState = estate_OFF;
        break;

    case ETYPE_SOUDURE:
        gLight[led].pblink = (blink*)&blinkSoudure;
        gLight[led].maxblink = construitTableauSoudure();
        gLight[led].nextState = estate_STPWRUP;
        break;
       
    case ETYPE_FIRE:
        gLight[led].pblink = (blink*)&blinkFire;
        gLight[led].maxblink = construitTableauFire();
        gLight[led].nextState = estate_STPWRUP;
        break;

    #ifdef Servo_h
    case ETYPE_SERVO:
        gServo.attach(2+led);
        break;
    #endif
    
    case ETYPE_NOTUSED:
    default:
        gLight[led].stateRunning = estate_OFF;
        if (debug) Serial.println("NOTUSED or UNKNOWN -> state_OFF");
        return;    
  }

  // passe en mode démarrage
  gLight[led].stateRunning = estate_PWRUP;
  if (debug>1) Serial.print("stateRunning(");
  if (debug>1) Serial.print(led);
  if (debug>1) Serial.println(")<-- PWRUP");

  // prépare la séquence
  gLight[led].statePwrup = 0;
  gLight[led].stateDelay = 0;
}

// allume les lumières
void lightOn(int led)
{
    int alea;

    if (ledCnf[led]!=ETYPE_SERVO) {
      set(led,PWM_FOR_LED); 
    }
    
    if (ledCnf[led]==ETYPE_NEONVIEUX) {
      // fait un tirage aléatoire et refait un glitch à partir d'une séquence prédéterminée
      // ajuster la valeur random en fonction de la fréquence d'apparition (en ms)
      alea = random(0,10000);
      if (alea < 1) {
        if (verbose) {
          Serial.print("Glitch led:");
          Serial.println(led);
        }
        gLight[led].stateRunning = estate_PWRUP;
        gLight[led].statePwrup = INDEX_GLITCHPWRUP;
        gLight[led].stateDelay = 0;
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
      Serial.print(gLight[led].stateDelay);
    }

    #ifdef Servo_h
    if (ledCnf[led]==ETYPE_SERVO) {
      gServo.write(90);
      gLight[led].stateRunning = estate_ON;
      return;
    }
    #endif
    
    // vérifier si le delai de la transition est écoulé
    if (gLight[led].stateDelay<=0) 
    { 
        if (debug>1) {
          Serial.print(") - Blink sequence:");
          Serial.print(gLight[led].statePwrup);
          Serial.print(" intensity: ");
        }
        
        if (debug>1) Serial.print(gLight[led].pblink[gLight[led].statePwrup].intensity);

        gLight[led].stateDelay = gLight[led].pblink[gLight[led].statePwrup].duration;
        if (debug>1) Serial.print(" duration: ");
        if (debug) Serial.println(gLight[led].stateDelay);

        set(led,gLight[led].pblink[gLight[led].statePwrup].intensity);
        
        gLight[led].statePwrup += 1;
        if (gLight[led].statePwrup>=gLight[led].maxblink) { 
          gLight[led].statePwrup = 0;
          gLight[led].stateRunning = gLight[led].nextState;
        }
  }
  if (debug>1) Serial.print(".");
  gLight[led].stateDelay--;
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

    switch (gLight[led].stateRunning) {
      
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

void PowerUpLeds(int leds)
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

void PowerDownLeds(int leds)
{
  int pos = 1;
          
  for (int led=0; led<maxLights; led++) {
    if (leds&pos) lightOff(led);
    pos = pos << 1;
  }
}

// ---------------------------------------------------------------------
// decodeInputPin()
// decode l'entrée à utiliser en fonction du numéro inscrit en parametre
// de la commande
// ---------------------------------------------------------------------

int decodeInputPin(int io)
{
  int pin;

  // applique un filtre pour extraire le numéro de l'entrée
  switch (io&0x7F) {
    case 0 : pin = startPin; break;
    case 1 : pin = inputUserPin1; break;
    case 2 : pin = inputUserPin2; break;
    case 3 : pin = inputUserPin3; break;
    case 4 : pin = inputUserPin4; break;
    default: pin = startPin; break;
  }

  return pin;
}

// ---------------------------------------------------------------------
// running Mode
// ---------------------------------------------------------------------

void runningFSM()
{
  int  io;
  int  duration;
  int  commande;
  int  r;

  int  ledsoff;

  int       pin;

  // vérifie si le compteur de rappel est écoulé
  if (gSeq.duration<millis()) {
    // il est écoulé
    Serial.print("NEXT ");
    
    // préserve les leds qu'il faudra peut être éteindre
    ledsoff = gSeq.leds;

     gSeq.leds = 0;
    
    /* lit la séquence suivante */
    io = *gpSeq++;
    duration = *gpSeq++;
    commande = *gpSeq++;     
    
    /* decode the instruction */
    switch (commande) {

      case END:
          Serial.println("END -> STOP");
          gSeqState = STOP;

          PowerDownLeds(ledsoff&~io);
          break;

      case MARK: /* place la mark pour un futur LOOP */
          Serial.println("MARK");
          gpMarkSeq = gpSeq-3;
        
          PowerDownLeds(ledsoff&~io);
          break;

      case LOOP: /* retourne sur la marque */
          Serial.println("LOOP");
          gpSeq = gpMarkSeq;

          PowerDownLeds(ledsoff&~io);
          break;        
      
      case STANDBY: /* tirage aléatoire en minutes (1 à MAX_STANDBY minutes) */
          r = random(1,duration)*60;
          gSeq.duration = millis() + r*1000;
          
          Serial.print("STANDBY duration:");
          Serial.print(r);
          Serial.print(" s cmd:");
          printCmd(io);

          // prévoir les extinction en même temps que le standby en cours
          gSeq.leds = io | ledsoff;
          
          PowerUpLeds(io);
         break; 

      case PERM: 
          /* permanent : jusqu'à l'arrêt de l'Arduino ou le changement de séquence ou UNSET */
          Serial.print("PERM cmd:");
          printCmd(io);
                    
          PowerUpLeds(io);   
          PowerDownLeds(ledsoff&~io);
          break;

      case ALEA:
          /* aléatoirement : permet de rendre aléatoire la présence d'une personne dans un bureau la nuit, aligné sur la commande STANDBY suivante ou PERM précédente */
          if (random(0,duration)==0) {
            Serial.print("ALEA cmd:");
            printCmd(io);
            
            PowerUpLeds(io);
            gSeq.leds = io;
          } else {
            Serial.print("ALEA NOPE cmd: ");
            printCmd(io);

         }
          
          // pour standby à suivre, prévoir la future extinction en même temps que le standby en cours
          gSeq.leds |= ledsoff;

          break;

      case UNSET:
          gSeq.duration = millis() + duration*1000;
 
          Serial.print("UNSET ");
          Serial.print(duration);
          Serial.print("s cmd: ");
          printCmd(io);
         
          PowerDownLeds(io | ledsoff);
          break;
            
      case SET:
          if (duration<=0) {
            gSeq.duration = millis() + 500; /* demi seconde */
          } else {
            gSeq.duration = millis() + duration*1000;
          }
          gSeq.leds = io;

          Serial.print("SET ");
          Serial.print(duration);
          Serial.print("s cmd: ");
          printCmd(io);
          
          PowerUpLeds(io);
          PowerDownLeds(ledsoff&~io);
          break;

      case WAIT:
           if (duration<=0) {
            gSeq.duration = millis() + 500; /* demi seconde */
          } else {
            gSeq.duration = millis() + duration*1000;
          }
          Serial.print("WAIT ");
          Serial.println(duration);
          
          // prévoir les extinction en même temps que le wait en cours
          gSeq.leds = ledsoff;
          break;
 
      case WSTOP:
          if (duration<=0) {
            gSeq.duration = millis() + 500; /* demi seconde */
          } else {
            gSeq.duration = millis() + duration*1000;
          }

          // récupère le numéro de l'entrée mais aussi l'état bas/haut attendu
          pin = decodeInputPin(io);

          // test la condition d'arret du STOP
          r = (digitalRead(pin) == ((io&80)?LOW:HIGH));

          // Un peu de blabla, c'est utile en mise au point de l'automatisme
          Serial.print("WSTOP ");
          Serial.print(duration);
          Serial.print(" pin: ");
          Serial.println(pin);
          Serial.print(" val: ");
          Serial.println(r);

          // prévoir les extinction en même temps que le stop en cours
          gSeq.leds = ledsoff;

          // il faut encore attendre
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
