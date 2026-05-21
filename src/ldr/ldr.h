#ifndef LDR_H
#define LDR_H

#include "sensor/sensor.h"

class ldr : public Sensor{
    private:
        const uint8_t _disparo = 200;
        uint16_t _uxRaw;
        uint8_t _pin;
    public:
        ldr(uint8_t p);

        void begin() override;
        void process() override;

        bool esNoche() const;
};

#endif