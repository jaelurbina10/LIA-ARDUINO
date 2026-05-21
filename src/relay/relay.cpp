#include "relay/relay.h"

relay::relay(uint8_t p) : _pin(p);

void relay::begin(){
    pinmode(_pin, OUTPUT);
}
void relay::encender(){
    digitalWrite(_pin, HIGH);
    _activo = true;
}
void relay::apagar(){
    digitalWrite(_pin, LOW);
    _activo = false;
}