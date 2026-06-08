#ifndef CONTROLLER_FUNGI_H
#define CONTROLLER_FUNGI_H

#include "controller/controller_base.h"

namespace LIA {
    class controllerFungi : public ControllerBase {
        private:
            uint32_t _uvOnTime;
            uint32_t _uvStartTime;
        public:
            controllerFungi();

            void setUvOnTimeMs(uint32_t time);

            void begin() override;
            void process() override;

            void linkLDRSensor(Sensor& ldr);
            void linkAirHumSensor(Sensor& dht11);
            void linkSoilHumiditySensor(Sensor& soil);
            
            void linkUvLight(Actuador& uv);
};
};



#endif