/*
Fichier Sortiess.cpp
Auteur: Stéphane Verdy
Date: 2022-12-20
Version: 0.1
*/
#include "Sorties.h"

// Ecrit le constructeur de la class Sorties
Sorties::Sorties()
{
    allumerLED_ = false;
    allumerChauffe_ = false;
    allumerUV_ = false;
}

// Ecrit la méthode begin de la class Sorties
void Sorties::begin()
{
    pinMode(RELAIS_PIN_LED, OUTPUT);
    pinMode(RELAIS_PIN_CHAUFFE, OUTPUT);
    pinMode(RELAIS_PIN_UV, OUTPUT);
    eteindreTout();
}

// Ecrit la méthode eteindreTout de la class Sorties
void Sorties::eteindreTout()
{
    digitalWrite(RELAIS_PIN_LED, RELAIS_OFF);
    digitalWrite(RELAIS_PIN_CHAUFFE, RELAIS_OFF);
    digitalWrite(RELAIS_PIN_UV, RELAIS_OFF);
    allumerLED_ = false;
    allumerChauffe_ = false;
    allumerUV_ = false;
}

// Ecrit la méthode allumerTout de la class Sorties
void Sorties::allumerTout()
{
    digitalWrite(RELAIS_PIN_LED, RELAIS_ON);
    digitalWrite(RELAIS_PIN_CHAUFFE, RELAIS_ON);
    digitalWrite(RELAIS_PIN_UV, RELAIS_ON);
    allumerLED_ = true;
    allumerChauffe_ = true;
    allumerUV_ = true;
}

// Ecrit la méthode allumerLED de la class Sorties
void Sorties::allumerLED()
{
    digitalWrite(RELAIS_PIN_LED, RELAIS_ON);
    allumerLED_ = true;
}

// Ecrit la méthode eteindreLED de la class Sorties
void Sorties::eteindreLED()
{
    digitalWrite(RELAIS_PIN_LED, RELAIS_OFF);
    allumerLED_ = false;
}

// Ecrit la méthode isAllumerLED de la class Sorties
bool Sorties::isAllumerLED()
{
    return allumerLED_;
}

// Ecrit la méthode allumerChauffe de la class Sorties
void Sorties::allumerChauffe()
{
    digitalWrite(RELAIS_PIN_CHAUFFE, RELAIS_ON);
    allumerChauffe_ = true;
}

// Ecrit la méthode eteindreChauffe de la class Sorties
void Sorties::eteindreChauffe()
{
    digitalWrite(RELAIS_PIN_CHAUFFE, RELAIS_OFF);
    allumerChauffe_ = false;
}

// Ecrit la méthode isAllumerChauffe de la class Sorties
bool Sorties::isAllumerChauffe()
{
    return allumerChauffe_;
}

// Ecrit la méthode allumerUV de la class Sorties
void Sorties::allumerUV()
{
    digitalWrite(RELAIS_PIN_UV, RELAIS_ON);
    allumerUV_ = true;
}

// Ecrit la méthode eteindreUV de la class Sorties
void Sorties::eteindreUV()
{
    digitalWrite(RELAIS_PIN_UV, RELAIS_OFF);
    allumerUV_ = false;
}

// Ecrit la méthode isAllumerUV de la class Sorties
bool Sorties::isAllumerUV()
{
    return allumerUV_;
}

