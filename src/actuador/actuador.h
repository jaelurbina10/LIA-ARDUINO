#ifndef ACTUADOR_H
#define ACTUADOR_H

#include <Arduino.h>

class Actuador{
    protected:
        bool _activo;
    public:

        Actuador();
    
        bool estado() const;
    
        virtual void encender() = 0;
        virtual void apagar() = 0;
};

#endif