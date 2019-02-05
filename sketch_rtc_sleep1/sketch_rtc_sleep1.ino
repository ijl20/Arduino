#include "Arduino.h"

#include <RTCZero.h>

#define DEBUG_BAUD 57600

/* Create an rtc object */
RTCZero rtc;

/* Change these values to set the current initial time */
const byte hours = 17;
const byte minutes = 0;
const byte seconds = 0;

/* Change these values to set the current initial date */
const byte year = 15;
const byte month = 6;
const byte day = 15;

volatile int alarm_count = 0;

void GLOW_RED() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

void GLOW_GREEN() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);
}

void GLOW_BLUE() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
}

void GLOW_OFF() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

void setup() {

  // DELAY STARTUP UNTIL READY
  while ((!SerialUSB) && (millis() < 10000)){
    // Wait 10 seconds for debugSerial to open
  }

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  GLOW_RED();
  delay(1000);

  SerialUSB.begin(DEBUG_BAUD);

  SerialUSB.println("RTC Sleep Program starting");

  delay(1000);
  
  GLOW_OFF();
  
  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(17, 00, 10);
  rtc.enableAlarm(rtc.MATCH_SS);

  rtc.attachInterrupt(alarmMatch);

  //rtc.standbyMode();
}

void loop()
{
  //SerialUSB.println("loop");
  
  GLOW_GREEN();
  
  delay(1000);

  //print2digits(alarm_count);

  delay(1000);
  
  GLOW_OFF();
  
  //delay(1000);
  rtc.standbyMode();    // Sleep until next alarm match
  alarm_count++;
  //SerialUSB.print("Alarm ");
  //SerialUSB.println(alarm_count++);
  
  //rtc.disableAlarm();
  //rtc.detachInterrupt();
  
  //delay(1000);
  
  //GLOW_BLUE();

  //delay(1000);

  //GLOW_GREEN();

  //delay(1000);
  
  //GLOW_OFF();
}

void alarmMatch()
{
}

void print2digits(int number) {
  if (number < 10) {
    SerialUSB.print("0"); // print a 0 before if the number is < than 10
  }
  SerialUSB.print(number);
}

