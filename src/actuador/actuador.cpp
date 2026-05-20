#include "actuador.h"

Actuador::Actuador() : _activo(false){}

bool Actuador::estado() const{
    return this->_activo;
}