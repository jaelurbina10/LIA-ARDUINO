#include "ldr.h"

ldr::ldr(uint8_t p) : _pin(p), _uxRaw(0), _disparo(200){}

void ldr::begin(){}

void ldr::process(){
    _uxRaw = analogRead(_pin);
    //invertir valor
    _uxRaw = 1023 - _uxRaw;
}

void ldr::setDisparo(uint16_t d){
    if(d > 1023) d = 1023;
    else if(d < 0) d = 0;
    else _disparo = d;
}

uint16_t ldr::getDisparo() const{
    return _disparo;
}

uint16_t ldr::getRawLux(){
    return _uxRaw;
}

bool ldr::esNoche() const{
    return _uxRaw <= _disparo;
}