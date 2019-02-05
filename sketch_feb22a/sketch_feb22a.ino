/*
  Simple RTC for Arduino Zero and MKR1000

  Demonstrates the use of the RTC library for the Arduino Zero and MKR1000

  This example code is in the public domain

  http://arduino.cc/en/Tutorial/SimpleRTC

  created by Arturo Guadalupi <a.guadalupi@arduino.cc>
  15 Jun 2015
  modified 
  18 Feb 2016
  modified by Andrea Richetta <a.richetta@arduino.cc>
  24 Aug 2016
*/
#include "Arduino.h"

#include <RTCZero.h>

#define DEBUG_BAUD 57600

/* Create an rtc object */
RTCZero rtc;

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 16;

/* Change these values to set the current initial date */
const byte day = 15;
const byte month = 6;
const byte year = 15;

void RED() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

void GREEN() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);
}

void BLUE() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
}

void setup() {

  // DELAY STARTUP UNTIL READY
  while ((!SerialUSB) && (millis() < 10000)){
    // Wait 10 seconds for debugSerial to open
  }

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  RED();
  delay(1000);
  
  SerialUSB.begin(DEBUG_BAUD);

  rtc.begin(); // initialize RTC

  // Set the time
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);

  // Set the date
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

  // you can use also
  //rtc.setTime(hours, minutes, seconds);
  //rtc.setDate(day, month, year);
}

void loop()
{
  GREEN();
  // Print date...
  print2digits(rtc.getDay());
  SerialUSB.print("/");
  print2digits(rtc.getMonth());
  SerialUSB.print("/");
  print2digits(rtc.getYear());
  SerialUSB.print(" ");

  // ...and time
  print2digits(rtc.getHours());
  SerialUSB.print(":");
  print2digits(rtc.getMinutes());
  SerialUSB.print(":");
  print2digits(rtc.getSeconds());

  SerialUSB.println();

  delay(1000);

  BLUE();
  delay(1000);
}



void print2digits(int number) {
  if (number < 10) {
    SerialUSB.print("0"); // print a 0 before if the number is < than 10
  }
  SerialUSB.print(number);
}

