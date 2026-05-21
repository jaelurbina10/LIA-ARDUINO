#include "ldr.h"

ldr::ldr(uint8_t p) : _pin(p), _uxRaw(0){}

void ldr::begin(){}

void ldr::process(){
    _uxRaw = analogRead(_pin);
}
bool ldr::esNoche() const{
    return _uxRaw <= _disparo;
}