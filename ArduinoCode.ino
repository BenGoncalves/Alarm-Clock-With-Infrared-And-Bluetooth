#include "dht.h"
#include "LedControl.h"
#include "Time.h"
#include "IRremote.h"

#define BUZZER 6
#define dht_dpin A0
#define IRED 11
#define BUTTON1 5
#define BUTTON2 10
#define DISP 11
#define UM 16738455
#define DOIS 16750695
#define TRES 16756815
#define QUATRO 16724175
#define CINCO 16718055
#define SEIS 16743045
#define SETE 16716015
#define OITO 16726215
#define NOVE 16734885
#define ZERO 16730805
#define BOT_OK 16712445
#define BOT_MENU 16761405
#define BOT_VOLTAR 16720605

LedControl lc = LedControl(DISP, DISP + 2, DISP + 1, 1);
dht DHT;

unsigned long delaytime = 250;
int RECV_PIN = IRED;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long timerhum = 0;
int U, u, T, t;
int hora, minuto, segundo;
int valor = 99;
int numero = 0;
int numeroAlarm = 0;
int numeroAlarm2 = 0;
int H, h, M, m, S, s;
unsigned long int timerLCD;
unsigned int alarmH, alarmM;
unsigned int alarmh, alarmm;
boolean alarmONOFF;
unsigned int alarmH2, alarmM2;
unsigned int alarmh2, alarmm2;
boolean alarmONOFF2;

unsigned int alarmHora, alarmMinuto;
unsigned int alarmHora2, alarmMinuto2;
boolean som = 0;
unsigned long int alarmtimer2, alarm2;
unsigned long int alarmtimer, alarm;
int menuop = 0;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  delay(300);//Let system settle
  Serial.println("Humidity and temperature\n\n");
  delay(700);

  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP); 
  pinMode(BUTTON2, INPUT_PULLUP); 
  pinMode(DISP, OUTPUT);
  pinMode(DISP + 1, OUTPUT);
  pinMode(DISP + 2, OUTPUT);

  digitalWrite(BUZZER, HIGH);
  setTime(21, 20, 0, 11, 3, 2015);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
}

void loop() {
  unsigned int horalocal = 0;
  results.value = 0;
  if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC);
    irrecv.resume(); // Receive the next value
  }

  switch (results.value) {
    case BOT_MENU:
      menuop = menuop + 1;
      break;
    case BOT_VOLTAR:
      if (menuop == 0) menuop = 5;
      menuop = menuop - 1;
      break;
    case UM:
      valor = 1;
      break;
    case DOIS:
      valor = 2;
      break;
    case TRES:
      valor = 3;
      break;
    case QUATRO:
      valor = 4;
      break;
    case CINCO:
      valor = 5;
      break;
    case SEIS:
      valor = 6;
      break;
    case SETE:
      valor = 7;
      break;
    case OITO:
      valor = 8;
      break;
    case NOVE:
      valor = 9;
      break;
    case ZERO:
      valor = 0;
      break;
    case BOT_OK:
      valor = 15;
      break;
  }

  display();
  
  horalocal = hour();
  Serial.print(horalocal);
  Serial.print("  -  ");
  Serial.println(minute());

  if ((alarmHora == hour()) && (alarmMinuto == minute()) && (second() <= 5) && (alarmONOFF)) tocaralarme();
  if ((alarmHora2 == hour()) && (alarmMinuto2 == minute()) && (second() <= 5) && (alarmONOFF2)) tocaralarme();


  if (som == 1) {
    if (millis() - timerLCD <= 5000) menuop = 1;
    else if (millis() - timerLCD <= 7500) menuop = 2;
    else {
      menuop = 0;
      som = 0;
      lc.clearDisplay(0);
    }
  }

  if (millis() - timerhum > 1000) {
    DHT.read11(dht_dpin);
    U = DHT.humidity / 10;
    u = DHT.humidity - 10 * U;
    T = DHT.temperature / 10;
    t = DHT.temperature - 10 * T;

    timerhum = millis();
  }
}


void display() {
  int hora, minuto, segundo;
  int hh, HH, mm, MM, ss, SS;

  if (menuop == 1) {
    if (valor < 10) {
      switch (numero) {
        case 0:
          HH = hour() / 10;
          hh = hour() - 10 * HH;
          setTime(10 * valor + hh, minute(), second(), day(), month(), year());
          numero++;
          valor = 99;
          break;
        case 1:
          HH = hour() / 10;
          setTime(10 * HH + valor, minute(), second(), day(), month(), year());
          numero++;
          valor = 99;
          break;
        case 2:
          MM = minute() / 10;
          mm = minute() - 10 * MM;
          setTime(hour(), 10 * valor + mm, second(), day(), month(), year());
          numero++;
          valor = 99;
          break;
        case 3:
          MM = minute() / 10;
          setTime(hour(), 10 * MM + valor, second(), day(), month(), year());
          numero++;
          valor = 99;
          break;
        case 4:
          SS = second() / 10;
          ss = second() - 10 * SS;
          setTime(hour(), minute(), 10 * valor + ss, day(), month(), year());
          numero++;
          valor = 99;
          break;
        case 5:
          SS = second() / 10;
          setTime(10 * valor + hh, minute(), 10 * SS + valor, day(), month(), year());
          numero = 0;
          valor = 99;
          break;
      }
    }
    lc.clearDisplay(0);
    H = hour() / 10;
    h = hour() - 10 * H;
    M = minute() / 10;
    m = minute() - 10 * M;
    S = second() / 10;
    s = second() - 10 * S;
    lc.setDigit(0, 7, H, false);
    lc.setDigit(0, 6, h, false);
    lc.setRow(0, 5, B10000000);
    lc.setDigit(0, 4, M, false);
    lc.setDigit(0, 3, m, false);
    lc.setRow(0, 2, B10000000);
    lc.setDigit(0, 1, S, false);
    lc.setDigit(0, 0, s, false);
    delay(500);
  } else {
    if (menuop == 2) {
      lc.clearDisplay(0);
      lc.setDigit(0, 2, T, false);
      lc.setDigit(0, 1, t, false);
      lc.setRow(0, 0, B01100011);
      lc.setDigit(0, 6, U, false);
      lc.setDigit(0, 5, u, false);
      lc.setRow(0, 4, B00011100);
      delay(500);
    } else {
      if (menuop == 3) {
        if (valor < 10) {
          switch (numeroAlarm) {
            case 0:
              if (valor > 2) valor = 2;
              alarmH = valor;
              numeroAlarm++;
              valor = 99;
              alarmHora = alarmh + 10 * alarmH;
              alarmMinuto = alarmm + 10 * alarmM;
              break;
            case 1:
              if (alarmH >= 2) {
                if (valor > 3) valor = 3;
              }
              alarmh = valor;
              numeroAlarm++;
              valor = 99;
              alarmHora = alarmh + 10 * alarmH;
              alarmMinuto = alarmm + 10 * alarmM;
              break;
            case 2:
              if (valor > 5) valor = 5;
              alarmM = valor;
              numeroAlarm++;
              valor = 99;
              alarmHora = alarmh + 10 * alarmH;
              alarmMinuto = alarmm + 10 * alarmM;
              break;
            case 3:
              alarmm = valor;
              numeroAlarm = 0;
              valor = 99;
              alarmHora = alarmh + 10 * alarmH;
              alarmMinuto = alarmm + 10 * alarmM;
              break;
          }
        }
        else {
          if (valor == 15) {

            alarmONOFF = !alarmONOFF;
            alarmtimer = millis();
            while (millis() - alarmtimer < 250) {
              digitalWrite(BUZZER, LOW);
            }
            while (millis() - alarmtimer < 500) {
              digitalWrite(BUZZER, HIGH);
            }
            while (millis() - alarmtimer < 750) {
              digitalWrite(BUZZER, LOW);
            }
            digitalWrite(BUZZER, HIGH);
            valor = 99;
          }
        }
        lc.clearDisplay(0);
        lc.setChar(0, 7, 'A', false);
        lc.setDigit(0, 6, 1, false);
        lc.setChar(0, 5, '-', false);
        if (alarmONOFF) {
          lc.setDigit(0, 4, alarmH, false);
          lc.setDigit(0, 3, alarmh, false);
          lc.setRow(0, 2, B10000000);
          lc.setDigit(0, 1, alarmM, false);
          lc.setDigit(0, 0, alarmm, false);
        } else {
          lc.setChar(0, 2, '0', false);
          lc.setChar(0, 1, 'F', false);
          lc.setChar(0, 0, 'F', false);
        }
        delay(500);
      }
    }
  }
  if (menuop == 4) {
    if (valor < 10) {
      switch (numeroAlarm2) {
        case 0:
          if (valor > 2) valor = 2;
          alarmH2 = valor;
          numeroAlarm2++;
          valor = 99;
          alarmHora2 = alarmh2 + 10 * alarmH2;
          alarmMinuto2 = alarmm2 + 10 * alarmM2;
          break;
        case 1:
          if (alarmH2 >= 2) {
            if (valor > 3) valor = 3;
          }
          alarmh2 = valor;
          numeroAlarm2++;
          valor = 99;
          alarmHora2 = alarmh2 + 10 * alarmH2;
          alarmMinuto2 = alarmm2 + 10 * alarmM2;
          break;
        case 2:
          if (valor > 5) valor = 5;
          alarmM2 = valor;
          numeroAlarm2++;
          valor = 99;
          alarmHora2 = alarmh2 + 10 * alarmH2;
          alarmMinuto2 = alarmm2 + 10 * alarmM2;
          break;
        case 3:
          alarmm2 = valor;
          numeroAlarm2 = 0;
          valor = 99;
          alarmHora2 = alarmh2 + 10 * alarmH2;
          alarmMinuto2 = alarmm2 + 10 * alarmM2;
          break;
      }
    }
    else {
      if (valor == 15) {

        alarmONOFF2 = !alarmONOFF2;
        alarmtimer2 = millis();
        while (millis() - alarmtimer2 < 250) {
          digitalWrite(BUZZER, LOW);
        }
        while (millis() - alarmtimer2 < 500) {
          digitalWrite(BUZZER, HIGH);
        }
        while (millis() - alarmtimer2 < 750) {
          digitalWrite(BUZZER, LOW);
        }
        digitalWrite(BUZZER, HIGH);
        valor = 99;
      }
    }
    lc.clearDisplay(0);
    lc.setChar(0, 7, 'A', false);
    lc.setDigit(0, 6, 2, false);
    lc.setChar(0, 5, '-', false);
    if (alarmONOFF2) {
      lc.setDigit(0, 4, alarmH2, false);
      lc.setDigit(0, 3, alarmh2, false);
      lc.setRow(0, 2, B10000000);
      lc.setDigit(0, 1, alarmM2, false);
      lc.setDigit(0, 0, alarmm2, false);
    } else {
      lc.setChar(0, 2, '0', false);
      lc.setChar(0, 1, 'F', false);
      lc.setChar(0, 0, 'F', false);
    }
    delay(500);
  }
  if (menuop == 5) {
    menuop = 0;
    lc.clearDisplay(0);
    results.value = 0;
  }
}


void tocaralarme() {
  boolean tocando = HIGH;
  unsigned long int timer1, timer2;

  timer1 = millis();
  results.value = 0;
  while ((tocando) && (millis() - timer1 < 30000)) {
    if (irrecv.decode(&results)) {
      Serial.println(results.value, DEC);
      irrecv.resume();
    }

    if (results.value == BOT_OK) tocando = 0;

    timer2 = millis();
    while (millis() - timer2 < 250) {
      digitalWrite(BUZZER, LOW);
    }
    while (millis() - timer2 < 500) {
      digitalWrite(BUZZER, HIGH);
    }

    lc.clearDisplay(0);
    lc.setChar(0, 7, 'A', false);
    lc.setChar(0, 6, 'c', false);
    lc.setChar(0, 5, 'o', false);
    lc.setRow(0, 4, B00000101);
    lc.setChar(0, 3, 'd', false);
    lc.setChar(0, 2, 'A', false);
  }
  digitalWrite(BUZZER, HIGH);
  delay (5000);
}
