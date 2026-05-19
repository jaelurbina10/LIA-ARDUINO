#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor{
    protected:
        uint8_t _pin;
    public:
        Sensor(uint8_t p);

        virtual void begin() = 0;
        virtual void process() = 0;
        
};

#endif