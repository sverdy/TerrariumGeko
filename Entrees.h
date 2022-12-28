/*
Fichier Entrees.h
Auteur: Stéphane Verdy
Date: 2022-12-21
Version: 0.1
*/
#ifndef ENTREES_H
#define ENTREES_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#include "Utils.h"

#define DHT22_PIN               26 // TODO: à changer
#define DS18B20_PIN             24 // TODO: à changer
#define TEMPERATURE_PRECISION   12

// Class Entrees qui gère les capteurs de température et d'humidité
class Entrees
{
    public:
        Entrees();
        void begin();
        void requestTemperatures();
        float getTemperatureGenerale();
        float getTemperatureSolPointChaud();
        float getTemperatureSolPointMedian();
        float getTemperatureSolPointFroid();
        float getHumiditeGenerale();
        void printAllData();

    private:
        OneWire *pOneWire_;
        DallasTemperature *pSensors_;
        DHT *pDHT22_;
        DeviceAddress pointChaud_, pointMedian_, pointFroid_;
        void printAddress(DeviceAddress deviceAddress);
        void printTemperature(DeviceAddress deviceAddress);
        void printResolution(DeviceAddress deviceAddress);
        void printData(DeviceAddress deviceAddress);
        void printDataByIndex(uint8_t index);

};

#endif