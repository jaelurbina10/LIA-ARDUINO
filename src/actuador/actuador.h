#ifndef ACTUADOR.H
#define ACTUADOR.H

#include <Arduino.h>

class Actuador{
    protected:
        uint8_t _pin;
        bool _activo;
    public:

        Actuador(uint8_t p);
    
        bool estado() const;
    
        virtual void encender() = 0;
        virtual void apagar() = 0;
};

#endif