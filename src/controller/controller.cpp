#include "controller.h"
#include <stdlib.h>

namespace LIA {

Controller::Controller(uint8_t maxSensores, uint8_t maxActuadores) {
    _maxSensores = maxSensores;
    _maxActuadores = maxActuadores;
    _sensorCount = 0;
    _actuadorCount = 0;

    _sensores = (Sensor**) calloc(_maxSensores, sizeof(Sensor*));
    _actuadores = (Actuador**) calloc(_maxActuadores, sizeof(Actuador*));
    
}

bool Controller::addSensor(Sensor* s) {
    if (_sensorCount < _maxSensores && s != nullptr) {
        _sensores[_sensorCount] = s;
        _sensorCount++;
        return true;
    }
    return false;
}

bool Controller::addActuador(Actuador* a) {
    if (_actuadorCount < _maxActuadores && a != nullptr) {
        _actuadores[_actuadorCount] = a;
        _actuadorCount++;
        return true;
    }
    return false;
}

Sensor* Controller::getSensor(uint8_t index) const {
    if (index < _sensorCount) {
        return _sensores[index];
    }
    return nullptr;
}

Actuador* Controller::getActuador(uint8_t index) const {
    if (index < _actuadorCount) {
        return _actuadores[index];
    }
    return nullptr;
}

uint8_t Controller::getSensorCount() const {
    return _sensorCount;
}

uint8_t Controller::getActuadorCount() const {
    return _actuadorCount;
}

}