// Trillobit3s@gmail.com
// Controle Bluetooth ESP32 - Motor + Ventilador + Lâmpada
// App Inventor (BOTÃO e VOZ)
// ESP32 38 PINOS - DOIT ESP32 DEVKIT V1
// Motor 28BYJ-48 + ULN2003
// Relés ATIVOS EM LOW

/*
COMANDOS RECEBIDOS PELO BLUETOOTH (APP / VOZ)

“ligar o motor”         -> M
“desligar o motor”      -> m

“ligar o ventilador”    -> V
“desligar o ventilador” -> v

“ligar a lâmpada”       -> L
“desligar a lâmpada”    -> l

“ligar tudo”            -> F
“desligar tudo”         -> f
*/

// ---------------- PINOS USADOS ----------------
// Motor de passo (ULN2003)
// IN1 -> GPIO 16
// IN2 -> GPIO 18
// IN3 -> GPIO 17
// IN4 -> GPIO 19

// Relés (ativo LOW)
// FAN     -> GPIO 25
// LÂMPADA -> GPIO 26

// ---------------- PINOS LIVRES ----------------
// Gerais: 4, 5, 13, 14, 21, 22, 23, 27, 32, 33
// Botões (INPUT ONLY): 34, 35, 36, 39
// LEDs recomendados: 21, 22, 23, 27

#include <Stepper.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// ---------------- CONFIGURAÇÕES ----------------
#define PASSOS_POR_VOLTA 2048

// Relés
#define RELE_FAN     25
#define RELE_LAMPADA 26

// Motor de passo (ordem correta ULN2003)
Stepper motor(PASSOS_POR_VOLTA, 16, 18, 17, 19);

// Estados
bool motorLigado   = false;
bool fanLigado     = false;
bool lampadaLigada = false;

// ------------------------------------------------
void setup() {

  pinMode(RELE_FAN, OUTPUT);
  pinMode(RELE_LAMPADA, OUTPUT);

  // Tudo desligado (relé ativo LOW)
  digitalWrite(RELE_FAN, HIGH);
  digitalWrite(RELE_LAMPADA, HIGH);

  motor.setSpeed(10); // RPM seguro

  Serial.begin(115200);
  SerialBT.begin("ESP32_Control");

  Serial.println("Bluetooth pronto");
  Serial.println("A Lamp ON | B Lamp OFF | C Fan ON | D Fan OFF");
  Serial.println("E Motor ON | F Motor OFF | G Tudo ON | H Tudo OFF");
}

// ------------------------------------------------
void loop() {

  // -------- BLUETOOTH --------
  if (SerialBT.available()) {

    char cmd = SerialBT.read();   // LÊ APENAS 1 CARACTERE

    Serial.print("Recebido: ");
    Serial.println(cmd);

    switch (cmd) {

      case 'N': // Lâmpada ON
        lampadaLigada = true;
        digitalWrite(RELE_LAMPADA, LOW);
        break;

      case 'O': // Lâmpada OFF
        lampadaLigada = false;
        digitalWrite(RELE_LAMPADA, HIGH);
        break;

      case 'Q': // Ventilador ON
        fanLigado = true;
        digitalWrite(RELE_FAN, LOW);
        break;

      case 'P': // Ventilador OFF
        fanLigado = false;
        digitalWrite(RELE_FAN, HIGH);
        break;

      case 'R': // Motor ON
        motorLigado = true;
        break;

      case 'S': // Motor OFF
        motorLigado = false;
        break;

      case 'G': // Tudo ON
        lampadaLigada = true;
        fanLigado = true;
        motorLigado = true;
        digitalWrite(RELE_LAMPADA, LOW);
        digitalWrite(RELE_FAN, LOW);
        break;

      case 'H': // Tudo OFF
        lampadaLigada = false;
        fanLigado = false;
        motorLigado = false;
        digitalWrite(RELE_LAMPADA, HIGH);
        digitalWrite(RELE_FAN, HIGH);
        break;
    }
  }

  // -------- MOTOR --------
  if (motorLigado) {
    motor.step(1);   // gira sem travar Bluetooth
  }
}

