/*
Fichier DateHeure.h
Auteur: Stéphane Verdy
Date: 2022-12-19
Version: 0.1
*/

#ifndef DATEHEURE_H
#define DATEHEURE_H

#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

// CONNECTIONS:
#define MY_RTC_CLK 53 // DS1302 CLK/SCLK
#define MY_RTC_DAT 52 // DS1302 DAT/IO
#define MY_RTC_RST 50 // DS1302 RST/CE
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#define HEURE_DEBUT_JOURNEE "08:00:00"
#define HEURE_FIN_JOURNEE   "20:00:00"
    
//Ecrit une class DateHeure avec un contructeur et une méthode serialPrintDateHeure et un attribut myWire de type ThreeWire
class DateHeure {
  public:
    DateHeure();
    void begin();
    void serialPrintDateHeure();
    static void serialPrintADateHeure(const RtcDateTime& dt);
    String getStringDateHeure();
    bool isJour();

  private:
    ThreeWire *myWire;
    RtcDS1302<ThreeWire> *rtc;
};

#endif  