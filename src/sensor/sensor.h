#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor{
    public:
        Sensor();

        virtual void begin() = 0;
        virtual void process() = 0;

};

#endif