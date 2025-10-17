#include <Arduino.h>
#include "ZonaRiego.h"
#include "IClima.h"
#include "ClimaSim.h"

// --- ZONAS: ajusta pines/umbrales a tu proyecto ---
ZonaRiego zona1(1, A0, 10, 300, 700);
ZonaRiego zona2(2, A1,  9, 400, 750);
ZonaRiego zona3(3, A2,  8, 350, 720);

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

  // 2) Actualiza zonas (humedad y clima)
  zona1.actualizarHumedad();
  zona2.actualizarHumedad();
  zona3.actualizarHumedad();

  // 3) Telemetría cada ~1 s
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
