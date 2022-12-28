/*
Fichier DateHeure.cpp
Auteur: Stéphane Verdy
Date: 2022-12-19
Version: 0.1
*/

#include "DateHeure.h"
#include "Utils.h"

//Ecrit le constructeur de la class DateHeure
DateHeure::DateHeure()
{
  myWire = new ThreeWire(MY_RTC_DAT, MY_RTC_CLK, MY_RTC_RST);
  rtc = new RtcDS1302<ThreeWire>(*myWire);
}

void DateHeure::begin() {
  Serial.println("RTC is starting");
  rtc->Begin();

  if (!rtc->IsDateTimeValid()) 
  {
      // Common Causes:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing
      Serial.println("RTC lost confidence in the DateTime!");
      RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
      Serial.print("On set la date a : ");
      serialPrintADateHeure(compiled);
      Serial.println();
      rtc->SetDateTime(compiled);
      Serial.println("Set date time is done");
      Serial.println("New date time");
      serialPrintADateHeure(rtc->GetDateTime());
      Serial.println();        

  }
  rtc->SetIsRunning(true);
  Serial.println("RTC is running");
}

//Ecrit la méthode serialPrintDateHeure de la class DateHeure
void DateHeure::serialPrintDateHeure() {
  RtcDateTime dt = rtc->GetDateTime();
  char datestring[20];

  snprintf_P(datestring, 
          countof(datestring),
          PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
          dt.Month(),
          dt.Day(),
          dt.Year(),
          dt.Hour(),
          dt.Minute(),
          dt.Second() );

  Serial.print(datestring);
}

static void DateHeure::serialPrintADateHeure(const RtcDateTime& dt) {
  char datestring[20];

  snprintf_P(datestring, 
          countof(datestring),
          PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
          dt.Month(),
          dt.Day(),
          dt.Year(),
          dt.Hour(),
          dt.Minute(),
          dt.Second() );

  Serial.print(datestring);
}

//Ecrit la méthode getStringDateHeure de la class DateHeure
String DateHeure::getStringDateHeure() {
  RtcDateTime dt = rtc->GetDateTime();
  char datestring[20];

  snprintf_P(datestring, 
          countof(datestring),
          PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
          dt.Month(),
          dt.Day(),
          dt.Year(),
          dt.Hour(),
          dt.Minute(),
          dt.Second() );

  return String(datestring);
}

// Methode isJour returne true si l'heure est comprise entre HEURE_DEBUT_JOURNEE et HEURE_FIN_JOURNEE
bool DateHeure::isJour() {
  RtcDateTime dt = rtc->GetDateTime();
  char datestring[20];

  snprintf_P(datestring, 
          countof(datestring),
          PSTR("%02u:%02u:%02u"),
          dt.Hour(),
          dt.Minute(),
          dt.Second() );

  String heure = String(datestring);
  return (heure >= HEURE_DEBUT_JOURNEE && heure <= HEURE_FIN_JOURNEE);
}