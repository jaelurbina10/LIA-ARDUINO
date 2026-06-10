/*
  ====================================================================
  PROYECTO INTEGRADOR - INVERNADERO INTELIGENTE LIA
  ====================================================================

  LIBRERIAS UTILIZADAS:
    - lia.h           -> Sensores, actuadores y controladores del invernadero
    - SR_Keypad.h     -> Lectura de teclado matricial 8x8 por registros de
                         desplazamiento (74HC595 / 74HC165)
    - LiquidCrystal_I2C -> Pantalla LCD 16x2 con modulo I2C

  DESCRIPCION GENERAL:
    El sistema controla automaticamente:
      - Riego (bomba de agua)       -> ControllerWatering
      - Techo automatico (servo)    -> ControllerRoof
      - Lampara UV anti-hongos      -> controllerFungi

    El usuario navega por una interfaz tipo menu en el LCD usando 3
    botones del teclado matricial:
      - Tecla '1' -> Siguiente dato / avanzar
      - Tecla '2' -> Entrar a Sensores (desde HOME) / Diagnostico (desde Sensores)
      - Tecla '3' -> Entrar a Actuadores (desde HOME)
      - Tecla '*' -> Entrar a Configuracion (desde HOME)
      - Tecla '#' -> Volver a HOME

  ====================================================================
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <lia.h>
#include <SR_Keypad.h>

using namespace LIA;

// ====================================================================
// 1. CONFIGURACION DE PINES
// ====================================================================

// ---- Sensores LIA ----
#define PIN_SUELO     A0   // hw080  - humedad de suelo
#define PIN_LLUVIA    2    // yl83   - sensor de lluvia
#define PIN_DHT11     4    // dht11  - temperatura y humedad ambiente
#define PIN_LDR       A1   // ldr    - sensor de luz (dia/noche)

// ---- Actuadores LIA ----
#define PIN_BOMBA     8    // relay  - bomba de riego
#define PIN_UV        7    // relay  - lampara UV anti-hongos
#define PIN_SERVO     9    // ServoActuador - techo automatico

// ---- Teclado matricial SR_Keypad (registros de desplazamiento) ----
// Salida (SIPO - control de filas)
#define SIPO_DATA_PIN   10
#define SIPO_CLOCK_PIN  11
#define SIPO_LATCH_PIN  12
// Entrada (PISO - lectura de columnas)
#define PISO_DATA_PIN   A2
#define PISO_CLOCK_PIN  A3
#define PISO_SL_PIN     A4

// ---- Direccion del LCD I2C (la mas comun es 0x27, algunos modulos usan 0x3F) ----
#define LCD_I2C_ADDR  0x27

// ====================================================================
// 2. OBJETOS GLOBALES - LIBRERIA LIA
// ====================================================================

// --- Sensores ---
hw080 sensorSuelo(PIN_SUELO);
yl83  sensorLluvia(PIN_LLUVIA);
dht11 sensorClima(PIN_DHT11);
ldr   sensorLuz(PIN_LDR);

// --- Actuadores ---
relay bombaRiego(PIN_BOMBA);
relay lampUV(PIN_UV);

// ServoActuador(pin, anguloCerrado, anguloAbierto)
ServoActuador techo(PIN_SERVO, 0, 90);

// --- Controladores ---
ControllerWatering ctrlRiego;
ControllerRoof      ctrlTecho;
controllerFungi     ctrlHongos;

// ====================================================================
// 3. OBJETOS GLOBALES - TECLADO MATRICIAL (SR_Keypad)
// ====================================================================

// --- Funciones de control para el registro de salida (SIPO) ---
void sipoData(bool valor) {
  digitalWrite(SIPO_DATA_PIN, valor);
}
void sipoClock() {
  digitalWrite(SIPO_CLOCK_PIN, HIGH);
  digitalWrite(SIPO_CLOCK_PIN, LOW);
}
void sipoLatch() {
  digitalWrite(SIPO_LATCH_PIN, HIGH);
  digitalWrite(SIPO_LATCH_PIN, LOW);
}

// --- Funciones de control para el registro de entrada (PISO) ---
bool pisoData() {
  return digitalRead(PISO_DATA_PIN);
}
void pisoClock() {
  digitalWrite(PISO_CLOCK_PIN, HIGH);
  digitalWrite(PISO_CLOCK_PIN, LOW);
}
void pisoSL() {
  digitalWrite(PISO_SL_PIN, HIGH);
  digitalWrite(PISO_SL_PIN, LOW);
}

// --- Objetos del teclado ---
SR_Keypad::SIPO registroSalida(sipoData, sipoClock, sipoLatch);
SR_Keypad::PISO registroEntrada(pisoData, pisoClock, pisoSL);

// El callback se declara mas abajo (necesita conocer la FSM)
void manejadorTeclado(uint8_t id, bool estado);

SR_Keypad::Controller teclado(manejadorTeclado);

// ====================================================================
// 4. OBJETO GLOBAL - LCD I2C
// ====================================================================

LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);

// ====================================================================
// 5. MAQUINA DE ESTADOS (FSM)
// ====================================================================

enum EstadoMenu {
  ESTADO_BOOT,
  ESTADO_HOME,
  ESTADO_SENSORES,
  ESTADO_ACTUADORES,
  ESTADO_CONFIGURACION,
  ESTADO_DIAGNOSTICO
};

EstadoMenu estadoActual = ESTADO_BOOT;

// Sub-indices para navegar dentro de cada menu (con tecla '1')
uint8_t subIndiceSensor     = 0;  // 0..3 -> Suelo, Aire, Luz, Lluvia
uint8_t subIndiceActuador   = 0;  // 0..2 -> Bomba, Techo, UV
uint8_t subIndiceConfig     = 0;  // 0..3 -> HumMin, HumMax, TiempoRiego, TiempoUV

const uint8_t NUM_PANTALLAS_SENSOR    = 4;
const uint8_t NUM_PANTALLAS_ACTUADOR  = 3;
const uint8_t NUM_PANTALLAS_CONFIG    = 4;

// Bandera para forzar el redibujado del LCD cuando cambia el estado
bool pantallaDebeActualizarse = true;

// ====================================================================
// 6. TEMPORIZADORES (no bloqueantes con millis)
// ====================================================================

unsigned long tiempoBoot          = 0;
const unsigned long DURACION_BOOT = 2000;   // 2 segundos en pantalla BOOT

unsigned long ultimaActualizacionSensores = 0;
const unsigned long PERIODO_SENSORES = 1000; // refrescar sensores cada 1 s

unsigned long ultimaActualizacionLCD = 0;
const unsigned long PERIODO_LCD = 500;       // refrescar LCD cada 0.5 s (mientras no cambie de estado)

// ====================================================================
// 7. CALLBACK DEL TECLADO
// ====================================================================

// SR_Keypad::Controller llama a esta funcion cada vez que detecta
// un cambio (flanco) en alguno de los 64 botones posibles (ID 0-63).
// Solo nos interesan los flancos de "presionado" (estado == true)
// y solo de los IDs que mapeamos como teclas logicas.
void manejadorTeclado(uint8_t id, bool estado) {

  // Solo procesar cuando el boton se PRESIONA (flanco de subida)
  if (!estado) {
    return;
  }

  switch (id) {

    case 0:  // Tecla logica '1' -> Avanzar / Siguiente dato
      switch (estadoActual) {
        case ESTADO_SENSORES:
          subIndiceSensor = (subIndiceSensor + 1) % NUM_PANTALLAS_SENSOR;
          pantallaDebeActualizarse = true;
          break;
        case ESTADO_ACTUADORES:
          subIndiceActuador = (subIndiceActuador + 1) % NUM_PANTALLAS_ACTUADOR;
          pantallaDebeActualizarse = true;
          break;
        case ESTADO_CONFIGURACION:
          subIndiceConfig = (subIndiceConfig + 1) % NUM_PANTALLAS_CONFIG;
          pantallaDebeActualizarse = true;
          break;
        default:
          break;
      }
      break;

    case 1:  // Tecla logica '2' -> Sensores (desde HOME) / Diagnostico (desde Sensores)
      if (estadoActual == ESTADO_HOME) {
        estadoActual = ESTADO_SENSORES;
        subIndiceSensor = 0;
        pantallaDebeActualizarse = true;
      } else if (estadoActual == ESTADO_SENSORES) {
        estadoActual = ESTADO_DIAGNOSTICO;
        pantallaDebeActualizarse = true;
      }
      break;

    case 2:  // Tecla logica '3' -> Actuadores (desde HOME)
      if (estadoActual == ESTADO_HOME) {
        estadoActual = ESTADO_ACTUADORES;
        subIndiceActuador = 0;
        pantallaDebeActualizarse = true;
      }
      break;

    case 8:  // Tecla logica '#' -> Volver a HOME
      if (estadoActual != ESTADO_HOME && estadoActual != ESTADO_BOOT) {
        estadoActual = ESTADO_HOME;
        pantallaDebeActualizarse = true;
      }
      break;

    case 9:  // Tecla logica '*' -> Configuracion (desde HOME)
      if (estadoActual == ESTADO_HOME) {
        estadoActual = ESTADO_CONFIGURACION;
        subIndiceConfig = 0;
        pantallaDebeActualizarse = true;
      }
      break;

    default:
      // Resto de los 64 IDs disponibles: reservados para
      // futuras ampliaciones (atajos, control manual, etc.)
      break;
  }
}

void setup() {

  Serial.begin(9600);

  // ----- Inicializacion del LCD -----
  lcd.init();
  lcd.backlight();

  // ----- Inicializacion de pines del teclado matricial -----
  pinMode(SIPO_DATA_PIN, OUTPUT);
  pinMode(SIPO_CLOCK_PIN, OUTPUT);
  pinMode(SIPO_LATCH_PIN, OUTPUT);

  pinMode(PISO_DATA_PIN, INPUT);
  pinMode(PISO_CLOCK_PIN, OUTPUT);
  pinMode(PISO_SL_PIN, OUTPUT);

  teclado.AttachSIPO(registroSalida);
  teclado.AttachPISO(registroEntrada);

  // ----- Inicializacion de sensores -----
  sensorSuelo.begin();
  sensorLluvia.begin();
  sensorClima.begin();
  sensorLuz.begin();

  // ----- Inicializacion de actuadores -----
  bombaRiego.begin();
  lampUV.begin();
  techo.begin();

  // ----- Configuracion de parametros del invernadero -----
  // Humedad minima del suelo para activar riego (%)
  sensorSuelo.setMinHum(35);
  // Humedad maxima del suelo (usada para techo y hongos) (%)
  sensorSuelo.setMaxHum(80);

  // Umbrales de temperatura/humedad ambiente (DHT11)
  // setThresholds(tempAlta, tempBaja, humAlta, humBaja)
  sensorClima.setThresholds(30, 15, 80, 40);

  // ----- Vincular sensores y actuadores a los controladores -----

  // Controlador de riego
  ctrlRiego.linkRainSensor(sensorLluvia);
  ctrlRiego.linkSoilHumiditySensor(sensorSuelo);
  ctrlRiego.linkWaterPump(bombaRiego);
  ctrlRiego.setWateringTime(15000); // 15 segundos maximo de riego

  // Controlador de techo
  ctrlTecho.linkRainSensor(sensorLluvia);
  ctrlTecho.linkSoilHumiditySensor(sensorSuelo);
  ctrlTecho.linkTempSensor(sensorClima);
  ctrlTecho.linkServo(techo);

  // Controlador de hongos (lampara UV)
  ctrlHongos.linkLDRSensor(sensorLuz);
  ctrlHongos.linkAirHumSensor(sensorClima);
  ctrlHongos.linkSoilHumiditySensor(sensorSuelo);
  ctrlHongos.linkUvLight(lampUV);
  ctrlHongos.setUvOnTimeMs(1000); // 1 segundo de ciclo UV

  // ----- Pantalla de arranque -----
  tiempoBoot = millis();
  pantallaDebeActualizarse = true;

  Serial.println(F("Sistema LIA inicializado correctamente."));
}

// ====================================================================
// 9. LOOP PRINCIPAL
// ====================================================================

void loop() {

  // 1. Escanear el teclado (puede disparar manejadorTeclado)
  teclado.Scan();

  // 2. Maquina de estados: transicion automatica BOOT -> HOME
  if (estadoActual == ESTADO_BOOT) {
    if (millis() - tiempoBoot >= DURACION_BOOT) {
      estadoActual = ESTADO_HOME;
      pantallaDebeActualizarse = true;
    }
  }

  // 3. Actualizar lecturas de sensores y logica de control (no bloqueante)
  if (millis() - ultimaActualizacionSensores >= PERIODO_SENSORES) {
    ultimaActualizacionSensores = millis();
    actualizarSensores();
    actualizarControladores();

    // En HOME y SENSORES queremos refrescar los valores constantemente
    if (estadoActual == ESTADO_HOME || estadoActual == ESTADO_SENSORES ||
        estadoActual == ESTADO_ACTUADORES || estadoActual == ESTADO_DIAGNOSTICO) {
      pantallaDebeActualizarse = true;
    }
  }

  // 4. Refrescar LCD solo cuando es necesario
  if (pantallaDebeActualizarse) {
    mostrarPantalla();
    pantallaDebeActualizarse = false;
  }
}

// ====================================================================
// 10. ACTUALIZACION DE SENSORES Y CONTROLADORES
// ====================================================================

void actualizarSensores() {
  sensorSuelo.process();
  sensorLluvia.process();
  sensorClima.process();
  sensorLuz.process();
}

void actualizarControladores() {
  ctrlRiego.process();
  ctrlTecho.process();
  ctrlHongos.process();
}

// ====================================================================
// 11. FUNCIONES DE PANTALLA (LCD 16x2)
// ====================================================================

// Despacha la pantalla correspondiente segun el estado actual de la FSM
void mostrarPantalla() {
  lcd.clear();

  switch (estadoActual) {
    case ESTADO_BOOT:
      pantallaBoot();
      break;
    case ESTADO_HOME:
      pantallaHome();
      break;
    case ESTADO_SENSORES:
      pantallaSensores();
      break;
    case ESTADO_ACTUADORES:
      pantallaActuadores();
      break;
    case ESTADO_CONFIGURACION:
      pantallaConfiguracion();
      break;
    case ESTADO_DIAGNOSTICO:
      pantallaDiagnostico();
      break;
  }
}

// --- BOOT ---
void pantallaBoot() {
  lcd.setCursor(0, 0);
  lcd.print(F("LIA INVERNADERO"));
  lcd.setCursor(0, 1);
  lcd.print(F("v1.0  Iniciando.."));
}

// --- HOME: resumen ejecutivo ---
void pantallaHome() {
  // Linea 1: Temperatura, humedad ambiente y humedad de suelo
  lcd.setCursor(0, 0);
  lcd.print(F("T:"));
  lcd.print(sensorClima.getTemperatura());
  lcd.print(F("C H:"));
  lcd.print(sensorClima.getHumedad());
  lcd.print(F("% S:"));
  lcd.print((int)sensorSuelo.getPorHum());
  lcd.print(F("%"));

  // Linea 2: accesos rapidos al menu
  lcd.setCursor(0, 1);
  lcd.print(F("2Sen 3Act *Cfg"));
}

// --- SENSORES: 4 sub-pantallas ---
void pantallaSensores() {
  switch (subIndiceSensor) {

    case 0: // Suelo
      lcd.setCursor(0, 0);
      lcd.print(F("SUELO: "));
      lcd.print((int)sensorSuelo.getPorHum());
      lcd.print(F("% Hum"));
      lcd.setCursor(0, 1);
      lcd.print(F("Min:"));
      lcd.print(sensorSuelo.getMinHum());
      lcd.print(F("% Max:"));
      lcd.print(sensorSuelo.getMaxHum());
      lcd.print(F("%"));
      break;

    case 1: // Aire (DHT11)
      lcd.setCursor(0, 0);
      lcd.print(F("AIRE: "));
      lcd.print(sensorClima.getTemperatura());
      lcd.print(F("C "));
      lcd.print(sensorClima.getHumedad());
      lcd.print(F("%RH"));
      lcd.setCursor(0, 1);
      lcd.print(F("Calor:"));
      lcd.print(sensorClima.haceCalor() ? F("S") : F("N"));
      lcd.print(F(" Frio:"));
      lcd.print(sensorClima.haceFrio() ? F("S") : F("N"));
      break;

    case 2: // Luz / LDR
      lcd.setCursor(0, 0);
      lcd.print(F("LUZ: Raw="));
      lcd.print(sensorLuz.getRawLux());
      lcd.setCursor(0, 1);
      lcd.print(F("Es de noche: "));
      lcd.print(sensorLuz.esNoche() ? F("SI") : F("NO"));
      break;

    case 3: // Lluvia
      lcd.setCursor(0, 0);
      lcd.print(F("LLUVIA: "));
      lcd.print(sensorLluvia.estaLloviendo() ? F("SI") : F("NO"));
      lcd.setCursor(0, 1);
      lcd.print(F("1=Sig  #=Volver"));
      break;
  }
}

// --- ACTUADORES: 3 sub-pantallas ---
void pantallaActuadores() {
  switch (subIndiceActuador) {

    case 0: // Bomba de riego
      lcd.setCursor(0, 0);
      lcd.print(F("BOMBA RIEGO:"));
      lcd.setCursor(0, 1);
      lcd.print(F("Estado: "));
      lcd.print(bombaRiego.estado() ? F("ON") : F("OFF"));
      break;

    case 1: // Techo (servo)
      lcd.setCursor(0, 0);
      lcd.print(F("TECHO (SERVO):"));
      lcd.setCursor(0, 1);
      lcd.print(F("Estado: "));
      lcd.print(techo.estado() ? F("ABIERTO") : F("CERRADO"));
      break;

    case 2: // Lampara UV
      lcd.setCursor(0, 0);
      lcd.print(F("LUZ UV (HONGOS):"));
      lcd.setCursor(0, 1);
      lcd.print(F("Estado: "));
      lcd.print(lampUV.estado() ? F("ON") : F("OFF"));
      break;
  }
}

// --- CONFIGURACION: 4 sub-pantallas (parametros realmente disponibles) ---
void pantallaConfiguracion() {
  switch (subIndiceConfig) {

    case 0:
      lcd.setCursor(0, 0);
      lcd.print(F("Hum.Min Riego:"));
      lcd.print(sensorSuelo.getMinHum());
      lcd.print(F("%"));
      lcd.setCursor(0, 1);
      lcd.print(F("1=Sig  #=Volver"));
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print(F("Hum.Max Techo:"));
      lcd.print(sensorSuelo.getMaxHum());
      lcd.print(F("%"));
      lcd.setCursor(0, 1);
      lcd.print(F("1=Sig  #=Volver"));
      break;

    case 2:
      lcd.setCursor(0, 0);
      lcd.print(F("Temp.Alta DHT:"));
      lcd.print(sensorClima.getMaxTemp());
      lcd.print(F("C"));
      lcd.setCursor(0, 1);
      lcd.print(F("1=Sig  #=Volver"));
      break;

    case 3:
      lcd.setCursor(0, 0);
      lcd.print(F("Hum.Alta DHT:"));
      lcd.print(sensorClima.getMaxhumidity());
      lcd.print(F("%"));
      lcd.setCursor(0, 1);
      lcd.print(F("#=Volver"));
      break;
  }
}

// --- DIAGNOSTICO: estado general del sistema ---
void pantallaDiagnostico() {
  lcd.setCursor(0, 0);
  lcd.print(F("DIAG: B"));
  lcd.print(bombaRiego.estado() ? F("1") : F("0"));
  lcd.print(F(" T"));
  lcd.print(techo.estado() ? F("1") : F("0"));
  lcd.print(F(" U"));
  lcd.print(lampUV.estado() ? F("1") : F("0"));
  lcd.print(F(" L:"));
  lcd.print(sensorLluvia.estaLloviendo() ? F("SI") : F("NO"));

  lcd.setCursor(0, 1);
  lcd.print(F("#=Volver a Home"));
}
