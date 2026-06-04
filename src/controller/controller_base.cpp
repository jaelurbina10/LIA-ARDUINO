#include "../controller/controller_base.h"
#include <stdlib.h>

LIA::ControllerBase::ControllerBase(uint8_t noSen, uint8_t noAct) {
    _SenArrSize = noSen;
    _ActArrSize = noAct;
    //Contenedor de punteros a sensores y actuadores
    _senPtrArr = (Sensor**)calloc(_SenArrSize, sizeof(Sensor*));
    _actPtrArr = (Actuador**)calloc(_ActArrSize, sizeof(Actuador*));

}

void LIA::ControllerBase::addSenPtr(Sensor* s, uint8_t index){
    _senPtrArr[index] = s;
    return;
}
void LIA::ControllerBase::addActPtr(Actuador* a, uint8_t index){
    _actPtrArr[index] = a;
    return;
}

Sensor* LIA::ControllerBase::getSensor(uint8_t index) const{
    return _senPtrArr[index];
}
Actuador* LIA::ControllerBase::getActuador(uint8_t index) const{
    return _actPtrArr[index];
}