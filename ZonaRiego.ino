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
