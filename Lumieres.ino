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
//  v20211030.5 - Ajout du type Clignotant, éclairage qui ne clignote que s'il n'est pas permanent !
//  v20211030.6 - Ajout de la commande PWM pour envoyer un signal sur les sorties compatibles
//  v20211031 - optimisation des variables globales + macro langage pour faciliter l'écriture des automatismes (lisibilité)
//  v20211101 - Ajout des commandes ATTACH/DETACH qui permet de lier le cyle d'une ou plusieurs sorties avec l'état d'entrée
//  v20211103 - Amélioration du systeme de trace, mise au points et tests de l'automate + type de sortie Buzzer (liée au poste soudage)
//  v20211107 - Better traces + some buzzer testing + SERVO usage impact D9 (S8) & D10 (S9) pins not working with PWM
//  v20230913 - Ajout d'un filtre anti-rebond sur les entrées (non publié)
//  v2023100x - Publication sur un Pull Request Git Julaye-filter (isoler le développement) + Corrige des typos + Maj commentaires + Traces
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
//#include <MemoryUsage.h>

// l'intensité maximum de chaque sortie PWM pour vos LEDs
// à adapter en fonction de votre situation, entre 16 et 255 !
// Ne pas oublier la résistance de 220 ohms pour les protéger
const int PWM_FOR_LED = 64;

// l'intensité maximum de la sortie pour un buzzer
const byte PWM_FOR_BUZZER = 255;

// Ce fichier concerne la configuration matérielle Arduino utilisée
#include "ConfigNano.h"

// Ce fichier concerne la machine à état fini pour gérer un éclairage
#include "FSMeclairage.h"

// Ces fichiers permettent de simuler un néon, un gyrophare, ...
#include "SimuNeon.h"
#include "SimuGyrophare.h"
#include "SimuSoudure.h"
#include "SimuFire.h"

// le macro-langage pour simplifier la vie de l'utilisateur de l'automate
#include "MacroLumieres.h"

// Ce fichier concerne la machine à état fini pour gérer le batiment, la scène, ...
#include "FSMLumieres.h"

// ---------------------------------------------------------------------
//
// Système de mise au point
//
//  mettre en commentaire pour neutraliser la trace correspondante
//

// Information de base
#define DBG_ENABLE_INFO

// Information de mise au point de vos automatismes
#define DBG_ENABLE_VERBOSE

// Information de mise au point de l'automate
//#define DBG_ENABLE_DEBUG

// informations d'erreurs
#define DBG_ENABLE_ERROR

// Utilisation d'automatismes de tests (cf FSMLumieres.h)
#define DBG_ENABLE_TESTS

// ---------------------------------------------------------------------
// Configuration des deux automatismes (mySeq1 et mySeq2)

#ifdef DBG_ENABLE_TESTS
#include "ConfigTests.h"
#else
// la configuration de votre automatisme se trouve dans ce fichier
#include "ConfigLumieres.h"
#endif

// ---------------------------------------------------------------------
// au besoin, structure pour piloter un servo moteur sur D9 ou D10

#ifdef Servo_h
Servo gServo;
#endif

// ---------------------------------------------------------------------
// --- initFSM()
// ---------------------------------------------------------------------

void initFSM()
{ 
  byte seq;

  #ifdef DBG_ENABLE_VERBOSE
    Serial.print("Init FSM ");
  #endif
  
  // Initialise l'automate de chaque sortie
  for (int i = 0 ; i < maxLights; i++) {
    gLight[i].stateRunning = estate_OFF;
    gLight[i].link = LightNotLinked;
  }

  // récupère la séquence pour le mode RUNNING
  seq = digitalRead(seqPin);

  #ifdef DBG_ENABLE_INFO
    Serial.print("Séquence : ");
    Serial.print(seq+1);
  #endif
  
  #ifdef DBG_ENABLE_DEBUG
    Serial.print(" - Debug");
  #else
    #ifdef DBG_ENABLE_INFO
      Serial.print(" - Normal");
    #endif
  #endif

  #ifdef DBG_ENABLE_TESTS
    Serial.println(" - ConfigTests.h");
  #else
    #ifdef DBG_ENABLE_INFO
      Serial.println(" - ConfigLumieres.h");
    #endif
  #endif

  (seq==LOW)?gpSeq=(int*)&mySeq1:gpSeq=(int*)&mySeq2;

  // mark par défaut sur le début de la séquence
  gpMarkSeq = gpSeq;

  // initialise les différents états
  gSeqState = START;
  gCurrentStateStartPin = false;

  gSeq.duration = millis();
  gSeq.leds = 0;
  gSeq.command = _SET;
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
  inputState[0] = digitalRead(startPin);
  inputCount[0] = maxFiltre;

  // 4 entrées utilisateurs
  pinMode(inputUserPin1,INPUT_PULLUP);
  inputState[1] = digitalRead(inputUserPin1);
  inputCount[1] = maxFiltre;
  
  pinMode(inputUserPin2,INPUT_PULLUP);
  inputState[2] = digitalRead(inputUserPin2);
  inputCount[2] = maxFiltre;
  
  pinMode(inputUserPin3,INPUT_PULLUP);
  inputState[3] = digitalRead(inputUserPin3);
  inputCount[3] = maxFiltre;

  pinMode(inputUserPin4,INPUT_PULLUP);
  inputState[4] = digitalRead(inputUserPin4);
  inputCount[4] = maxFiltre;

  // la germe du générateur aléatoire
  randomSeed(analogRead(seedPin));

  // Annonce la version
  Serial.println("Lumieres - version 20231007 - (c) Julie Dumortier - Licence GPL");

  // initialize la FSM
  #ifdef DBG_ENABLE_DEBUG
    Serial.print("HW RESET -> INIT seed:");
    Serial.println(random());
  #endif
  initFSM();
 }

// ---------------------------------------------------------------------
// Fonctions de support pour afficher l'état des leds impactés par une
// commande et l'état courant des leds.
// ---------------------------------------------------------------------

int mapLeds = 0;
int prevMapLeds = 0;

void printCmd(int leds,bool timing=false,char cmd=0x00)
{
  int pos = 1;

  if (timing) {
    long int t = millis();
    Serial.print("t");
    Serial.print((float)t/1000.0,2);
  }

  Serial.print(" [");
  Serial.print(cmd);
  for (int i=0; i<maxLights; i++) {
    if (leds&pos) Serial.print(" X"); else Serial.print(" _");
    pos = pos << 1 ;
  }
  Serial.println(" ] ");
}

// pour pouvoir tester les automates en l'absence des branchements :)
void displayLeds(bool timing,char cmd)
{
  // ne pas afficher si rien ne change !
  if (prevMapLeds==mapLeds) return;
  prevMapLeds = mapLeds;

  // sinon affiche
  printCmd(mapLeds,timing,cmd);
}

// ---------------------------------------------------------------------
// Fonctions de support pour allumer / éteindre une led
// ---------------------------------------------------------------------

void unset(byte led)
{
  //Serial.print("unset ");
  //Serial.println(led);
  
  switch (outputMode[led]) {
    case MODE_IO: digitalWrite(2+led,LOW); break;
    case MODE_PWM: analogWrite(2+led,0); break;
    default: break;
  }

  #ifdef DBG_ENABLE_VERBOSE
    int pos = (1 << led);
    mapLeds &= ~pos;
    displayLeds(true,'U');
  #endif
}

void set(byte led, int value)
{
  if (value==LIGHT_OFF) {
    // indicateur que la séquence est vivante !
    digitalWrite(LED_BUILTIN,HIGH);

    unset(led);
    return;
  }

  //Serial.print("set ");
  //Serial.println(led);

  // indicateur que la séquence est vivante !
  digitalWrite(LED_BUILTIN,LOW);
  
  switch (outputMode[led]) {
    case MODE_IO: digitalWrite(2+led,HIGH); break;
    case MODE_PWM: analogWrite(2+led,value); break;
//      if (ledCnf[led]==ETYPE_BUZZER) {
//        analogWrite(2+led,PWM_FOR_BUZZER);
//      } else {
//        analogWrite(2+led,value);
//      }
//      break;
      
    default: break;
  }

  #ifdef DBG_ENABLE_VERBOSE
    int pos = (1 << led);
    mapLeds |= pos;
    displayLeds(true,'S');
  #endif
}

// ---------------------------------------------------------------------
// decodeInputPin()
// decode l'entrée à utiliser en fonction du numéro inscrit en parametre
// de la commande
// ---------------------------------------------------------------------

byte decodeInputPin(int io)
{
  byte pin;

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
// updateInput(io)
//  met à jour l'état de l'entrée en gérant l'anti-rebond (filtre)
// ---------------------------------------------------------------------

#ifdef DBG_ENABLE_VERBOSE
int mapInputs = 0;
int prevInputs = 0;

void displayInputs()
{
  int pos = 1;

  mapInputs = 0;
  for (int i=0 ; i < maxInputPins; i++) {
    mapInputs += (inputState[i] << i);
  }

  if (mapInputs!=prevInputs) {
    prevInputs = mapInputs;
  
    Serial.print(" INPUTS[");
    for (int i=0 ; i < maxInputPins; i++) {
      if (mapInputs&pos) Serial.print(" X"); else Serial.print(" _");
      pos = pos << 1 ;
    }
    Serial.println(" ] ");
  } 
}
#endif

bool updateInput(int io)
{
  byte pin;
  bool r;

  // lecture de la valeur courante
  pin = decodeInputPin(io);
  r = digitalRead(pin);

  // si l'état précédent est différent -> déclenche le filtre
  if (inputState[io]!=r) {
    if (inputCount[io]=0) {
      // le filtre a fait son job
      inputState[io] = r;         // garde le nouvel état
      inputCount[io] = maxFiltre; // relance le filtre
    } else {
      // le filtre doit faire son job
      inputCount[io]--;
    }     
  } else {
    // l'entrée n'a pas changé d'état depuis la dernière mise à jour
    inputCount[io] = maxFiltre;  // relance le filtre
  }

  #ifdef DBG_ENABLE_VERBOSE
    displayInputs();
  #endif

  // retourne l'état filtré
  return inputState[io];
}

// ---------------------------------------------------------------------
// sortie liée à une entrée ou à une autre sortie
// ---------------------------------------------------------------------

bool linkOff(byte led) 
{
    byte io;
    bool r;
    
    // la led est-elle liée à une entrée ? */
    if (gLight[led].link != LightNotLinked) {

      // récupère le numéro de l'entrée mais aussi l'état bas/haut attendu
      io = gLight[led].link&0x7F;

      // test la condition d'allumage
      r = (inputState[io] == ((gLight[led].link&0x80)?LOW:HIGH));
      if (r) {
        #ifdef DBG_ENABLE_INFO
          Serial.print("LINK input E");
          Serial.print(io);
          Serial.println(" said --> state OFF");
        #endif
        
        gLight[led].stateRunning = estate_OFF;
        return true;
      }
    }
    
    return false;
}

bool linkOn(byte led) 
{
    byte io;
    bool r;
    
    // la led est-elle liée à une entrée ? */
    if (gLight[led].link != LightNotLinked) {

      // récupère le numéro de l'entrée mais aussi l'état bas/haut attendu
      io = gLight[led].link&0x7F;

      // test la condition d'allumage
      r = (inputState[io] == ((gLight[led].link&0x80)?LOW:HIGH));
      if (!r) {
        #ifdef DBG_ENABLE_INFO
          Serial.print("LINK input E");
          Serial.print(io);
          Serial.println(" said --> state STPWRUP");
        #endif
        
        gLight[led].stateRunning = estate_STPWRUP;
        return true;
      }
    }
    
    return false;
}

byte linkOut(byte typeled=ETYPE_BUZZER)
{
    for (int i=0; i<maxLights; i++) {
      if (ledCnf[i]==typeled) return i;
    }
    return (byte)-1;
}
 
// ---------------------------------------------------------------------
// Fonctions de support pour la FSM éclairage d'une led
// ---------------------------------------------------------------------

// eteint les lumières
void lightOff(byte led)
{
  // la led est-elle liée à une entrée ? */
  if (linkOn(led)) return;

  if (ledCnf[led]==ETYPE_SERVO) {
    #ifdef Servo_h    
    gServo.write(0);
    #endif
  } else {
    // eteint la led
    unset(led);
  }

  #ifdef DBG_ENABLE_DEBUG
  if (gLight[led].stateRunning!=estate_OFF) {
    Serial.print("lightOff(S");
    Serial.print(led+1);
    Serial.println(") stateRunning<--OFF");
  }
  #endif
    
  // passe en mode démarrage (au cas où)
  gLight[led].stateRunning = estate_OFF;
}

// démarre l'allumage des lumières
// param est à true si l'allumage va être permanent
void lightStartPowerUp(byte led,byte param=false)
{
    byte buz;
 
    #ifdef DBG_ENABLE_DEBUG
      Serial.print("lightStartPowerUp S");
      Serial.println(led+1);
    #endif

  // si la led est déjà allumée, ne rien faire
  if (gLight[led].stateRunning == estate_ON) return;

  switch (ledCnf[led]) {
    case ETYPE_STANDARD: 
        gLight[led].stateRunning = estate_ON;
        if (param==0) {
          gLight[led].param = PWM_FOR_LED;          
        } else {
          gLight[led].param = param;
        }
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

        // buzzer linked out ?
        buz = linkOut(ETYPE_BUZZER);
        if (buz!=(byte)-1) {
          gLight[buz].pblink = gLight[led].pblink;
          gLight[buz].maxblink = gLight[led].maxblink;
        }
        break;

    case ETYPE_BUZZER:
        gLight[led].nextState = estate_STPWRUP;
        break;
       
    case ETYPE_FIRE:
        gLight[led].pblink = (blink*)&blinkFire;
        gLight[led].maxblink = construitTableauFire();
        gLight[led].nextState = estate_PWRUP;
        break;

    case ETYPE_CLIGNOTANT:
        if (param) {
          gLight[led].stateRunning = estate_ON;
          return;
        }
        gLight[led].pblink = (blink*)&blinkClignotant;
        gLight[led].maxblink = sizeof(blinkClignotant)/sizeof(blink);
        gLight[led].nextState = estate_PWRUP;
        break;

    #ifdef Servo_h
    case ETYPE_SERVO:
        #ifdef DBG_ENABLE_DEBUG
          Serial.print("Attach Servo to S");
          Serial.println(led+1);
        #endif
        //gServo.attach(2+led);
        break;
    #endif
    
    case ETYPE_NOTUSED:
    default:
        gLight[led].stateRunning = estate_OFF;

        #ifdef DBG_ENABLE_ERROR
          Serial.print("S");
          Serial.print(led+1);
          Serial.print(" NOTUSED or UNKNOWN (ledCnf:");
          Serial.print(ledCnf[led]);
          Serial.println(") <-- state_OFF");
        #endif
        return;    
  }

  // passe en mode démarrage
  gLight[led].stateRunning = estate_PWRUP;
  
  // prépare la séquence
  gLight[led].statePwrup = 0;
  gLight[led].stateDelay = 0;
}

// allume les lumières
void lightOn(byte led)
{
    int alea;
  
    // la led est-elle liée à une entrée ? */
    if (linkOff(led)) return;

    switch (ledCnf[led]) {
      case ETYPE_STANDARD:
        set(led,gLight[led].param); 
        break;

      case ETYPE_CLIGNOTANT:
      case ETYPE_NEONNEUF:
      case ETYPE_NEONVIEUX:
        set(led,PWM_FOR_LED); 
        break;
        
      case ETYPE_SERVO:
      default:
        break;
    }
    
    if (ledCnf[led]==ETYPE_NEONVIEUX) {
      // fait un tirage aléatoire et refait un glitch à partir d'une séquence prédéterminée
      // ajuster la valeur random en fonction de la fréquence d'apparition (en ms)
      alea = random(0,10000);
      if (alea < 1) {
        #ifdef DBG_ENABLE_DEBUG
          Serial.print("Glitch S");
          Serial.println(led+1);
        #endif
        
        gLight[led].stateRunning = estate_PWRUP;
        gLight[led].statePwrup = INDEX_GLITCHPWRUP;
        gLight[led].stateDelay = 0;
      }
    }
}

// gère une séquence d'allumage
void lightPowerUp(byte led)
{ 
    // la led est-elle liée à une entrée ? */
    if (linkOff(led)) return;

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
        gLight[led].stateDelay = gLight[led].pblink[gLight[led].statePwrup].duration;

        set(led,gLight[led].pblink[gLight[led].statePwrup].intensity);
        
        gLight[led].statePwrup += 1;
        if (gLight[led].statePwrup>=gLight[led].maxblink) { 
          gLight[led].statePwrup = 0;
          gLight[led].stateRunning = gLight[led].nextState;

          #ifdef DBG_ENABLE_DEBUG
            Serial.print("lightPowerUp S");
            Serial.print(led+1);
            Serial.print(" stateRunning<--nextstate:");
            Serial.println(gLight[led].stateRunning);          
          #endif
        }
  }

  gLight[led].stateDelay--;
}

// ---------------------------------------------------------------------
// LINK / UNLINK
// ---------------------------------------------------------------------

// connecte une led avec une sortie
void lightLink(byte led, byte input)
{
  gLight[led].link = input;

  #ifdef DBG_ENABLE_VERBOSE
    Serial.print("Link S");
    Serial.print(led+1);
    Serial.print(" with input E");
    Serial.println(input);
  #endif
  
  lightStartPowerUp(led);
}

// déconnecte une led avec une sortie
void lightUnlink(byte led)
{
  gLight[led].link = LightNotLinked;

  #ifdef DBG_ENABLE_VERBOSE
    Serial.print("UnLink S");
    Serial.println(led+1);
  #endif
  
  lightOff(led);
}

// ---------------------------------------------------------------------
// updateInputs()
// ---------------------------------------------------------------------

void updateInputs()
{
  for (int io=0; io<maxInputPins; io++) updateInput(io);
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

      default: 
        #ifdef DBG_ENABLE_ERROR
          Serial.print("Unknown stateRunning S"); Serial.print(led+1); Serial.print(" stateRunning:"); Serial.println(gLight[led].stateRunning);
        #endif
        break;
        
    } // fin du switch
    
  } // pour chaque sortie

  // lecture des entrées
  updateInputs();

  // base de temps de nos FSM : la milliseconde (précision sommaire)
  delay(1);
}

// ---------------------------------------------------------------------
// PowerUpLeds
// Met en séquence d'allumage les leds qui ont besoin de l'être
// En utilisant le tableau de bits correspondant au mapping 
// ---------------------------------------------------------------------

void PowerUpLeds(int leds,byte param=false)
{
  int pos = 1;
          
  for (int led=0; led<maxLights; led++) {
    if (leds&pos) lightStartPowerUp(led,param);
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
// Attach/DetachLeds
// Met en lien les leds qui ont besoin de l'être
// ---------------------------------------------------------------------

void AttachLeds(int leds,byte input)
{
  int pos = 1;
          
  for (int led=0; led<maxLights; led++) {
    if (leds&pos) lightLink(led,input);
    pos = pos << 1;
  }
}

void DetachLeds(int leds)
{
  int pos = 1;
          
  for (int led=0; led<maxLights; led++) {
    if (leds&pos) lightUnlink(led);
    pos = pos << 1;
  }
}

// ---------------------------------------------------------------------
// running Mode
// ---------------------------------------------------------------------

void runningFSM()
{
  int       io;
  int       commande;

  long int  duration;
  long int  r;

  int       ledsoff;

  int       pin;

  // vérifie si le compteur de rappel est écoulé
  if (gSeq.duration<millis()) {
    // il est écoulé
    #ifdef DBG_ENABLE_INFO
      Serial.print("NEXT ");
    #endif
    
    // préserve les leds qu'il faudra peut être éteindre
    ledsoff = gSeq.leds;

    gSeq.leds = 0;
    
    /* lit la séquence suivante */
    io = *gpSeq++;
    duration = *gpSeq++;
    commande = *gpSeq++;     
    
    /* decode the instruction */
    switch (commande) {

      case _END:
          #ifdef DBG_ENABLE_INFO
            Serial.println("END -> STOP");
          #endif
          
          gSeqState = STOP;

          PowerDownLeds(ledsoff&~io);
          break;

      case _MARK: /* place la mark pour un futur LOOP */
          #ifdef DBG_ENABLE_INFO
            Serial.println("MARK");
          #endif
          
          gpMarkSeq = gpSeq-3;
        
          PowerDownLeds(ledsoff&~io);
          break;

      case _LOOP: /* retourne sur la marque */
          #ifdef DBG_ENABLE_INFO
            Serial.println("LOOP");
          #endif
          
          gpSeq = gpMarkSeq;

          PowerDownLeds(ledsoff&~io);
          break;        
      
      case _STANDBY: /* tirage aléatoire en minutes (1 à MAX_STANDBY minutes) */
          r = random(1,duration)*60;
          gSeq.duration = millis() + r*1000;
          
          #ifdef DBG_ENABLE_INFO
            Serial.print("STANDBY duration:");
            Serial.print(r);
            Serial.print(" s cmd:");
            printCmd(io);
          #endif
          
          // prévoir les extinction en même temps que le standby en cours
          gSeq.leds = io | ledsoff;
          
          PowerUpLeds(io);
         break; 

      case _PERM: 
          /* permanent : jusqu'à l'arrêt de l'Arduino ou le changement de séquence ou UNSET */
          #ifdef DBG_ENABLE_INFO
            Serial.print("PERM cmd:");
            printCmd(io);
          #endif
                  
          PowerUpLeds(io,true);   
          PowerDownLeds(ledsoff&~io);
          break;

      case _PWM:
          #ifdef DBG_ENABLE_INFO
            Serial.print("PWN duration:");
            Serial.print(duration);
            Serial.print(" s cmd:");
            printCmd(io);
          #endif
          
          /* envoie une commande PWM sur une sortie STANDARD supportant le PWM */
          PowerUpLeds(io,duration);
          
          // prévoir les extinction plus tard
          gSeq.leds = io | ledsoff;
          break;

      case _ALEA:
          /* aléatoirement : permet de rendre aléatoire la présence d'une personne dans un bureau la nuit, aligné sur la commande STANDBY suivante ou PERM précédente */
          if (random(0,duration)==0) {
            #ifdef DBG_ENABLE_INFO
              Serial.print("ALEA cmd:");
              printCmd(io);
            #endif
            
            PowerUpLeds(io);
            gSeq.leds = io;
          } else {
            #ifdef DBG_ENABLE_INFO
              Serial.print("ALEA NOPE cmd: ");
              printCmd(io);
            #endif
         }
          
          // pour standby à suivre, prévoir la future extinction en même temps que le standby en cours
          gSeq.leds |= ledsoff;

          break;

      case _UNSET:
          gSeq.duration = millis() + duration*1000;
 
          #ifdef DBG_ENABLE_INFO
            Serial.print("UNSET ");
            Serial.print(duration);
            Serial.print("s cmd: ");
            printCmd(io);
          #endif
          
          PowerDownLeds(io | ledsoff);
          break;
            
      case _SET:
          if (duration<=0) {
            gSeq.duration = millis() + 500; /* demi seconde */
          } else {
            gSeq.duration = millis() + duration*1000;
          }
          gSeq.leds = io;

          #ifdef DBG_ENABLE_INFO
            Serial.print("SET ");
            Serial.print(duration);
            Serial.print("s cmd: ");
            printCmd(io);
          #endif
          
          PowerUpLeds(io);
          PowerDownLeds(ledsoff&~io);
          break;

      case _ATTACH:
          #ifdef DBG_ENABLE_INFO
            Serial.print("ATTACH input E");
            Serial.print(duration);
            Serial.print(" cmds:");
            printCmd(io);
          #endif
          
          AttachLeds(io,duration);

          // prévoir la future extinction 
          gSeq.leds |= ledsoff;
          break;

      case _DETACH:
          #ifdef DBG_ENABLE_INFO
            Serial.print("DETACH cmds:");
            printCmd(io);
          #endif
          
          DetachLeds(io);

          // prévoir la future extinction 
          gSeq.leds |= ledsoff;
          break;

      case _WAIT:
          if (duration<=0) {
            gSeq.duration = millis() + 500; /* demi seconde */
          } else {
            gSeq.duration = millis() + duration*1000;
          }

          #ifdef DBG_ENABLE_INFO
            Serial.print("WAIT ");
            Serial.println(duration);
          #endif
          
          // prévoir les extinction en même temps que le wait en cours
          gSeq.leds = ledsoff;
          break;
 
      case _WSTOP:
          if (duration<=0) {
            gSeq.duration = millis() + 500; /* demi seconde */
          } else {
            gSeq.duration = millis() + duration*1000;
          }

          // test la condition d'arret du STOP
          r = (inputState[io&0x7F] == ((io&0x80)?LOW:HIGH));

          // Un peu de blabla, c'est utile en mise au point de l'automatisme
          #ifdef DBG_ENABLE_INFO
            Serial.print("WSTOP ");
            Serial.print(duration);
            Serial.print(" input E");
            Serial.print(io);
            Serial.print(" state: ");
            Serial.println(r);
          #endif

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
        bool rst = (inputState[0] == HIGH);
      
        if (rst!=gCurrentStateStartPin) {
          if (rst==false) gCurrentStateStartPin=false;
          if (rst==true) {
            #ifdef DBG_ENABLE_INFO
            Serial.print("SW RESET -> INIT ");
            #endif
            initFSM();
          }
        }
      }
      return;
      
    case START: /* Vérifie que c'est un départ sous controle de l'entrée D14 ! */
      if ((inputState[0]==HIGH) && (gCurrentStateStartPin==false)) {
        #ifdef DBG_ENABLE_INFO
          Serial.println("START");
        #endif
        // fsmEclairage(); fait en entrée de loop() la première fois

        // on peut mettre la FSM dans le mode RUNNING
        gSeqState=RUNNING;

        // et indiquer que la séquence de démarrage a bien été détectée
        gCurrentStateStartPin=true;

        #ifdef __MemoryUsage_h__
        SRamDisplay();
        #endif
      }
      break;

    case RUNNING:
      runningFSM();
      break;

     default: 
      break;
  }
       
}
