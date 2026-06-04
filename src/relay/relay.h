#ifndef RELAY_H
#define RELAY_H

#include "actuador/actuador.h"

class relay : public Actuador{
    private:
        uint8_t _pin;
    public:
        relay(uint8_t p);

        void begin() override;

        void encender();
        void apagar();
};

#endif