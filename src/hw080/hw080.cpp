#include "hw080.h"

hw080::hw080(uint8_t p) : _pin(p), _humedadP(0){}

void hw080::begin(){}

void hw080::process(){
    this->_humedadP = map(analogRead(_pin), 0, 1023, 100, 0);
}
uint8_t hw080::getPorHum() const{
    return this->_humedadP;
}