/*
Fichier Ecran.h
Auteur: Stéphane Verdy
Date: 2022-12-20
Version: 0.1
*/
#ifndef ECRAN_H
#define ECRAN_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

#include "Modes.h"
#include "Couleurs.h"
#include "Utils.h"
#include "DateHeure.h"

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP = 9, XM = A3, YP = A2, YM = 8; //ID=0x9341
const int TS_LEFT = 122, TS_RT = 908, TS_TOP = 69, TS_BOT = 941;

//Ecrit une class Ecran avec un contructeur et une méthode afficherDateHeure et un attribut tft de type MCUFRIEND_kbv
class Ecran {
  public:
    Ecran(const DateHeure& dh);
    void paint();
    void refresh();
    void sleep();
    void print(char *texte, int x, int y, int couleur);
    bool touch_getXY(int *x, int *y);
    void action(int x, int y, uint8_t* mode);
    void setTemperaturePointChaud(float temperaturePointChaud);
    void setTemperaturePointFroid(float temperaturePointFroid);
    void setTemperaturePointMedian(float temperaturePointMedian);
    void setTemperatureGenerale(float temperatureGenerale);
    void setHumiditeGenerale(float humiditeGenerale);
    void setTemperatureDemandeeJour(float temperatureDemandeeJour);
    void setTemperatureDemandeeNuit(float temperatureDemandeeNuit);
    void setIsChauffeOn(bool isChauffeOn);
    void setIsUVOn(bool isUVOn);

  private:
    MCUFRIEND_kbv tft;
    TouchScreen ts;
    DateHeure *pDateHeure;
    Adafruit_GFX_Button light_on_btn, light_off_btn, uv_on_btn, uv_off_btn, uv_auto_btn, heat_on_btn, heat_off_btn, heat_auto_btn;
    float temperaturePointChaud_, temperaturePointFroid_, temperaturePointMedian_, temperatureGenerale_, humiditeGenerale_;
    float temperatureDemandeeJour_, temperatureDemandeeNuit_; 
    bool  isChauffeOn_, isUVOn_;

};

#endif 