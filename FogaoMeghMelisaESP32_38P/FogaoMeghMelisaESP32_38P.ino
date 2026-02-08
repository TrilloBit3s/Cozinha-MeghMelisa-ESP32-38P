// Trillobit3s@gmail.com
// Projeto: Controle de motor de passo + FAN + LÂMPADA com Bluetooth
// Motor: 28BYJ-48 + ULN2003
// ESP32 38 PINOS - DOIT ESP32 DEVKIT V1

#include <Stepper.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// ---------------- CONFIGURAÇÕES ----------------
#define PASSOS_POR_VOLTA 2048

// Botões
#define BOTAO_LIGA 32
#define BOTAO_PARA 33

// Relés
#define RELE_FAN     25
#define RELE_LAMPADA 26

// Motor (ordem correta ULN2003)
Stepper MotorStepper(PASSOS_POR_VOLTA, 16, 18, 17, 19);

// Estados
bool motorLigado   = false;
bool fanLigada     = false;
bool lampadaLigada = false;

// ------------------------------------------------
void setup() {
  pinMode(BOTAO_LIGA, INPUT_PULLUP);
  pinMode(BOTAO_PARA, INPUT_PULLUP);

  pinMode(RELE_FAN, OUTPUT);
  pinMode(RELE_LAMPADA, OUTPUT);

  digitalWrite(RELE_FAN, LOW);
  digitalWrite(RELE_LAMPADA, LOW);

  MotorStepper.setSpeed(10);   // RPM seguro

  Serial.begin(115200);
  SerialBT.begin("ESP32_Motor");

  Serial.println("Bluetooth pronto");
  Serial.println("L=Motor ON | D=Motor OFF | V=FAN | A=LAMPADA");
}

void loop() {

  // -------- BOTÕES --------
  if (digitalRead(BOTAO_LIGA) == LOW) {
    motorLigado = true;
    Serial.println("Motor LIGADO (botão)");
    delay(200);
  }

  if (digitalRead(BOTAO_PARA) == LOW) {
    motorLigado = false;
    Serial.println("Motor DESLIGADO (botão)");
    delay(200);
  }

  // -------- BLUETOOTH --------
  if (SerialBT.available()) {
    char comando = SerialBT.read();

    switch (comando) {

      case 'L':
      case 'l':
        motorLigado = true;
        SerialBT.println("Motor LIGADO");
        break;

      case 'D':
      case 'd':
        motorLigado = false;
        SerialBT.println("Motor DESLIGADO");
        break;

      case 'V':
      case 'v':
        fanLigada = !fanLigada;
        digitalWrite(RELE_FAN, fanLigada ? HIGH : LOW);
        SerialBT.println(fanLigada ? "FAN LIGADA" : "FAN DESLIGADA");
        break;

      case 'A':
      case 'a':
        lampadaLigada = !lampadaLigada;
        digitalWrite(RELE_LAMPADA, lampadaLigada ? HIGH : LOW);
        SerialBT.println(lampadaLigada ? "LAMPADA LIGADA" : "LAMPADA DESLIGADA");
        break;
    }
  }

  // -------- MOTOR --------
  if (motorLigado) {
    MotorStepper.step(1);
  }
}
