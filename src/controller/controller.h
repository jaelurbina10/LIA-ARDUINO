#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "sensor.h"
#include "actuador.h"

namespace LIA {

class Controller {
private:
    uint8_t _maxSensores;
    uint8_t _maxActuadores;
    
    uint8_t _sensorCount;
    uint8_t _actuadorCount;

    Sensor** _sensores;
    Actuador** _actuadores;

public:
    Controller(uint8_t maxSensores, uint8_t maxActuadores);

    bool addSensor(Sensor* s);
    bool addActuador(Actuador* a);

    virtual void begin() = 0;
    virtual void process() = 0;

protected:
    Sensor* getSensor(uint8_t index) const;
    Actuador* getActuador(uint8_t index) const;
    
    uint8_t getSensorCount() const;
    uint8_t getActuadorCount() const;
};

}

#endif