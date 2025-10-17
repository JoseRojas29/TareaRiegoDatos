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
    int idZona;          // 1,2,3
    int pinSensor;       // A0/A1/A2
    int pinRiego;        // D10/D9/D8 (o los que uses)
    int humedadMin;      // umbral mínimo (activa)
    int humedadMax;      // umbral máximo (apaga)
    bool estadoRiego;    // true = ON
    bool bloqueoGlobal;  // true = bloqueo por clima

    int leerHumedadCruda();
    int filtrar(int muestra);

public:
    // Constructor: no lleva tipo de retorno (los constructores nunca devuelven valor)
    ZonaRiego(int id, int sensorPin, int riegoPin, int minH, int maxH);

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

private:
    // ... tus atributos ...
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
