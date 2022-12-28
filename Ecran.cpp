/*
Fichier Ecran.cpp
Auteur: Stéphane Verdy
Date: 2022-12-20
Version: 0.1
*/
#include "Ecran.h"

// Ecrit le constructeur de la class Ecran
Ecran::Ecran(const DateHeure &dh) : tft(), ts(XP, YP, XM, YM, 300), pDateHeure(&dh)
{
    tft.reset();
    uint16_t identifier = tft.readID();
    if (identifier == 0xD3D3)
        identifier = 0x9486; // write-only shield

    tft.begin(identifier);
    tft.setRotation(1); // paysage
    tft.setTextColor(BLANC);
    tft.setTextSize(2);

    // initialiste les états des boutons
    light_on_btn.press(false);
    light_off_btn.press(true);
    heat_on_btn.press(false);
    heat_off_btn.press(false);
    heat_auto_btn.press(true);
    uv_on_btn.press(false);
    uv_off_btn.press(false);
    uv_auto_btn.press(true);

    // initialisation des températures et taux d'hydrométrie
    temperaturePointChaud_ = 00.00;
    temperaturePointMedian_ = 00.00;
    temperaturePointFroid_ = 00.00;
    temperatureGenerale_ = 00.00;
    temperatureDemandeeJour_ = 00.00;
    temperatureDemandeeNuit_ = 00.00;
    humiditeGenerale_ = 00.00;

    isChauffeOn_ = false;
    isUVOn_ = false;
    
}

// Ecrit la méthode touch_getXY de la class Ecran
bool Ecran::touch_getXY(int *x, int *y)
{
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z < MINPRESSURE || p.z > MAXPRESSURE)
    {
        return false;
    }
    if (tft.width() <= tft.height())
    {                                                       // Portrait
        *x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        *y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    else
    {
        *x = map(p.y, TS_TOP, TS_BOT, 0, tft.width());
        *y = map(p.x, TS_RT, TS_LEFT, 0, tft.height());
    }
    return true;
}

// Ecrit la méthode paint de la class Ecran
void Ecran::paint()
{
    tft.fillScreen(NOIR);
    tft.setTextColor(BLANC, NOIR);
    // affiche la date et l'heure
    tft.setCursor(0, 0);
    tft.print(pDateHeure->getStringDateHeure());
    // affiche debut de journée
    tft.setCursor(0, 20);
    tft.print("Debut de journee: ");
    // Affiche que les heures et les minutes de HEURE_DEBUT_JOURNEE
    tft.print(String(HEURE_DEBUT_JOURNEE).substring(0, 5));
    // affiche fin de journée
    tft.setCursor(0, 40);
    tft.print("Fin de journee: ");
    tft.print(String(HEURE_FIN_JOURNEE).substring(0, 5));

    // zone d'affichage des températures
    tft.drawRect(300, 10, 165, 50, BLANC);
    tft.drawRect(10, 70, 145, 90, BLANC);
    tft.drawRect(165, 70, 145, 90, BLANC);
    tft.drawRect(320, 70, 145, 90, BLANC);

    // temperature et humidité générale
    tft.setCursor(310, 15);
    tft.print("Temp: " + String(temperatureGenerale_) + "C");
    tft.setCursor(310, 35);
    tft.print("Hum: " + String(humiditeGenerale_) + "%");

    if( isChauffeOn_ ) {
        tft.fillRect(15, 75, 5, 5, ROUGE);
    } else {
        tft.fillRect(15, 75, 5, 5, NOIR);
    }

    if( isUVOn_ ) {
        tft.fillRect(455, 75, 5, 5, JAUNE);
    } else {
        tft.fillRect(455, 75, 5, 5, NOIR);
    }

    tft.setCursor(50, 80);
    if( pDateHeure->isJour() ) {
        tft.print("[" + String(temperatureDemandeeJour_) + "C]");
    } else {
        tft.print("[" + String(temperatureDemandeeNuit_) + "C]");
    }
    tft.setCursor(20, 115);
    tft.print(String(temperaturePointChaud_) + "C");
    tft.setCursor(175, 115);
    tft.print(String(temperaturePointMedian_) + "C");
    tft.setCursor(330, 115);
    tft.print(String(temperaturePointFroid_) + "C");
    
    // affiche les commandes de la lampe UV
    tft.setCursor(0, 190);
    tft.print("Lampe UV:");
    uv_on_btn.initButton(&tft,  200, 195, 100, 40, BLANC, CYAN, NOIR, "ON", 2);
    uv_off_btn.initButton(&tft, 310, 195, 100, 40, BLANC, CYAN, NOIR, "OFF", 2);
    uv_auto_btn.initButton(&tft, 420, 195, 100, 40, BLANC, CYAN, NOIR, "AUTO", 2);
    uv_on_btn.drawButton(!uv_on_btn.isPressed());
    uv_off_btn.drawButton(!uv_off_btn.isPressed());
    uv_auto_btn.drawButton(!uv_auto_btn.isPressed());

    // affiche les commandes de la chauffe
    tft.setTextColor(BLANC, NOIR);
    tft.setCursor(0, 235);
    tft.print("Chauffage:");
    heat_on_btn.initButton(&tft,  200, 240, 100, 40, BLANC, CYAN, NOIR, "ON", 2);
    heat_off_btn.initButton(&tft, 310, 240, 100, 40, BLANC, CYAN, NOIR, "OFF", 2);
    heat_auto_btn.initButton(&tft, 420, 240, 100, 40, BLANC, CYAN, NOIR, "AUTO", 2);
    heat_on_btn.drawButton(!heat_on_btn.isPressed());
    heat_off_btn.drawButton(!heat_off_btn.isPressed());
    heat_auto_btn.drawButton(!heat_auto_btn.isPressed());

    // affiche les commande de la lumière
    tft.setTextColor(BLANC, NOIR);
    tft.setCursor(0, 280);
    tft.print("Eclairage:");
    light_on_btn.initButton(&tft,  200, 285, 100, 40, BLANC, CYAN, NOIR, "ON", 2);
    light_off_btn.initButton(&tft, 310, 285, 100, 40, BLANC, CYAN, NOIR, "OFF", 2);
    light_on_btn.drawButton(!light_on_btn.isPressed());
    light_off_btn.drawButton(!light_off_btn.isPressed());

}

// Ecrit la méthode refresh de la class Ecran
void Ecran::refresh()
{
    tft.setTextColor(BLANC, NOIR);
    tft.setCursor(0, 0);
    String d = pDateHeure->getStringDateHeure();
    tft.print(d);

    tft.setCursor(310, 15);
    tft.print("Temp: " + String(temperatureGenerale_) + "C");
    tft.setCursor(310, 35);
    tft.print("Hum: " + String(humiditeGenerale_) + "%");

    if( isChauffeOn_ ) {
        tft.fillRect(15, 75, 5, 5, ROUGE);
        Serial.println("ON isChauffeOn_ => " + String(isChauffeOn_));
    } else {
        tft.fillRect(15, 75, 5, 5, NOIR);
        Serial.println("OFF isChauffeOn_ => " + String(isChauffeOn_));
    }

    if( isUVOn_ ) {
        tft.fillRect(455, 75, 5, 5, JAUNE);
    } else {
        tft.fillRect(455, 75, 5, 5, NOIR);
    }

    tft.setCursor(50, 80);
    if( pDateHeure->isJour() ) {
        tft.print("[" + String(temperatureDemandeeJour_) + "C]");
    } else {
        tft.print("[" + String(temperatureDemandeeNuit_) + "C]");
    }
    tft.setCursor(20, 115);
    tft.print(String(temperaturePointChaud_) + "C");
    tft.setCursor(175, 115);
    tft.print(String(temperaturePointMedian_) + "C");
    tft.setCursor(330, 115);
    tft.print(String(temperaturePointFroid_) + "C");
}

// Ecrit la méthode print de la class Ecran
void Ecran::print(char *texte, int x, int y, int couleur)
{
    tft.setCursor(x, y);
    tft.setTextColor(couleur);
    tft.print(texte);
}

// Ecrit la méthode sleep de la class Ecran
void Ecran::sleep()
{
    tft.fillScreen(NOIR);
}

// Ecrit la méthode action de la class Ecran
void Ecran::action(int x, int y, uint8_t *mode)
{
    if (light_on_btn.contains(x, y)) // on a appuyé sur le bouton light on
    {
        light_on_btn.press(true);
        light_off_btn.press(false);    
        light_on_btn.drawButton(!light_on_btn.isPressed());
        light_off_btn.drawButton(!light_off_btn.isPressed());
   
        // mettre le 7 eme bit de mode à 1
        *mode = *mode & 63; // 0011 1111
        *mode = *mode | MODE_LED_MARCHE_FORCEE; // 0100 0000
    }
    else if (light_off_btn.contains(x, y)) // on a appuyé sur le bouton light off
    {
        light_on_btn.press(false);
        light_off_btn.press(true);
        light_on_btn.drawButton(!light_on_btn.isPressed());
        light_off_btn.drawButton(!light_off_btn.isPressed());

        // mettre le 8 eme bit de mode à 1
        *mode = *mode & 63; // 0011 1111
        *mode = *mode | MODE_LED_ARRET_FORCEE; // 1000 0000  
    }
    else if (heat_on_btn.contains(x, y)) // on a appuyé sur le bouton heat on
    {
        heat_on_btn.press(true);
        heat_off_btn.press(false);
        heat_auto_btn.press(false);
        heat_on_btn.drawButton(!heat_on_btn.isPressed());
        heat_off_btn.drawButton(!heat_off_btn.isPressed());
        heat_auto_btn.drawButton(!heat_auto_btn.isPressed());

        // mettre le 2 eme bit de mode à 1
        *mode = *mode & 248; // 1111 1000
        *mode = *mode | MODE_CHAUFFE_MARCHE_FORCEE; // 0000 0010
    }
    else if (heat_off_btn.contains(x, y)) // on a appuyé sur le bouton heat off
    {
        heat_on_btn.press(false);
        heat_off_btn.press(true);
        heat_auto_btn.press(false);
        heat_on_btn.drawButton(!heat_on_btn.isPressed());
        heat_off_btn.drawButton(!heat_off_btn.isPressed());
        heat_auto_btn.drawButton(!heat_auto_btn.isPressed());

        // mettre le 3 eme bit de mode à 1
        *mode = *mode & 248; // 1111 1000
        *mode = *mode | MODE_CHAUFFE_ARRET_FORCEE; // 0000 0100
    }
    else if (heat_auto_btn.contains(x, y)) // on a appuyé sur le bouton heat auto
    {
        heat_on_btn.press(false);
        heat_off_btn.press(false);
        heat_auto_btn.press(true);
        heat_on_btn.drawButton(!heat_on_btn.isPressed());
        heat_off_btn.drawButton(!heat_off_btn.isPressed());
        heat_auto_btn.drawButton(!heat_auto_btn.isPressed());

        // mettre le 1 er bit de mode à 1
        *mode = *mode & 248; // 1111 1000
        *mode = *mode | MODE_CHAUFFE_AUTO; // 0000 0001
    }
    else if (uv_on_btn.contains(x, y)) // on a appuyé sur le bouton uv on
    {
        uv_on_btn.press(true);
        uv_off_btn.press(false);
        uv_auto_btn.press(false);
        uv_on_btn.drawButton(!uv_on_btn.isPressed());
        uv_off_btn.drawButton(!uv_off_btn.isPressed());
        uv_auto_btn.drawButton(!uv_auto_btn.isPressed());

        // mettre le 5 eme bit de mode à 1
        *mode = *mode & 199; // 1100 0111
        *mode = *mode | MODE_UV_MARCHE_FORCEE; // 0001 0000
    }
    else if (uv_off_btn.contains(x, y)) // on a appuyé sur le bouton uv off
    {
        uv_on_btn.press(false);
        uv_off_btn.press(true);
        uv_auto_btn.press(false);
        uv_on_btn.drawButton(!uv_on_btn.isPressed());
        uv_off_btn.drawButton(!uv_off_btn.isPressed());
        uv_auto_btn.drawButton(!uv_auto_btn.isPressed());

        // mettre le 6 eme bit de mode à 1
        *mode = *mode & 199; // 1100 0111
        *mode = *mode | MODE_UV_ARRET_FORCEE; // 0010 0000
    }
    else if (uv_auto_btn.contains(x, y)) // on a appuyé sur le bouton uv auto
    {
        uv_on_btn.press(false);
        uv_off_btn.press(false);
        uv_auto_btn.press(true);
        uv_on_btn.drawButton(!uv_on_btn.isPressed());
        uv_off_btn.drawButton(!uv_off_btn.isPressed());
        uv_auto_btn.drawButton(!uv_auto_btn.isPressed());

        // mettre le 4 eme bit de mode à 1
        *mode = *mode & 199; // 1100 0111
        *mode = *mode | MODE_UV_AUTO; // 0000 1000
    }
}

// Ecrit la méthode setTemperaturePointChaud de la class Ecran
void Ecran::setTemperaturePointChaud(float temperaturePointChaud) {
    temperaturePointChaud_ = temperaturePointChaud;
}

void Ecran::setTemperaturePointFroid(float temperaturePointFroid) {
    temperaturePointFroid_ = temperaturePointFroid;
}

void Ecran::setTemperaturePointMedian(float temperaturePointMedian) {
    temperaturePointMedian_ = temperaturePointMedian;
}

void Ecran::setTemperatureGenerale(float temperatureGenerale) {
    temperatureGenerale_ = temperatureGenerale;
}

void Ecran::setHumiditeGenerale(float humiditeGenerale) {
    humiditeGenerale_ = humiditeGenerale;
}

void Ecran::setTemperatureDemandeeJour(float temperatureDemandeeJour) {
    temperatureDemandeeJour_ = temperatureDemandeeJour;
}

void Ecran::setTemperatureDemandeeNuit(float temperatureDemandeeNuit) {
    temperatureDemandeeNuit_ = temperatureDemandeeNuit;
}

void Ecran::setIsChauffeOn(bool isChauffeOn) {
    isChauffeOn_ = isChauffeOn;
}

void Ecran::setIsUVOn(bool isUVOn) {
    isUVOn_ = isUVOn;
}

