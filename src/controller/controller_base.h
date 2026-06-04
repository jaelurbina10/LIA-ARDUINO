#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <Arduino.h>
#include "../sensor/sensor.h"
#include "../actuador/actuador.h"

namespace LIA {

class ControllerBase{
private:
    uint8_t _SenArrSize;
    uint8_t _ActArrSize;

    //Arrays para almacenar punteros (punteros de punteros)
    Sensor** _senPtrArr;
    Actuador** _actPtrArr;

public:
    ControllerBase(uint8_t noSensores, uint8_t noActuadores);

    void addSenPtr(Sensor* s, uint8_t index);
    void addActPtr(Actuador* a, uint8_t index);

    Sensor* getSensor(uint8_t index) const;
    Actuador* getActuador(uint8_t index) const;

    virtual void begin() = 0;
    virtual void process() = 0;

};
};

#endif