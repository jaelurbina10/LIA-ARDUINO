#ifndef CONTROLLER_WATERING_H
#define CONTROLLER_WATERING_H

#include "../controller/controller_base.h"

namespace LIA {

class ControllerWatering : public ControllerBase {
  private:
    uint32_t _maxRunTimeMs;
    uint32_t _startTime;

  public:
    ControllerWatering();

    void setWateringTime(uint32_t time);

    void begin() override;
    void process() override;

    void linkRainSensor(Sensor& rainSensor);
    void linkSoilHumiditySensor(Sensor& humiditySensor);
    void linkWaterPump(Actuador& waterPump);
    void linkServo(Actuador& servo);

};
};
#endif
