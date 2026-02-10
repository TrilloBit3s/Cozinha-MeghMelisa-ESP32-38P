// Trillobit3s@gmail.com
// Controle Bluetooth ESP32 - Motor + Ventilador + Lâmpada
// Cozinha IOT Megh Melisa
// ESP32 38 PINOS - DOIT ESP32 DEVKIT V1
// Motor 28BYJ-48 + ULN2003
// 10/02/2026

// -------- APENAS VOZ ------------------
// 1  desligar a lâmpada       = b
// 2  ligar a lâmpada          = a
// 3  desligar o ventilador    = d
// 4  ligar o ventilador       = c
// 5  desligar os equipamentos = e
// 6  ligar os equipamentos    = f
// 7  desligar fogo 1          = g
// 8  ligar fogo 1             = h
// 9  desligar fogo 2          = i
// 10 ligar fogo 2             = j
// 11 desligar fogo 3          = k
// 12 ligar fogo 3             = l
// 13 desligar fogo 4          = m
// 14 ligar fogo 4             = n
// 15 ligar o micro-ondas      = 3
// 16 desligar o micro-ondas   = 2

// -------- APENAS BOTÕES ----------------
// 15 botão lâmpada desligada        = o
// 16 botão lâmpada ligada           = p
// 17 botão ventilador desligado     = r
// 18 botão ventilador ligado        = q
// 19 botão micro-ondas desligado    = s
// 20 botão fogo 1 desligado         = u
// 21 botão fogo 1 ligado            = v
// 22 botão fogo 2 desligado         = w
// 23 botão fogo 2 ligado            = x
// 24 botão fogo 3 desligado         = y
// 25 botão fogo 3 ligado            = z
// 26 botão fogo 4 desligado         = 0
// 27 botão fogo 4 ligado            = 1

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

  // LEDs do fogo iniciam DESLIGADOS
  digitalWrite(RELE_FOGO1, LOW);
  digitalWrite(RELE_FOGO2, LOW);
  digitalWrite(RELE_FOGO3, LOW);
  digitalWrite(RELE_FOGO4, LOW);

  digitalWrite(RELE_FAN, HIGH);
  digitalWrite(RELE_LAMPADA, HIGH);
  
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

    // ===== BOTÕES =====
    if (cmd == 0 && recebido.length() == 1) {
      cmd = recebido.charAt(0);
    }

    if (cmd != 0) {
      Serial.print("Comando final: ");
      Serial.println(cmd);

    // ===== EXECUÇÃO =====
    switch (cmd) {

        //lampada
        case 'a': case 'p': digitalWrite(RELE_LAMPADA, LOW);  break; // ok
        case 'b': case 'o': digitalWrite(RELE_LAMPADA, HIGH); break; // ok
        //ventilador
        case 'c': case 'r': digitalWrite(RELE_FAN, LOW);  break; // ok
        case 'd': case 'q': digitalWrite(RELE_FAN, HIGH); break; // ok
        //micro-ondas
        case '3': case 't': motorLigado = true;  break; //ok
        case '2': case 's': motorLigado = false; break; //ok

      case 'e': // desligar os equipamentos
        digitalWrite(RELE_LAMPADA, HIGH);   // relé
        digitalWrite(RELE_FAN, HIGH);       // relé
        motorLigado = false;
        digitalWrite(RELE_FOGO1, LOW);      // LED
        digitalWrite(RELE_FOGO2, LOW);
        digitalWrite(RELE_FOGO3, LOW);
        digitalWrite(RELE_FOGO4, LOW);
        break;

      case 'f': // ligar os equipamentos
        digitalWrite(RELE_LAMPADA, LOW);    // relé
        digitalWrite(RELE_FAN, LOW);        // relé
        motorLigado = true;
        digitalWrite(RELE_FOGO1, HIGH);     // LED
        digitalWrite(RELE_FOGO2, HIGH);
        digitalWrite(RELE_FOGO3, HIGH);
        digitalWrite(RELE_FOGO4, HIGH);
        break;

        // FOGO 1
        case 'h': case 'v': digitalWrite(RELE_FOGO1, HIGH);  break;   
        case 'g': case 'u': digitalWrite(RELE_FOGO1, LOW); break;  

        // FOGO 2
        case 'j': case 'x': digitalWrite(RELE_FOGO2, HIGH);  break;
        case 'i': case 'w': digitalWrite(RELE_FOGO2, LOW); break;

        // FOGO 3
        case 'l': case 'z': digitalWrite(RELE_FOGO3, HIGH);  break;
        case 'k': case 'y': digitalWrite(RELE_FOGO3, LOW); break;

        // FOGO 4
        case 'n': case '1': digitalWrite(RELE_FOGO4, HIGH);  break;
        case 'm': case '0': digitalWrite(RELE_FOGO4, LOW); break;
      }
    }
  }

  if (motorLigado) {
    motor.step(1);
  }
}