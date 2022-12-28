// initialisation of arduino main program
// 2022-12-19  S.Verdy  v0.1

#include <Arduino.h>
#include <Adafruit_GFX.h>

#include "Modes.h"
#include "Couleurs.h"
#include "DateHeure.h"
#include "Ecran.h"
#include "Sorties.h"
#include "Entrees.h"

#define TEMPERATURE_DEMANDER_POINT_CHAUD_JOUR  33.0
#define TEMPERATURE_DEMANDER_POINT_CHAUD_NUIT  22.0

// enumeration des mode d'écran
enum LesEcrans
{
    MODE_ECRAN_PRINCIPAL,
    MODE_ECRAN_GESTION_DATE_HEURE,
};

// global variables
DateHeure* pDateHeure;
Ecran* pEcran;
Sorties* pSorties;
Entrees* pEntrees;

const long refreshScreenInterval = 1000;  // interval at which to repaint the screen (milliseconds)
const long refreshTemperatureInterval = 2000;  // interval at which to refresh temperature (milliseconds)
const long miseEnveilleInterval = 60000 * 5;  // interval at which to switch in sleep mode (5 minutes)
unsigned long previousRefreshScreen = 0;  // will store last time screen was been repainted
unsigned long previousTouch = 0;  // will store last time screen was been touched
unsigned long previousRefreshTemperature = 0;  // will store last time temperature was been refreshed
int x, y;
bool isSleeping = false;
uint8_t modeActif = MODE_CHAUFFE_AUTO & MODE_UV_AUTO & MODE_LED_ARRET_FORCEE;
float temperaturePointChaud, temperaturePointFroid, temperaturePointMedian, temperatureGenerale, humiditeGenerale;

//Ecrit la fonction setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Démarrage du programme");
  pDateHeure = new DateHeure();
  pDateHeure->begin();
  pEcran = new Ecran(*pDateHeure);
  pEcran->setTemperatureDemandeeJour(TEMPERATURE_DEMANDER_POINT_CHAUD_JOUR);
  pEcran->setTemperatureDemandeeNuit(TEMPERATURE_DEMANDER_POINT_CHAUD_NUIT);
  pEcran->paint();
  pSorties = new Sorties();
  pSorties->begin();
  pEntrees = new Entrees();
  pEntrees->begin();
}

//ecrit la fonction loop
void loop() {
  // test de la fonction touch_getXY
  if (pEcran->touch_getXY(&x, &y)) {
    Serial.print("x = ");
    Serial.print(x);
    Serial.print(" y = ");
    Serial.println(y);
    if( !isSleeping ) { // si l'écran est en veille, on le reveille sans prendre en compte l'action
      pEcran->action(x, y, &modeActif);
    } else {
      pEcran->paint();
      isSleeping = false;
      Serial.println("ecran reveille");
    }
    previousTouch = millis();
  } else {
    if(!isSleeping) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousTouch >= miseEnveilleInterval) {
        pEcran->sleep();
        isSleeping = true;
        Serial.println("mise en veille");
      }
    }
  }

  // recuperation des capteurs de température et d'humidité
  if (millis() - previousRefreshTemperature >= refreshTemperatureInterval) {
    previousRefreshTemperature = millis();
    pEntrees->requestTemperatures();
    temperaturePointChaud = pEntrees->getTemperatureSolPointChaud();
    temperaturePointFroid = pEntrees->getTemperatureSolPointFroid();
    temperaturePointMedian = pEntrees->getTemperatureSolPointMedian();
    temperatureGenerale = pEntrees->getTemperatureGenerale();
    humiditeGenerale = pEntrees->getHumiditeGenerale();
  }

  // gestion de la lumiere UV
  if(modeActif & MODE_UV_MARCHE_FORCEE) {
    pSorties->allumerUV();
  } else if(modeActif & MODE_UV_ARRET_FORCEE) {
      pSorties->eteindreUV();
  } else { // mode Auto
    // gestion de la lampe UV en fonction de l'heure
    if( pDateHeure->isJour() ) {
      if( !pSorties->isAllumerUV() ) pSorties->allumerUV();
    } else {
      if( pSorties->isAllumerUV() ) pSorties->eteindreUV();
    }
  }

  // gestion de la source de chaleur
  if(modeActif & MODE_CHAUFFE_MARCHE_FORCEE) {
    pSorties->allumerChauffe();
  } else if(modeActif & MODE_CHAUFFE_ARRET_FORCEE) {
      pSorties->eteindreChauffe();
  } else { // mode Auto
    // gestion de la source de chaleur en fonction de la température
    if( pDateHeure->isJour() ) {
      if( temperaturePointChaud < (TEMPERATURE_DEMANDER_POINT_CHAUD_JOUR - 0.5) ) {
        if( !pSorties->isAllumerChauffe() ) pSorties->allumerChauffe();
      } else if( temperaturePointChaud > (TEMPERATURE_DEMANDER_POINT_CHAUD_JOUR + 0.5) ) {
        if( pSorties->isAllumerChauffe() ) pSorties->eteindreChauffe();
      }
    } else {
      if( temperaturePointChaud < (TEMPERATURE_DEMANDER_POINT_CHAUD_NUIT - 0.5) ) {
        if( !pSorties->isAllumerChauffe() ) pSorties->allumerChauffe();
      } else if( temperaturePointChaud > (TEMPERATURE_DEMANDER_POINT_CHAUD_NUIT + 0.5) ) {
        if( pSorties->isAllumerChauffe() ) pSorties->eteindreChauffe();
      }
    }
  }

  // gestion de l'eclairage LED
  if(modeActif & MODE_LED_ARRET_FORCEE) {
    pSorties->eteindreLED();
  } else {
    if(modeActif & MODE_LED_MARCHE_FORCEE) {
      pSorties->allumerLED();
    }
  }

  if (!isSleeping) {
    // gestion rafrachissement de l'écran
    unsigned long currentMillis = millis();
    if (currentMillis - previousRefreshScreen >= refreshScreenInterval) {
      previousRefreshScreen = currentMillis;
      pEcran->setIsChauffeOn(pSorties->isAllumerChauffe());
      pEcran->setIsUVOn(pSorties->isAllumerUV());
      pEcran->setTemperaturePointChaud(temperaturePointChaud);
      pEcran->setTemperaturePointFroid(temperaturePointFroid);
      pEcran->setTemperaturePointMedian(temperaturePointMedian);
      pEcran->setTemperatureGenerale(temperatureGenerale);
      pEcran->setHumiditeGenerale(humiditeGenerale);
      pEcran->refresh();
      pEntrees->printAllData();
    }
  }
  delay(200 /*ms*/);
}

