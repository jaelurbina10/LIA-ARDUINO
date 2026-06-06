#include "dht11.h"

dht11::dht11(uint8_t p) : _pin(p), _dht11(_pin){
    _startTime = 0;
    _periodMs = 2000;
    _highTemp = 37;
    _lowTemp = 15;
    _highHum = 80;
    _lowHum = 40;
}

void dht11::begin() {}

void dht11::process() {
    if(millis() - _startTime >= _periodMs) {
        _startTime = millis();
        int a = 0, b = 0;
        _dhtReading = _dht11.readTemperatureHumidity(a,b);
        if(_dhtReading == 0) {
            _temperatura = a;
            _humedad = b;
        }else{
            Serial.print(millis());
            Serial.print(" --> DHT11 Error: ");
            Serial.println(_dhtReading);
        }
    }
}

void dht11::setThresholds(uint8_t highTemp, uint8_t lowTemp, uint8_t highHum, uint8_t lowHum) {
    _highTemp = highTemp;
    _lowTemp = lowTemp;
    _highHum = highHum;
    _lowHum = lowHum;
}


bool dht11::haceCalor() const {
    return _temperatura >= _highTemp;
}

bool dht11::haceFrio() const {
    return _temperatura <= _lowTemp;
}

bool dht11::muchaHumedad() const {
    return _humedad >= _highHum;
}

bool dht11::pocaHumedad() const {
    return _humedad <= _lowHum;
}

uint8_t dht11::getTemperatura() const {
    return _temperatura;
}

uint8_t dht11::getHumedad() const {
    return _humedad;
}