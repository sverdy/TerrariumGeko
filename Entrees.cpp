/*
Fichier Entrees.cpp
Auteur: Stéphane Verdy
Date: 2022-12-21
Version: 0.1
*/
#include "Entrees.h"

// Ecrit le constructeur de la class Entrees
Entrees::Entrees()
{
    pOneWire_ = new OneWire(DS18B20_PIN);
    pSensors_ = new DallasTemperature(pOneWire_);
    pDHT22_ = new DHT(DHT22_PIN, DHT22);
}

// Ecrit la méthode begin de la class Entrees
void Entrees::begin()
{
    pSensors_->begin();
    Serial.print("Locating DS18B20 devices...");
    Serial.print("Found ");
    Serial.print(pSensors_->getDeviceCount(), DEC);
    Serial.println(" devices.");
    Serial.print("Parasite power is: ");
    if (pSensors_->isParasitePowerMode())
    {
        Serial.println("ON");
    } else {
        Serial.println("OFF");
    }
    if (!pSensors_->getAddress(pointChaud_, 0))
    {
        Serial.println("Unable to find address for Device 0");
    }
    if (!pSensors_->getAddress(pointMedian_, 1))
    {
        Serial.println("Unable to find address for Device 1");
    }
    if (!pSensors_->getAddress(pointFroid_, 2))
    {
        Serial.println("Unable to find address for Device 2");
    }
    Serial.print("Device pointChaud Address: ");
    printAddress(pointChaud_);
    Serial.println();
    Serial.print("Device pointMedian Address: ");
    printAddress(pointMedian_);
    Serial.println();
    Serial.print("Device pointFroid Address: ");
    printAddress(pointFroid_);
    Serial.println();
    pSensors_->setResolution(pointChaud_, TEMPERATURE_PRECISION);
    pSensors_->setResolution(pointMedian_, TEMPERATURE_PRECISION);
    pSensors_->setResolution(pointFroid_, TEMPERATURE_PRECISION);

    Serial.print("Device pointChaud Resolution: ");
    Serial.print(pSensors_->getResolution(pointChaud_), DEC);
    Serial.println();
    Serial.print("Device pointMedian Resolution: ");
    Serial.print(pSensors_->getResolution(pointMedian_), DEC);
    Serial.println();
    Serial.print("Device pointFroid Resolution: ");
    Serial.print(pSensors_->getResolution(pointFroid_), DEC);
    Serial.println();

    // initialisation du DHT22
    pDHT22_->begin();
    Serial.println("DHT22 initialise");
    Serial.println("Temperature generale : " + String(getTemperatureGenerale()) + "°C");
    Serial.println("Humidite generale : " + String(getHumiditeGenerale()) + "%");
}

// Ecrit le méthode getTemperatureGenerale de la class Entrees
float Entrees::getTemperatureGenerale()
{
    return pDHT22_->readTemperature();
}

// Ecrit le méthode requestTemperatures de la class Entrees
void Entrees::requestTemperatures()
{
    pSensors_->requestTemperatures();
}

// Ecrit le méthode getTemperatureSolPointChaud de la class Entrees
float Entrees::getTemperatureSolPointChaud()
{
    return pSensors_->getTempC(pointMedian_);
}

// Ecrit le méthode getTemperatureSolPointMedian de la class Entrees
float Entrees::getTemperatureSolPointMedian()
{
    return pSensors_->getTempC(pointChaud_);
}

// Ecrit le méthode getTemperatureSolPointFroid de la class Entrees
float Entrees::getTemperatureSolPointFroid()
{
    return pSensors_->getTempC(pointFroid_);
}

// Ecrit le méthode getHumiditeGenerale de la class Entrees
float Entrees::getHumiditeGenerale()
{
    return pDHT22_->readHumidity();
}

void Entrees::printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
void Entrees::printTemperature(DeviceAddress deviceAddress) {
  float tempC = pSensors_->getTempC(deviceAddress);
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" C; ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.print(" F");
}
void Entrees::printResolution(DeviceAddress deviceAddress) {
  Serial.print("Resolution: ");
  Serial.println(pSensors_->getResolution(deviceAddress));
}
void Entrees::printData(DeviceAddress deviceAddress) {
  printTemperature(deviceAddress); Serial.print(" ; Device:");
  printAddress(deviceAddress); Serial.println();
}
void Entrees::printDataByIndex(uint8_t index) {
  Serial.print("Device:");
  Serial.print(index);
  Serial.print(" Temperature: ");
  Serial.println(pSensors_->getTempCByIndex(index));
}

void Entrees::printAllData() {
  Serial.println("Print all data");
  for(uint8_t i=0; i<pSensors_->getDeviceCount(); i++) {
    printDataByIndex(i);
  }
}
