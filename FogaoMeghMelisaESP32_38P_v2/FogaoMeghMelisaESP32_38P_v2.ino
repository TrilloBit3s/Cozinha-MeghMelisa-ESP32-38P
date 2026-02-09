// Trillobit3s@gmail.com
// Controle Bluetooth ESP32 - Motor + Ventilador + Lâmpada
// ESP32 38 PINOS - DOIT ESP32 DEVKIT V1
// Motor 28BYJ-48 + ULN2003

#include <Stepper.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// ---------------- CONFIGURAÇÕES ----------------
#define PASSOS_POR_VOLTA 2048

// Relés
#define RELE_FAN     25
#define RELE_LAMPADA 26

#define RELE_FOGO1 21
#define RELE_FOGO2 22
#define RELE_FOGO3 23
#define RELE_FOGO4 27

// Motor
Stepper motor(PASSOS_POR_VOLTA, 16, 18, 17, 19);

bool motorLigado = false;

// ------------------------------------------------
void setup() {

  pinMode(RELE_FAN, OUTPUT);
  pinMode(RELE_LAMPADA, OUTPUT);

  pinMode(RELE_FOGO1, OUTPUT);
  pinMode(RELE_FOGO2, OUTPUT);
  pinMode(RELE_FOGO3, OUTPUT);
  pinMode(RELE_FOGO4, OUTPUT);

  digitalWrite(RELE_FAN, HIGH);
  digitalWrite(RELE_LAMPADA, HIGH);

  digitalWrite(RELE_FOGO1, HIGH);
  digitalWrite(RELE_FOGO2, HIGH);
  digitalWrite(RELE_FOGO3, HIGH);
  digitalWrite(RELE_FOGO4, HIGH);

  motor.setSpeed(10);

  Serial.begin(115200);
  SerialBT.begin("ESP32_Control");

  Serial.println("Bluetooth pronto");
}

// ------------------------------------------------
void loop() {

  if (SerialBT.available()) {

    String recebido = "";

    while (SerialBT.available()) {
      char c = SerialBT.read();
      recebido += c;
      delay(2);
    }

    recebido.trim();
    recebido.toLowerCase();

    Serial.print("Texto recebido: ");
    Serial.println(recebido);

    char cmd = 0;

    // // ===== VOZ =====
    // if (recebido == "ligar a lâmpada") cmd = 'a';
    // else if (recebido == "desligar a lâmpada") cmd = 'b';

    // else if (recebido == "ligar o ventilador") cmd = 'c';
    // else if (recebido == "desligar o ventilador") cmd = 'd';

    // else if (recebido == "ligar o micro-ondas") cmd = 'f';
    // else if (recebido == "desligar o micro-ondas") cmd = 'e';

    // else if (recebido == "ligar os equipamentos") cmd = 'h';
    // else if (recebido == "desligar os equipamentos") cmd = 'g';

    // else if (recebido == "ligar fogo 1") cmd = 'j';
    // else if (recebido == "desligar fogo 1") cmd = 'i';

    // else if (recebido == "ligar fogo 2") cmd = 'l';
    // else if (recebido == "desligar fogo 2") cmd = 'k';

    // else if (recebido == "ligar fogo 3") cmd = 'n';
    // else if (recebido == "desligar fogo 3") cmd = 'm';

    // else if (recebido == "ligar fogo 4") cmd = 'p';
    // else if (recebido == "desligar fogo 4") cmd = 'o';

    // ===== VOZ (ROBUSTA - SEM ACENTO) =====
    if (recebido.indexOf("ligar") >= 0 && recebido.indexOf("lampada") >= 0) cmd = 'a';
    else if (recebido.indexOf("desligar") >= 0 && recebido.indexOf("lampada") >= 0) cmd = 'b';

    else if (recebido.indexOf("ligar") >= 0 && recebido.indexOf("ventilador") >= 0) cmd = 'c';
    else if (recebido.indexOf("desligar") >= 0 && recebido.indexOf("ventilador") >= 0) cmd = 'd';

    else if (recebido.indexOf("ligar") >= 0 && recebido.indexOf("micro") >= 0) cmd = 'f';
    else if (recebido.indexOf("desligar") >= 0 && recebido.indexOf("micro") >= 0) cmd = 'e';

    else if (recebido.indexOf("ligar") >= 0 && recebido.indexOf("equipamentos") >= 0) cmd = 'h';
    else if (recebido.indexOf("desligar") >= 0 && recebido.indexOf("equipamentos") >= 0) cmd = 'g';

    else if (recebido.indexOf("ligar") >= 0 && recebido.indexOf("fogo 1") >= 0) cmd = 'j';
    else if (recebido.indexOf("desligar") >= 0 && recebido.indexOf("fogo 1") >= 0) cmd = 'i';

    else if (recebido.indexOf("ligar") >= 0 && recebido.indexOf("fogo 2") >= 0) cmd = 'l';
    else if (recebido.indexOf("desligar") >= 0 && recebido.indexOf("fogo 2") >= 0) cmd = 'k';

    else if (recebido.indexOf("ligar") >= 0 && recebido.indexOf("fogo 3") >= 0) cmd = 'n';
    else if (recebido.indexOf("desligar") >= 0 && recebido.indexOf("fogo 3") >= 0) cmd = 'm';

    else if (recebido.indexOf("ligar") >= 0 && recebido.indexOf("fogo 4") >= 0) cmd = 'p';
    else if (recebido.indexOf("desligar") >= 0 && recebido.indexOf("fogo 4") >= 0) cmd = 'o';

    // ===== BOTÕES =====
    if (cmd == 0 && recebido.length() == 1) {
      cmd = recebido.charAt(0);
    }

    if (cmd != 0) {
      Serial.print("Comando final: ");
      Serial.println(cmd);

    // ===== EXECUÇÃO =====
    switch (cmd) {

        case 'a': case 'r': digitalWrite(RELE_LAMPADA, LOW);  break;
        case 'b': case 'q': digitalWrite(RELE_LAMPADA, HIGH); break;

        case 'c': case 't': digitalWrite(RELE_FAN, LOW);  break;
        case 'd': case 's': digitalWrite(RELE_FAN, HIGH); break;

        case 'f': case 'v': motorLigado = true;  break;
        case 'e': case 'u': motorLigado = false; break;

        case 'h':
          digitalWrite(RELE_LAMPADA, LOW);
          digitalWrite(RELE_FAN, LOW);
          motorLigado = true;
          digitalWrite(RELE_FOGO1, LOW);
          digitalWrite(RELE_FOGO2, LOW);
          digitalWrite(RELE_FOGO3, LOW);
          digitalWrite(RELE_FOGO4, LOW);
          break;

        case 'g':
          digitalWrite(RELE_LAMPADA, HIGH);
          digitalWrite(RELE_FAN, HIGH);
          motorLigado = false;
          digitalWrite(RELE_FOGO1, HIGH);
          digitalWrite(RELE_FOGO2, HIGH);
          digitalWrite(RELE_FOGO3, HIGH);
          digitalWrite(RELE_FOGO4, HIGH);
          break;

        case 'j': case 'x': digitalWrite(RELE_FOGO1, LOW);  break;
        case 'i': case 'w': digitalWrite(RELE_FOGO1, HIGH); break;

        case 'l': case 'z': digitalWrite(RELE_FOGO2, LOW);  break;
        case 'k': case 'y': digitalWrite(RELE_FOGO2, HIGH); break;

        case 'n': case '1': digitalWrite(RELE_FOGO3, LOW);  break;
        case 'm': case '0': digitalWrite(RELE_FOGO3, HIGH); break;

        case 'p': case '3': digitalWrite(RELE_FOGO4, LOW);  break;
        case 'o': case '2': digitalWrite(RELE_FOGO4, HIGH); break;
      }
    }
  }

  if (motorLigado) {
    motor.step(1);
  }
}
