#include "dht11.h"

LIA::dht11::dht11(uint8_t p) : _pin(p), _dht11(_pin){
    _startTime = 0;
    _periodMs = 2000;
    _highTemp = 37;
    _lowTemp = 15;
    _highHum = 80;
    _lowHum = 40;
}

void LIA::dht11::begin() {}

void LIA::dht11::process() {
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

void LIA::dht11::setThresholds(uint8_t highTemp, uint8_t lowTemp, uint8_t highHum, uint8_t lowHum) {
    _highTemp = highTemp;
    _lowTemp = lowTemp;
    _highHum = highHum;
    _lowHum = lowHum;
}


bool LIA::dht11::haceCalor() const {
    return _temperatura >= _highTemp;
}

bool LIA::dht11::haceFrio() const {
    return _temperatura <= _lowTemp;
}

bool LIA::dht11::muchaHumedad() const {
    return _humedad >= _highHum;
}

bool LIA::dht11::pocaHumedad() const {
    return _humedad <= _lowHum;
}

uint8_t LIA::dht11::getTemperatura() const {
    return _temperatura;
}

uint8_t LIA::dht11::getHumedad() const {
    return _humedad;
}

uint8_t LIA::dht11::getMaxhumidity() const {
    return _highHum;
}

uint8_t LIA::dht11::getMaxTemp() const {
    return _highTemp;
}

uint8_t LIA::dht11::getMinTemp() const {
    return _lowTemp;
}

uint8_t LIA::dht11::getMinHumidity() const {
    return _lowHum;
}
