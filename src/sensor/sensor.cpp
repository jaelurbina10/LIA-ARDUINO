#include "sensor.h"

Sensor::Sensor(uint8_t p) : _pin(p){
    pinMode(_pin, INPUT);
}