#ifndef DHT11_H
#define DHT11_H

#include "sensor/sensor.h"
#include <DHT11.h>

class dht11 : public Sensor {
private:
    uint8_t _pin;
    DHT11 _dht11;
    int _dhtReading;
    
    uint8_t _temperatura;
    uint8_t _humedad;

    uint32_t _startTime;
    uint32_t _periodMs;

    uint8_t _highTemp;
    uint8_t _lowTemp;
    uint8_t _highHum;
    uint8_t _lowHum;

public:
    dht11(uint8_t p);

    void begin() override;
    void process() override;

    void setThresholds(uint8_t highTemp, uint8_t lowTemp, uint8_t highHum, uint8_t lowHum);

    bool haceCalor() const;
    bool haceFrio() const;
    bool muchaHumedad() const;
    bool pocaHumedad() const;

    uint8_t getTemperatura() const;
    uint8_t getHumedad() const;

    uint8_t getMaxhumidity() const;
    uint8_t getMaxTemp() const;
    uint8_t getMinTemp() const;
    uint8_t getMinHumidity() const;
};

#endif