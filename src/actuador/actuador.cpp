#include "actuador.h"

Actuador::Actuador(uint8_t p) : _pin(p), _activo(false){}

bool Actuador::estado() const{
    return this->_activo;
}