/*
  ============================================================
  EJEMPLO 04 - INVERNADERO COMPLETO
  ============================================================

  Este ejemplo utiliza TODOS los elementos principales
  de la librería:

  SENSORES
  --------
  - HW080 (humedad de suelo)
  - YL83 (lluvia)
  - DHT11 (temperatura y humedad)
  - LDR (día/noche)

  ACTUADORES
  ----------
  - Relé para bomba de agua
  - Servo para techo
  - Relé para luz UV

  CONTROLADORES
  -------------
  - ControllerWatering
  - ControllerRoof
  - controllerFungi
*/

#include <lia.h>

using namespace LIA;

// ------------------------------------------------------------
// Sensores
// ------------------------------------------------------------

hw080 suelo(A0);
yl83 lluvia(2);
dht11 clima(4);
ldr luz(A1);

// ------------------------------------------------------------
// Actuadores
// ------------------------------------------------------------

relay bomba(8);
relay uv(7);

ServoActuador techo(
  9,   // Pin del servo
  0,   // Ángulo cerrado
  90   // Ángulo abierto
);

// ------------------------------------------------------------
// Controladores
// ------------------------------------------------------------

ControllerWatering watering;
ControllerRoof roof;
controllerFungi fungi;

void setup()
{
  Serial.begin(115200);

  // Configuración humedad mínima para riego
  suelo.setMinHum(35);

  // Configuración humedad máxima para techo y hongos
  suelo.setMaxHum(80);

  /*
    setThresholds(
      temperaturaAlta,
      temperaturaBaja,
      humedadAlta,
      humedadBaja
    );
  */
  clima.setThresholds(
    30,
    15,
    85,
    40
  );

  // Inicializar sensores
  suelo.begin();
  lluvia.begin();
  clima.begin();
  luz.begin();

  // Inicializar actuadores
  bomba.begin();
  uv.begin();
  techo.begin();

  // --------------------------
  // Controlador de riego
  // --------------------------
  watering.linkRainSensor(lluvia);
  watering.linkSoilHumiditySensor(suelo);
  watering.linkWaterPump(bomba);

  watering.setWateringTime(20000);

  // --------------------------
  // Controlador de techo
  // --------------------------
  roof.linkRainSensor(lluvia);
  roof.linkSoilHumiditySensor(suelo);
  roof.linkTempSensor(clima);
  roof.linkServo(techo);

  // --------------------------
  // Controlador de hongos
  // --------------------------
  fungi.linkLDRSensor(luz);
  fungi.linkAirHumSensor(clima);
  fungi.linkSoilHumiditySensor(suelo);
  fungi.linkUvLight(uv);

  fungi.setUvOnTimeMs(1500);
}

void loop()
{
  // Actualizar todos los sensores
  suelo.process();
  lluvia.process();
  clima.process();
  luz.process();

  // Ejecutar todos los controladores
  watering.process();
  roof.process();
  fungi.process();

  // Mostrar información en el monitor serial
  Serial.println("===== ESTADO DEL SISTEMA =====");

  Serial.print("Lluvia: ");
  Serial.println(lluvia.estaLloviendo());

  Serial.print("Temperatura: ");
  Serial.print(clima.getTemperatura());
  Serial.println(" C");

  Serial.print("Humedad aire: ");
  Serial.print(clima.getHumedad());
  Serial.println("%");

  Serial.print("Humedad suelo: ");
  Serial.print(suelo.getPorHum());
  Serial.println("%");

  Serial.print("Es de noche: ");
  Serial.println(luz.esNoche());

  Serial.println();

  delay(1000);
}