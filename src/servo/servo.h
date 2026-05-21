#ifndef SERVO_ACTUADOR_H
#define SERVO_ACTUADOR_H

#include "actuador/actuador.h"
#include <Servo.h> 

class ServoActuador : public Actuador {
private:
    uint8_t _pin;
    uint8_t _anguloInactivo;
    uint8_t _anguloActivo;
    
    Servo _miServo;

public:
    ServoActuador(uint8_t pin, uint8_t anguloInactivo, uint8_t anguloActivo);

    void begin() override;
    void encender() override; 
    void apagar() override;   

    void finalizar();
};

#endif