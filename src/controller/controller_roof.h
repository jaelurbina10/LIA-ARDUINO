#ifndef CONTROLLER_ROOF_H
#define CONTROLLER_ROOF_H
#include "../controller/controller_base.h"

namespace LIA {
    class ControllerRoof : public ControllerBase {


        public:
            ControllerRoof();

            void begin() override;
            void process() override;

            // Enlazar sensores y actuadores específicos
            void linkRainSensor(Sensor& rainSensor);
            void linkSoilHumiditySensor(Sensor& humiditySensor);
            void linkTempSensor(Sensor& tempSensor);
            void linkServo(Actuador& servo);
    };
};
#endif