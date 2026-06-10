#include <Arduino.h>
#include "controller/controller_fungi/controller_fungi.h"
#include "ldr/ldr.h"
#include "dht11/dht11.h"
#include "hw080/hw080.h"
#include "relay/relay.h"

LIA::controllerFungi::controllerFungi() : _uvOnTime(500), _uvStartTime(0), ControllerBase(3,1) {}

void LIA::controllerFungi::setUvOnTimeMs(uint32_t t) {
    _uvOnTime = t;
}

void LIA::controllerFungi::begin() {}

// Sensor and actuator linkers
// LDR -> 0
// DHT11 -> 1
// Soil Humidity -> 2
// UV Light -> 0

void LIA::controllerFungi::linkLDRSensor(Sensor& ldr) {
    addSenPtr(&ldr, 0);
}
void LIA::controllerFungi::linkAirHumSensor(Sensor& dht11) {
    addSenPtr(&dht11, 1);
}
void LIA::controllerFungi::linkSoilHumiditySensor(Sensor& soil) {
    addSenPtr(&soil, 2);
}
void LIA::controllerFungi::linkUvLight(Actuador& uv) {
    addActPtr(&uv, 0);
}

void LIA::controllerFungi::process(){
    //Method linker for sensors and actuators
    ldr* LDR = (ldr*)ControllerBase::getSensor(0);
    dht11* DHT11 = (dht11*)ControllerBase::getSensor(1);
    hw080* soilSensor = (hw080*)ControllerBase::getSensor(2);
    relay* uvLight = (relay*)ControllerBase::getActuador(0);


    //Lógica de control

    bool a = LDR->esNoche();
    bool b = soilSensor->getPorHum() > soilSensor->getMaxHum();
    bool c = DHT11->getHumedad() > DHT11->getMaxhumidity();

    if((!a && b && c) || (a && b && !c)){
        if(millis() - _uvStartTime >= _uvOnTime) {
            if(uvLight->estado()) {
                uvLight->apagar();
            }else{
                uvLight->encender();
            }
            _uvStartTime = millis();
        }
    }else{
        uvLight->apagar();
        _uvStartTime = millis();
    }
}