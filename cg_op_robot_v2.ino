#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int motorA_EN = 5;
const int motorA_IN1 = 6;
const int motorA_IN2 = 7;
const int motorB_EN = 8;
const int motorB_IN3 = 9;
const int motorB_IN4 = 10;

int first_mark = 0;
int second_mark;
int state = 0;     // faixa não iniciada
char command;      // a variable to read incoming serial data into


SoftwareSerial bluetooth(2, 3); // TX, RX (Bluetooth)
SoftwareSerial dfplayer_serial(4, 5);

DFRobotDFPlayerMini dfplayer;

void setup() {
  bluetooth.begin(9600);
  dfplayer_serial.begin(9600);
  
  if (!dfplayer.begin(dfplayer_serial)) {
     Serial.println("Erro ao iniciar DFPlayer");
     while (true);
  }

  dfplayer.volume(30);
  
  bluetooth.println("DFPlayer iniciado...");
  bluetooth.println("Conexao estabelecida...");
  bluetooth.println("Aguardando comandos...");

  pinMode(motorA_EN, OUTPUT);
  pinMode(motorB_EN, OUTPUT);
  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorB_IN3, OUTPUT);
  pinMode(motorB_IN4, OUTPUT);

  parar();
}

void loop() {
  second_mark = millis();
  if (second_mark - first_mark > 5000){
    parar();
  }
  
  if (bluetooth.available() > 0) {
    command = bluetooth.read();
    first_mark = millis();

    // motores
    if (command == 'F') {
      bluetooth.println("indo para frente");
      frente();
    }
 
    if (command == 'B') {
      bluetooth.println("indo para tras");
      tras();
    }

    if (command == 'L'){
      bluetooth.println("virando a esquerda");
      esquerda();
    }

    if (command == 'R'){
      bluetooth.println("virando a direita");
      direita();
    }

//    if (command == 'A'){
//      bluetooth.println("iniciando robo... vrummm...");
//      iniciar();
//    }

    if (command == 'P'){
      bluetooth.println("parar");
      parar();
    }

    // mp3
    if ((command == 'X') && (state == 0)){
      bluetooth.println("faixa 1 iniciada");
      dfplayer.play(1);
      state = 1;
    }

    if ((command == 'X') && (state == 1)){
      bluetooth.println("faixa pausada");
      dfplayer.pause();
      state = 2;
    }
    
    if ((command == 'X') && (state == 2)){
      bluetooth.println("faixa retomada");
      dfplayer.start();
      state = 1;
    }

    if (command == 'C'){
      bluetooth.println("próxima faixa");
      dfplayer.next();
    }

    if (command == 'S'){
      bluetooth.println("faixa anterior");
      dfplayer.previous();
    }
  }
}


void frente(){
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);
  analogWrite(motorA_EN, 255);
  analogWrite(motorB_EN, 255);
  }

void tras(){
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, HIGH);
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, HIGH);
  analogWrite(motorA_EN, 255);
  analogWrite(motorB_EN, 255);
  }

void esquerda(){
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, HIGH);
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);
  analogWrite(motorA_EN, 255);
  analogWrite(motorB_EN, 255);
  }

void direita(){
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, HIGH);
  analogWrite(motorA_EN, 255);
  analogWrite(motorB_EN, 255);
  }

void parar(){
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, LOW);
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, LOW);
  analogWrite(motorA_EN, 0);
  analogWrite(motorB_EN, 0);
  }
