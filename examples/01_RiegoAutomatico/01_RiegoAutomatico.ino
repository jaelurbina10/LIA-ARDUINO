/*
  ============================================================
  EJEMPLO 01 - RIEGO AUTOMÁTICO
  ============================================================

  Este ejemplo muestra cómo utilizar:

  - Sensor de humedad de suelo HW080
  - Sensor de lluvia YL83
  - Relé para controlar una bomba de agua
  - ControllerWatering

  El sistema regará automáticamente cuando:

  1. No esté lloviendo
  2. La humedad del suelo sea menor al mínimo configurado

  Hardware:

  HW080  -> A0
  YL83   -> D2
  Relé   -> D8
*/

#include <lia.h>

using namespace LIA;

// ------------------------------------------------------------
// Sensores
// ------------------------------------------------------------

// Sensor de humedad de suelo conectado al pin A0
hw080 suelo(A0);

// Sensor de lluvia conectado al pin digital 2
yl83 lluvia(2);

// ------------------------------------------------------------
// Actuadores
// ------------------------------------------------------------

// Relé que activará la bomba de agua
relay bomba(8);

// ------------------------------------------------------------
// Controlador
// ------------------------------------------------------------

ControllerWatering riego;

void setup()
{
  Serial.begin(9600);

  // Humedad mínima permitida (%)
  // Si el suelo baja de este valor, se activará el riego.
  suelo.setMinHum(35);

  // Inicialización de dispositivos
  suelo.begin();
  lluvia.begin();
  bomba.begin();

  // Asociar sensores y actuador al controlador
  riego.linkRainSensor(lluvia);
  riego.linkSoilHumiditySensor(suelo);
  riego.linkWaterPump(bomba);

  // Tiempo máximo de riego en milisegundos
  // 15000 ms = 15 segundos
  riego.setWateringTime(15000);
}

void loop()
{
  // Actualizar lecturas de sensores
  suelo.process();
  lluvia.process();

  // Ejecutar lógica de riego
  riego.process();

  // Mostrar datos por monitor serial
  Serial.print("Humedad del suelo: ");
  Serial.print(suelo.getPorHum());
  Serial.println("%");

  delay(500);
}