#include <Stepper.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// ---------------- CONFIGURAÇÕES ----------------
#define PASSOS_POR_VOLTA 2048

// Relés EXISTENTES (ATIVO EM LOW)
#define RELE_FAN     25
#define RELE_LAMPADA 26

// >>> FOGOS (LED DIRETO – ATIVO EM HIGH) <<<
#define RELE_FOGO1 21
#define RELE_FOGO2 22
#define RELE_FOGO3 23
#define RELE_FOGO4 27

// Motor de passo (ULN2003)
Stepper motor(PASSOS_POR_VOLTA, 16, 18, 17, 19);

// Estados EXISTENTES
bool motorLigado   = false;
bool fanLigado     = false;
bool lampadaLigada = false;

// ------------------------------------------------
void setup() {
  pinMode(RELE_FAN, OUTPUT);
  pinMode(RELE_LAMPADA, OUTPUT);

  // >>> PINOS DOS FOGOS <<<
  pinMode(RELE_FOGO1, OUTPUT);
  pinMode(RELE_FOGO2, OUTPUT);
  pinMode(RELE_FOGO3, OUTPUT);
  pinMode(RELE_FOGO4, OUTPUT);

  // RELÉS iniciam desligados (HIGH = OFF)
  digitalWrite(RELE_FAN, HIGH);
  digitalWrite(RELE_LAMPADA, HIGH);

  // FOGOS iniciam desligados (LOW = OFF | LED direto)
  digitalWrite(RELE_FOGO1, LOW);
  digitalWrite(RELE_FOGO2, LOW);
  digitalWrite(RELE_FOGO3, LOW);
  digitalWrite(RELE_FOGO4, LOW);

  motor.setSpeed(10);

  Serial.begin(115200);
  SerialBT.begin("ESP32_Control");
  Serial.println("Bluetooth pronto");
}

// ------------------------------------------------
void loop() {

  // -------- BLUETOOTH --------
  if (SerialBT.available() > 0) {

    char cmd = SerialBT.read();

    // >>> COMPORTAMENTO IGUAL AO ARDUINO ANTIGO <<<
    // ignora lixo enviado pelo App (voz)
    if (cmd == '\n' || cmd == '\r' || cmd == ' ') {
      return;
    }

    Serial.print("Recebido: ");
    Serial.println(cmd);

    switch (cmd) {

      // ===== LÂMPADA (RELÉ) =====
      case 'A':
      case 'I':
        digitalWrite(RELE_LAMPADA, HIGH); // OFF
        break;

      case 'a':
      case 'i':
        digitalWrite(RELE_LAMPADA, LOW);  // ON
        break;

      // ===== VENTILADOR (RELÉ) =====
      case 'B':
      case 'J':
        digitalWrite(RELE_FAN, HIGH); // OFF
        break;

      case 'b':
      case 'j':
        digitalWrite(RELE_FAN, LOW);  // ON
        break;

      // ===== MICRO-ONDAS (MOTOR) =====
      case 'C':
      case 'K':
        motorLigado = false;
        break;

      case 'c':
      case 'k':
        motorLigado = true;
        break;

      // ===== TODOS EQUIPAMENTOS =====
      case 'D':
        digitalWrite(RELE_LAMPADA, HIGH);
        digitalWrite(RELE_FAN, HIGH);
        motorLigado = false;
        digitalWrite(RELE_FOGO1, LOW);
        digitalWrite(RELE_FOGO2, LOW);
        digitalWrite(RELE_FOGO3, LOW);
        digitalWrite(RELE_FOGO4, LOW);
        break;

      case 'd':
        digitalWrite(RELE_LAMPADA, LOW);
        digitalWrite(RELE_FAN, LOW);
        motorLigado = true;
        digitalWrite(RELE_FOGO1, HIGH);
        digitalWrite(RELE_FOGO2, HIGH);
        digitalWrite(RELE_FOGO3, HIGH);
        digitalWrite(RELE_FOGO4, HIGH);
        break;

      // ===== FOGÃO – FOGO 1 =====
      case 'E':
      case 'L':
        digitalWrite(RELE_FOGO1, LOW);
        break;

      case 'e':
      case 'l':
        digitalWrite(RELE_FOGO1, HIGH);
        break;

      // ===== FOGÃO – FOGO 2 =====
      case 'F':
      case 'M':
        digitalWrite(RELE_FOGO2, LOW);
        break;

      case 'f':
      case 'm':
        digitalWrite(RELE_FOGO2, HIGH);
        break;

      // ===== FOGÃO – FOGO 3 =====
      case 'G':
      case 'N':
        digitalWrite(RELE_FOGO3, LOW);
        break;

      case 'g':
      case 'n':
        digitalWrite(RELE_FOGO3, HIGH);
        break;

      // ===== FOGÃO – FOGO 4 =====
      case 'H':
      case 'O':
        digitalWrite(RELE_FOGO4, LOW);
        break;

      case 'h':
      case 'o':
        digitalWrite(RELE_FOGO4, HIGH);
        break;
    }
  }

  // -------- MOTOR --------
  if (motorLigado) {
    motor.step(1);
  }
}
