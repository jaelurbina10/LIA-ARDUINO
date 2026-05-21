#include "dht11.h"

dht11::dht11(uint8_t p) : _pin(p), _dht(p, DHT11), _temperatura(0.0), _humedad(0.0), _ultimoTLectura(0) {}

void dht11::begin() {
    _dht.begin();
}

void dht11::process() {
    unsigned long tiempoActual = millis();

    if (tiempoActual - _ultimoTLectura >= _interLectura) {
        
        _ultimoTLectura = tiempoActual;

        float t = _dht.readTemperature();
        float h = _dht.readHumidity();

        if (!isnan(t)) {
            _temperatura = t;
        }
        if (!isnan(h)) {
            _humedad = h;
        }
    }
}

bool dht11::haceCalor() const {
    return _temperatura >= _umbralTempAlta;
}

bool dht11::haceFrio() const {
    return _temperatura <= _umbralTempBaja;
}

bool dht11::muchaHumedad() const {
    return _humedad >= _umbralHumAlta;
}

bool dht11::pocaHumedad() const {
    return _humedad <= _umbralHumBaja;
}

float dht11::getTemperatura() const {
    return _temperatura;
}

float dht11::getHumedad() const {
    return _humedad;
}