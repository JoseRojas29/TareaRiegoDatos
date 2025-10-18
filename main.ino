#include <Arduino.h>
#include "ZonaRiego.h"
#include "IClima.h"
#include "ClimaSim.h"

// --- HORARIOS: simulados en minutos desde medianoche ---
HorarioRiego horario1 = {420, 435, 1, -1, true};  // 7:00–7:15 AM, diario
HorarioRiego horario2 = {1260, 1320, 3, -1, true}; // 9:00–10:00 PM, cada 3 días
HorarioRiego horario3 = {0, 0, 1, -1, false};      // zona desactivada

// --- ZONAS: pines/umbrales ajustados al proyecto ---
ZonaRiego zona1(1, A0, 10, 300, 700, horario1);
ZonaRiego zona2(2, A1,  9, 400, 750, horario2);
ZonaRiego zona3(3, A2,  8, 350, 720, horario3);

// --- CLIMA: simulador (Serial + opcional A3/D4) ---
ClimaSim clima;
const int UMBRAL_LLUVIA = 70;  // %

static void aplicarBloqueoTodas(bool on) {
  zona1.setBloqueoGlobal(on);
  zona2.setBloqueoGlobal(on);
  zona3.setBloqueoGlobal(on);
}

void setup() {
  Serial.begin(115200);

  zona1.begin();
  zona2.begin();
  zona3.begin();

  clima.begin();

  Serial.println("Hidrosmart - Control de riego con bloqueo por clima (>70%)");
  Serial.println("Comando Serial: LLUVIA=85");
}

void loop() {
  // 1) Actualiza clima y calcula bloqueo global
  clima.tick();
  const int prob = clima.probLluviaDia();
  const bool bloqueo = (prob > UMBRAL_LLUVIA);
  aplicarBloqueoTodas(bloqueo);

  // 2) Calcular tiempo actual en minutos y día simulado
  unsigned long tiempoActualMinutos = (millis() / 1000) / 60;
  int diaActual = tiempoActualMinutos / 1440;

  // 3) Actualiza zonas con lógica completa
  zona1.actualizar(tiempoActualMinutos % 1440, diaActual);
  zona2.actualizar(tiempoActualMinutos % 1440, diaActual);
  zona3.actualizar(tiempoActualMinutos % 1440, diaActual);

  // 4) Telemetría cada ~1 s
  static unsigned long t0 = 0;
  unsigned long now = millis();
  if (now - t0 >= 1000) {
    t0 = now;
    Serial.print("[CLIMA] ProbLluvia=");
    Serial.print(prob);
    Serial.print("%  Bloqueo=");
    Serial.println(bloqueo ? "SI" : "NO");

    Serial.print("Z1 H=");
    Serial.print(zona1.leerHumedad());
    Serial.print(" ");
    Serial.print(zona1.getEstadoRiego() ? "ON" : "OFF");

    Serial.print(" | Z2 H=");
    Serial.print(zona2.leerHumedad());
    Serial.print(" ");
    Serial.print(zona2.getEstadoRiego() ? "ON" : "OFF");

    Serial.print(" | Z3 H=");
    Serial.print(zona3.leerHumedad());
    Serial.print(" ");
    Serial.println(zona3.getEstadoRiego() ? "ON" : "OFF");
  }
}
