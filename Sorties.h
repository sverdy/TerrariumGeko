/*
Fichier Sortie.h
Auteur: Stéphane Verdy
Date: 2022-12-20
Version: 0.1
*/
#ifndef SORTIE_H
#define SORTIE_H

#include <Arduino.h>

#include "Modes.h"

#define RELAIS_PIN_1 32
#define RELAIS_PIN_2 33
#define RELAIS_PIN_3 34
#define RELAIS_PIN_4 35

#define RELAIS_PIN_LED      RELAIS_PIN_1
#define RELAIS_PIN_CHAUFFE  RELAIS_PIN_2
#define RELAIS_PIN_UV       RELAIS_PIN_3

#define RELAIS_OFF  1
#define RELAIS_ON   0

// définission de la class Sortie
class Sorties
{
    public:
        Sorties();
        void begin();

        void eteindreTout();
        void allumerTout();

        void allumerLED();
        void eteindreLED();
        bool isAllumerLED();
        void allumerChauffe();
        void eteindreChauffe();
        bool isAllumerChauffe();
        void allumerUV();
        void eteindreUV();
        bool isAllumerUV();

    private:
        bool allumerLED_;
        bool allumerChauffe_;
        bool allumerUV_;
};

#endif