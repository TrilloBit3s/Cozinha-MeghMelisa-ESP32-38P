// Projeto: Controle de motor de passo com botões e Bluetooth (ESP32)

#include <Stepper.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// ---------------- CONFIGURAÇÕES ----------------
int PassosPorVolta = 100;

int Pino_botaoLiga = 6;     // Botão LIGAR
int Pino_botaoPara = 7;     // Botão PARAR

Stepper MotorStepper(PassosPorVolta, 8, 10, 9, 11);

// Estado do motor
bool motorLigado = false;

// ------------------------------------------------
void setup() {
    pinMode(Pino_botaoLiga, INPUT_PULLUP);
    pinMode(Pino_botaoPara, INPUT_PULLUP);

    MotorStepper.setSpeed(300);

    Serial.begin(115200);
    SerialBT.begin("ESP32_Motor");  // Nome do Bluetooth

    Serial.println("Bluetooth pronto. Envie L ou D");
}

void loop() {

    // -------- BOTÕES FÍSICOS --------
    if (digitalRead(Pino_botaoLiga) == LOW) {
        motorLigado = true;
        Serial.println("Motor LIGADO (botão)");
        delay(200); // debounce simples
    }

    if (digitalRead(Pino_botaoPara) == LOW) {
        motorLigado = false;
        Serial.println("Motor DESLIGADO (botão)");
        delay(200); // debounce simples
    }

    // -------- BLUETOOTH --------
    if (SerialBT.available()) {
        char comando = SerialBT.read();

        if (comando == 'L' || comando == 'l') {
            motorLigado = true;
            Serial.println("Motor LIGADO (Bluetooth)");
            SerialBT.println("Motor LIGADO");
        }

        if (comando == 'D' || comando == 'd') {
            motorLigado = false;
            Serial.println("Motor DESLIGADO (Bluetooth)");
            SerialBT.println("Motor DESLIGADO");
        }
    }

    // -------- MOTOR --------
    if (motorLigado) {
        MotorStepper.step(1);   // gira continuamente
    }
}
