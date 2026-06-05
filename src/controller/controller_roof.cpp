#include <Arduino.h>
#include "../controller/controller_roof.h"
#include "../yl83/yl83.h"
#include "../hw080/hw080.h"
#include "../dht11/dht11.h"
#include "../servo/servo.h"


LIA::ControllerRoof::ControllerRoof() : ControllerBase(3,1){

}

void LIA::ControllerRoof::begin() {}

//Enlazar sensores y actuadores específicos
//Sensor lluvia -> 0
//Sensor humedad -> 1
//Sensor temperatura -> 2
//Actuador servo -> 0
void LIA::ControllerRoof::linkRainSensor(Sensor& rS){
    addSenPtr(&rS, 0);
}
void LIA::ControllerRoof::linkSoilHumiditySensor(Sensor& hS){
    addSenPtr(&hS, 1);
}
void LIA::ControllerRoof::linkTempSensor(Sensor& tS){
    addSenPtr(&tS, 2);
}
void LIA::ControllerRoof::linkServo(Actuador& s){
    addActPtr(&s, 0);
}

void LIA::ControllerRoof::process(){
    //===METHOD GETTERS===
    //Sensores
    yl83* rainSensor = (yl83*)ControllerBase::getSensor(0);
    hw080* soilSensor = (hw080*)ControllerBase::getSensor(1);
    dht11* temphSensor = (dht11*)ControllerBase::getSensor(2);
    //Actuador
    ServoActuador* roofServo = (ServoActuador*)ControllerBase::getActuador(0);

    //===LÓGICA DE CONTROL===
    bool a = rainSensor->estaLloviendo();
    bool b = soilSensor->getPorHum() >= soilSensor->getMaxHum();
    bool c = temphSensor->getTemperatura() >= temphSensor->haceCalor();

    if(a && !b || !a && b && c){
        roofServo->encender();
    }
    else{
        roofServo->apagar();
    }
    
}