#include "ServoActuador.h"

ServoActuador::ServoActuador(uint8_t p, uint8_t i, uint8_t a) 
    : _pin(p), _anguloInactivo(i), _anguloActivo(a) {
}

// Inicialización del hardware
void ServoActuador::begin() {
    _miServo.attach(_pin);
    _miServo.write(_anguloInactivo);
    _activo = false;
}

void ServoActuador::encender() {
    if (!_miServo.attached()) {
        _miServo.attach(_pin);
    }
    _miServo.write(_anguloActivo); 
    _activo = true;
}

void ServoActuador::apagar() {
    if (!_miServo.attached()) {
        _miServo.attach(_pin);
    }
    _miServo.write(_anguloInactivo); 
    _activo = false;
}

void ServoActuador::finalizar() {
    if (_miServo.attached()) {
        _miServo.detach();
    }
}