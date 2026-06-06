#include "hw080.h"

hw080::hw080(uint8_t p) : _pin(p), _humedadP(0), _maxHumedad(80), _minHumedad(10){}

void hw080::begin(){}

void hw080::process(){
    this->_humedadP = analogRead(_pin) * (100.0 / 1023.0); // Convertir a porcentaje
}
//Setters
void hw080::setMaxHum(uint8_t p){
    if(p>=0 && p<=100){
        this->_maxHumedad = p;
    }
}
void hw080::setMinHum(uint8_t p){
    if(p>=0 && p<=100){
        this->_minHumedad = p;
    }
}
//Getters
float hw080::getPorHum() const{
    return this->_humedadP;
}
uint8_t hw080::getMaxHum() const{
    return this->_maxHumedad;
}
uint8_t hw080::getMinHum() const{
    return this->_minHumedad;
}