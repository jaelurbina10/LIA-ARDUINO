#ifndef HW080_H
#define HW080_H

#include "sensor/sensor.h"


class hw080 : public Sensor {

private:
    uint8_t _humedadP;
    uint8_t _pin;

public:
    hw080(uint8_t p);


    void begin() override;
    void process() override;

    uint8_t getPorHum() const;

    //Posiblemente cambiar a controlador de humedad, define la humedad especifica que tiene que tener el suelo
    //void setHumedSuelo(float valor);

};

#endif