#ifndef DHT11_H
#define DHT11_H

#include "sensor/sensor.h"
#include <DHT.h>

class dht11 : public Sensor {
private:
    uint8_t _pin;
    DHT _dht; 
    
    float _temperatura;
    float _humedad;

    unsigned long _ultimoTLectura;
    const unsigned long _interLectura = 1000;

    const float _umbralTempAlta = 37.0;
    const float _umbralTempBaja = 15.0;
    const float _umbralHumAlta = 80.0;
    const float _umbralHumBaja = 40.0;

public:
    dht11(uint8_t p);

    void begin() override;
    void process() override;

    bool muchoCalor() const;
    bool muchoFrio() const;
    bool muchaHumedad() const;
    bool pocaHumedad() const;

    float getTemperatura() const;
    float getHumedad() const;
};

#endif