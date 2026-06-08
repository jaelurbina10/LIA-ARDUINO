/*
  ============================================================
  EJEMPLO 03 - CULTIVO DE HONGOS
  ============================================================

  Este ejemplo utiliza:

  - LDR para detectar día/noche
  - DHT11 para humedad ambiental
  - HW080 para humedad del sustrato
  - Relé para una lámpara UV

  El controladorFungi activará la luz UV
  según las condiciones configuradas.
*/

#include <lia.h>

using namespace LIA;

// LDR conectado al pin analógico A1
ldr luz(A1);

// DHT11 conectado al pin digital 4
dht11 aire(4);

// Humedad del sustrato
hw080 suelo(A0);

// Relé que controla la lámpara UV
relay uv(7);

controllerFungi fungi;

void setup()
{
  Serial.begin(9600);

  /*
    setThresholds(
      temperaturaAlta,
      temperaturaBaja,
      humedadAlta,
      humedadBaja
    );
  */
  aire.setThresholds(
    28,
    15,
    85,
    50
  );

  // Humedad máxima del sustrato
  suelo.setMaxHum(75);

  luz.begin();
  aire.begin();
  suelo.begin();

  uv.begin();

  fungi.linkLDRSensor(luz);
  fungi.linkAirHumSensor(aire);
  fungi.linkSoilHumiditySensor(suelo);
  fungi.linkUvLight(uv);

  // Tiempo de encendido UV
  // 1000 ms = 1 segundo
  fungi.setUvOnTimeMs(1000);
}

void loop()
{
  luz.process();
  aire.process();
  suelo.process();

  fungi.process();

  Serial.print("Es de noche: ");
  Serial.print(luz.esNoche());

  Serial.print(" | Humedad aire: ");
  Serial.print(aire.getHumedad());

  Serial.print("% | Humedad suelo: ");
  Serial.print(suelo.getPorHum());

  Serial.println("%");

  delay(500);
}