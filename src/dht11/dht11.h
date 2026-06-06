#ifndef DHT11_H
#define DHT11_H

#include "sensor/sensor.h"
#include <DHT11.h

class dht11 : public Sensor {
private:
    uint8_t _pin;
    DHT11 _dht11;
    int _dhtReading;
    
    float _temperatura;
    float _humedad;

    float _startTime;
    uint32_t _periodMs;

    float _highTemp;
    float _lowTemp;
    float _highHum;
    float _lowHum;

public:
    dht11(uint8_t p);

    void begin() override;
    void process() override;

    void setThresholds(float highTemp, float lowTemp, float highHum, float lowHum);

    bool haceCalor() const;
    bool haceFrio() const;
    bool muchaHumedad() const;
    bool pocaHumedad() const;

    float getTemperatura() const;
    float getHumedad() const;
};

#endif