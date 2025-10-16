// ZonaRiego.h
#ifndef ZONARIEGO_H
#define ZONARIEGO_H

struct HorarioRiego {
    int diaSemana;     // 0 = Domingo, 1 = Lunes, ..., 6 = Sábado
    int horaInicio;    // En minutos desde medianoche
    int horaFin;       // En minutos desde medianoche
};

class ZonaRiego {
private:
    int idZona;          // Identificador de la zona (ej. 1, 2, 3)
    int pinSensor;       // Pin donde está conectado el sensor de humedad
    int pinRiego;        // Pin que controla la bomba o válvula de riego
    int humedadMin;      // Umbral mínimo de humedad para activar riego
    int humedadMax;      // Umbral máximo para desactivar riego
    bool estadoRiego;    // Estado actual del riego (true = encendido)

public:
    ZonaRiego(int id, int sensorPin, int riegoPin, int minH, int maxH); // Constructor para inicializar la zona
    
    actualizarHumedad(); // Decide si se riega o no según humedad actual
    debeRegar();         // Retorna true si la humedad está por debajo del mínimo
    activarRiego();      // Enciende el riego
    desactivarRiego();   // Apaga el riego
    leerHumedad();       // Lee el valor del sensor
    getEstadoRiego();    // Retorna si el riego está activo o no

};

#endif

// ZonaRiego.cpp
#include "ZonaRiego.h"
#include <Arduino.h>

ZonaRiego::ZonaRiego(int id, int sensorPin, int riegoPin, int minH, int maxH, HorarioRiego h) {
    idZona = id;
    pinSensor = sensorPin;
    pinRiego = riegoPin;
    humedadMin = minH;
    humedadMax = maxH;
    estadoRiego = false;
    horario = h;
    pinMode(pinRiego, OUTPUT); // Configura el pin de riego como salida
}

int ZonaRiego::leerHumedad() {
    return analogRead(pinSensor); // Simulación básica
}

void ZonaRiego::actualizar(unsigned long tiempoActualMinutos) {
    if (estaEnHorario(tiempoActualMinutos) && debeRegar()) {
        activarRiego();
    } else {
        desactivarRiego();
    }
}

bool ZonaRiego::debeRegar() {
    int humedad = leerHumedad();
    return humedad < humedadMin;
}

bool ZonaRiego::estaEnHorario(unsigned long tiempoActualMinutos) {
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

bool ZonaRiego::getEstadoRiego() {
    return estadoRiego;
}
