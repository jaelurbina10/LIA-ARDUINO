#ifndef HW080_H
#define HW080_H

#include "sensor/sensor.h"

//Sensor de humedad del suelo HW-080, basado en el sensor capacitivo de humedad del suelo. Proporciona una lectura de humedad relativa del suelo en porcentaje.

class hw080 : public Sensor {

private:
    float _humedadP;
    uint8_t _pin;
    uint8_t _maxHumedad;
    uint8_t _minHumedad;

public:
    hw080(uint8_t p);


    void begin() override;
    void process() override;

    float getPorHum() const;
    uint8_t getMaxHum() const;
    uint8_t getMinHum() const;
    void setMaxHum(uint8_t percent);
    void setMinHum(uint8_t percent);

};

#endif