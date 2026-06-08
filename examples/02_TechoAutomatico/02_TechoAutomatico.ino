/*
  ============================================================
  EJEMPLO 02 - TECHO AUTOMÁTICO
  ============================================================

  Este ejemplo utiliza:

  - Sensor de lluvia YL83
  - Sensor de humedad de suelo HW080
  - Sensor DHT11
  - Servo para abrir/cerrar el techo
  - ControllerRoof

  El controlador decidirá automáticamente
  cuándo abrir o cerrar el techo.
*/

#include <lia.h>

using namespace LIA;

// Sensor de lluvia
yl83 lluvia(2);

// Sensor de humedad de suelo
hw080 suelo(A0);

// Sensor DHT11
dht11 clima(4);

// ServoActuador(pinServo, anguloCerrado, anguloAbierto)
//
// pinServo       -> pin PWM del servo
// anguloCerrado  -> posición cuando el techo está cerrado
// anguloAbierto  -> posición cuando el techo está abierto
ServoActuador techo(9, 0, 90);

ControllerRoof roofCtrl;

void setup()
{
  Serial.begin(9600);

  // Humedad máxima del suelo (%)
  suelo.setMaxHum(80);

  /*
    setThresholds()

    Orden de parámetros:

    setThresholds(
      temperaturaAlta,
      temperaturaBaja,
      humedadAlta,
      humedadBaja
    );
  */
  clima.setThresholds(
    30, // Temperatura alta
    15, // Temperatura baja
    80, // Humedad alta
    40  // Humedad baja
  );

  lluvia.begin();
  suelo.begin();
  clima.begin();

  techo.begin();

  // Asociar sensores y actuador
  roofCtrl.linkRainSensor(lluvia);
  roofCtrl.linkSoilHumiditySensor(suelo);
  roofCtrl.linkTempSensor(clima);
  roofCtrl.linkServo(techo);
}

void loop()
{
  lluvia.process();
  suelo.process();
  clima.process();

  roofCtrl.process();

  Serial.print("Temperatura: ");
  Serial.print(clima.getTemperatura());
  Serial.println(" C");

  delay(1000);
}