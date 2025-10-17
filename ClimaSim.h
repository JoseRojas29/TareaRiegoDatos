#pragma once
#include <Arduino.h>
#include "IClima.h"

class ClimaSim : public IClima {
  int _prob;          // 0..100
  uint8_t _pinPot;    // A3 (opcional)
  uint8_t _pinBtn;    // D4 (opcional)
  unsigned long _t0;
public:
  ClimaSim(uint8_t pinPot = A3, uint8_t pinBtn = 4)
  : _prob(0), _pinPot(pinPot), _pinBtn(pinBtn), _t0(0) {}

  void begin() {
    // Si no conectas A3/D4, igual funciona solo con Serial.
    pinMode(_pinPot, INPUT);
    pinMode(_pinBtn, INPUT_PULLUP);
  }

  void tick() override {
    // 1) Serial: "LLUVIA=85"
    while (Serial.available()) {
      String s = Serial.readStringUntil('\n');
      s.trim();
      int eq = s.indexOf('=');
      if (eq > 0) {
        String key = s.substring(0, eq); key.trim();
        String val = s.substring(eq + 1); val.trim();
        if (key.equalsIgnoreCase("LLUVIA")) {
          _prob = constrain(val.toInt(), 0, 100);
        }
      }
    }
    // 2) Botón: presionado => 80% (si está cableado)
    if (digitalRead(_pinBtn) == LOW) {
      _prob = 80;
      return;
    }
    // 3) Potenciómetro: mapea 0..1023 -> 0..100 (si está cableado)
    unsigned long now = millis();
    if (now - _t0 >= 200) {
      _t0 = now;
      int raw = analogRead(_pinPot);
      _prob = map(raw, 0, 1023, 0, 100);
    }
  }

  int probLluviaDia() override { return _prob; }
};
