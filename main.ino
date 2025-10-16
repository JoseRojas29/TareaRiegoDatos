#include "ZonaRiego.h"

// Horarios simulados (en minutos desde inicio del programa)
HorarioRiego horario1 = {1, 5, 10};   // Lunes, de 5 a 10 minutos
HorarioRiego horario2 = {2, 15, 25};  // Martes, de 15 a 25 minutos
HorarioRiego horario3 = {3, 30, 40};  // Miércoles, de 30 a 40 minutos

ZonaRiego zona1(1, A0, 10, 300, 700); // Rojo
ZonaRiego zona2(2, A1, 9, 400, 750); // Amarillo
ZonaRiego zona3(3, A2, 8, 350, 720); // Azul

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long tiempoActualMinutos = (millis() / 1000) / 60;
  
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