#include "yl83.h"

yl83::yl83(uint8_t p) : _pin(p), _llueve(false){}

void yl83::begin(){
    pinMode(_pin, INPUT);
}
void yl83::process(){
    _llueve = !digitalRead(_pin);
}
bool yl83::estaLLoviendo() const{
    return this->_llueve;
}