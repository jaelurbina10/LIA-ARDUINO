#include <Arduino.h>
#include "../controller/controller_watering.h"
#include "../yl83/yl83.h"
#include "../hw080/hw080.h"
#include "../relay/relay.h"

LIA::ControllerWatering::ControllerWatering():ControllerBase(2,1){
    _maxRunTimeMs = 60000; // Tiempo máximo de riego
    _startTime = 0;
}

void LIA::ControllerWatering::setWateringTime(uint32_t time){
    _maxRunTimeMs = time;
}

// Enlazar sensores y actuadores específicos
// Array position:
// 0: Sensor de lluvia
// 1: Sensor de humedad del suelo
// 0: Bomba de agua

void LIA::ControllerWatering::linkRainSensor(Sensor& rainSensor){
    addSenPtr(&rainSensor, 0);
}
void LIA::ControllerWatering::linkSoilHumiditySensor(Sensor& humiditySensor){
    addSenPtr(&humiditySensor, 1);
}
void LIA::ControllerWatering::linkWaterPump(Actuador& waterPump){
    addActPtr(&waterPump, 0);
}

void LIA::ControllerWatering::begin(){
    return;
}

void LIA::ControllerWatering::process(){
    // === METHOD GETTERS ===
    // Sensores
    yl83* rainSensor = (yl83*)ControllerBase::getSensor(0);
    hw080* soilSensor = (hw080*)ControllerBase::getSensor(1);
    //Actuador
    relay* waterPump = (relay*)ControllerBase::getActuador(0);

    // Lógica de control

    // === RIEGO ===

    if(!rainSensor->estaLloviendo() && soilSensor->getPorHum() < soilSensor->getMinHum()){
         
        if(!waterPump->estado()) {
            waterPump->encender();
            _startTime = millis();
        }
        if(millis() - _startTime >= _maxRunTimeMs){
            waterPump->apagar();
        }

    }else{
        waterPump->apagar();
    }

    return;
}
