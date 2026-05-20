#ifndef YL83_H
#define YL83_H

#include "sensor.h"

class yl83 : public Sensor {
    private:
        uint8_t _pin;
        bool _llueve;

    public:
        yl83(uint8_t p);

        void begin() override;
        void process() override;

        bool estaLloviendo() const;

};

#endif