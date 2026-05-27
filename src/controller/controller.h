#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <stdint.h>
#include "sensor.h"
#include "actuador.h"

namespace LIA{
    class Controller{
        private :
            uint8_t _NoSensores;
            uint8_t _NoActuadores;
            sensor** _sensores;
            actuador** _actuadores;

        public:
            Controller(uint8_t, uint8_t){};

            bool addSensor(sensor&, uint8_t){};

    }
}


#endif