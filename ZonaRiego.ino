// ZonaRiego.h
#ifndef ZONARIEGO_H
#define ZONARIEGO_H

class ZonaRiego {
private:
    int idZona;
    int pinSensor;
    int pinRiego;
    int humedadMin;
    int humedadMax;
    bool estadoRiego;

public:
    ZonaRiego(int id, int sensorPin, int riegoPin, int minH, int maxH);

    void actualizarHumedad();
    bool debeRegar();
    void activarRiego();
    void desactivarRiego();
    int leerHumedad();
    bool getEstadoRiego();
};

#endif

// ZonaRiego.cpp
#include "ZonaRiego.h"
#include <Arduino.h>

ZonaRiego::ZonaRiego(int id, int sensorPin, int riegoPin, int minH, int maxH) {
    idZona = id;
    pinSensor = sensorPin;
    pinRiego = riegoPin;
    humedadMin = minH;
    humedadMax = maxH;
    estadoRiego = false;
    pinMode(pinRiego, OUTPUT);
}

int ZonaRiego::leerHumedad() {
    return analogRead(pinSensor); // Simulación básica
}

void ZonaRiego::actualizarHumedad() {
    int humedad = leerHumedad();
    if (humedad < humedadMin) {
        activarRiego();
    } else if (humedad > humedadMax) {
        desactivarRiego();
    }
}

bool ZonaRiego::debeRegar() {
    int humedad = leerHumedad();
    return humedad < humedadMin;
}

void ZonaRiego::activarRiego() {
    digitalWrite(pinRiego, HIGH);
    estadoRiego = true;
}

void ZonaRiego::desactivarRiego() {
    digitalWrite(pinRiego, LOW);
    estadoRiego = false;
}

bool ZonaRiego::getEstadoRiego() {
    return estadoRiego;
}

