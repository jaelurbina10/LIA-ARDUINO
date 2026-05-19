#ifndef HW080_H
#define HW080_H

#include "sensor.h"


class hw080 : public Sensor {

private:
    float _humedad;

public:
    hw080(int p);

    float leerValor() override;

    //Posiblemente cambiar a controlador de humedad, define la humedad especifica que tiene que tener el suelo
    //void setHumedSuelo(float valor);

};

#endif