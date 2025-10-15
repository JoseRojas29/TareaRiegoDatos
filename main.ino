#include "ZonaRiego.h"

ZonaRiego zona1(1, A0, 10, 300, 700); // Rojo
ZonaRiego zona2(2, A1, 9, 400, 750); // Amarillo
ZonaRiego zona3(3, A2, 8, 350, 720); // Azul

void setup() {
  Serial.begin(9600);
}

void loop() {
  zona1.actualizarHumedad();
  zona2.actualizarHumedad();
  zona3.actualizarHumedad();

  Serial.print("Zona 1: ");
  Serial.println(zona1.getEstadoRiego());
  Serial.print("Zona 2: ");
  Serial.println(zona2.getEstadoRiego());
  Serial.print("Zona 3: ");
  Serial.println(zona3.getEstadoRiego());
  
  delay(2000);
}