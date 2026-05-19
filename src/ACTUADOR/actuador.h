#ifndef ACTUADOR.H
#define ACTUADOR.H

#include <Arduino.h>

class Actuador{
    protected:
        uint8_t pin;
        bool activo;
    public:

        Actuador(uint8_t pin);
    
        bool estado() const;
    
        virtual void encender() = 0;
        virtual void apagar() = 0;
};

#endif