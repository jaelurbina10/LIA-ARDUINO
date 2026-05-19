#include "actuador.h"

Actuador::Actuador(uint8_t p) : _pin(p), _activo(false){
    pinMode(_pin, OUTPUT);
}

bool Actuador::estado() const{
    return this->_activo;
}