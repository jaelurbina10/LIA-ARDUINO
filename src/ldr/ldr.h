#ifndef LDR_H
#define LDR_H

#include "sensor/sensor.h"

class ldr : public Sensor{
    private:
        uint16_t _disparo;
        uint16_t _uxRaw;
        uint8_t _pin;
    public:
        ldr(uint8_t p);

        void begin() override;
        void process() override;

        void setDisparo(uint16_t d);
        uint16_t getRawLux();
        uint16_t getDisparo() const;
        bool esNoche() const;
};

#endif