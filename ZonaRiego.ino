// ZonaRiego.h
#ifndef ZONARIEGO_H
#define ZONARIEGO_H

struct HorarioRiego {
    int horaInicio;       // En minutos desde medianoche
    int horaFin;          // En minutos desde medianoche
    int frecuenciaDias;   // Cada cuántos días se permite regar
    int ultimoRiego;      // Día del sistema en que se regó por última vez
    bool zonaActiva;      // Si la zona está habilitada o no
};

class ZonaRiego {
private:
    int idZona;          // 1,2,3
    int pinSensor;       // A0/A1/A2
    int pinRiego;        // D10/D9/D8 (o los que uses)
    int humedadMin;      // umbral mínimo (activa)
    int humedadMax;      // umbral máximo (apaga)
    bool estadoRiego;    // true = ON
    bool bloqueoGlobal;  // true = bloqueo por clima
    HorarioRiego horario;
    
    bool estaEnHorario(unsigned long tiempoActualMinutos) const;

public:
    ZonaRiego(int id, int sensorPin, int riegoPin, int minH, int maxH, HorarioRiego h);

    // Inicialización de pines/estado. Solo hace acciones -> void
    void begin();

    // Activa/desactiva el bloqueo global por clima. Solo cambia estado -> void
    void setBloqueoGlobal(bool on);

    // Decide encender/apagar según humedad y bloqueo. Solo efectos -> void
    void actualizarHumedad();

    // ¿Está por debajo del mínimo de humedad? -> devuelve SÍ/NO
    bool debeRegar() const;

    // Enciende/apaga el actuador. Solo efectos -> void
    void activarRiego();
    void desactivarRiego();

    // Lee el sensor de humedad (0..1023 o 0..100). Devuelve un número -> int
    int leerHumedad() const;

    // ¿El riego está actualmente encendido? -> devuelve SÍ/NO
    bool getEstadoRiego() const;

    void actualizar(unsigned long tiempoActualMinutos, int diaActual);
};

#endif


// ZonaRiego.cpp}
#pragma once
#include "ZonaRiego.h"
#include <Arduino.h>

ZonaRiego::ZonaRiego(int id, int sensorPin, int riegoPin, int minH, int maxH, HorarioRiego h) {
    idZona = id;
    pinSensor = sensorPin;
    pinRiego = riegoPin;
    humedadMin = minH;
    humedadMax = maxH;
    estadoRiego = false;
    bloqueoGlobal = false;
    horario = h;
}

void ZonaRiego::begin() {
    pinMode(pinRiego, OUTPUT);
    desactivarRiego();
}

void ZonaRiego::setBloqueoGlobal(bool on) {
    bloqueoGlobal = on;
}

void ZonaRiego::actualizar(unsigned long tiempoActualMinutos, int diaActual) {
    if (!horario.zonaActiva || bloqueoGlobal) {
        desactivarRiego();
        return;
    }

    if (estadoRiego) {
        if (leerHumedad() >= humedadMax) {
            desactivarRiego();
        }
        return;
    }

    if (estaEnHorario(tiempoActualMinutos) &&
        debeRegar() &&
        (diaActual - horario.ultimoRiego >= horario.frecuenciaDias)) {
        
        activarRiego();
        horario.ultimoRiego = diaActual;
    } else {
        desactivarRiego();
    }
}

bool ZonaRiego::debeRegar() const {
    int humedad = leerHumedad();
    return humedad < humedadMin;
}

bool ZonaRiego::estaEnHorario(unsigned long tiempoActualMinutos) const {
    return tiempoActualMinutos >= horario.horaInicio && tiempoActualMinutos <= horario.horaFin;
}

void ZonaRiego::activarRiego() {
    digitalWrite(pinRiego, HIGH);
    estadoRiego = true;
}

void ZonaRiego::desactivarRiego() {
    digitalWrite(pinRiego, LOW);
    estadoRiego = false;
}

int ZonaRiego::leerHumedad() const {
    return analogRead(pinSensor);
}

bool ZonaRiego::getEstadoRiego() const {
    return estadoRiego;
}
