#include "Arduino.h"

#include <Sodaq_RN2483.h>

#include <RTCZero.h>

#define LORA_BAUD 57600
#define DEBUG_BAUD 57600

#define LORA_RESET_PIN 4 // Sodaq Explorer Rev 3 only !!

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

// ***********************************************************************************************
// ***********************************************************************************************
// LoraWAN config
// ***********************************************************************************************
// ***********************************************************************************************

#define loraSerial Serial2

bool OTAA = false; // Over-The-Air Activation

//  ## ABP ACTIVATION 
//  ## ABP ACTIVATION 
//  ## ABP ACTIVATION 
// USE YOUR OWN KEYS!
const uint8_t devAddr[4] =
{
    0x17, 0x85, 0x00, 0x13
};

// USE YOUR OWN KEYS!
const uint8_t appSKey[16] =
{
//  d7e769dbad847cfce331159a188f6636
  0xd7, 0xe7, 0x69, 0xdb, 0xad, 0x84, 0x7c, 0xfc, 0xe3, 0x31, 0x15, 0x9a, 0x18, 0x8f, 0x66, 0x36
};

// USE YOUR OWN KEYS!
const uint8_t nwkSKey[16] =
{
//  ddc2512407aeccd0d10d8b6592b0e194
  0xdd, 0xc2, 0x51, 0x24, 0x07, 0xae, 0xcc, 0xd0, 0xd1, 0x0d, 0x8b, 0x65, 0x92, 0xb0, 0xe1, 0x94
};

//  ## OTAA ACTIVATION 
//  ## OTAA ACTIVATION 
//  ## OTAA ACTIVATION 
// OTAA
const uint8_t DevEUI[8] =
{
  0x76, 0x39, 0x53, 0xfa, 0xb3, 0x42, 0xfb, 0xd8
};

const uint8_t AppEUI[16] =
{
   0x60, 0x1f, 0x7e, 0xf6, 0x9e, 0x86, 0x12, 0xa2 
};

const uint8_t AppKey[16] =
{
  0x33, 0x19, 0x72, 0x6c, 0x10, 0x0e, 0x49, 0xb6, 0xec, 0xd2, 0xee, 0xc5, 0x52, 0xf1, 0xf5, 0xd9
};



volatile int alarm_count = 0;

void LED_SET_RED() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

void LED_SET_GREEN() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);
}

void LED_SET_BLUE() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
}

void LED_SET_OFF() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);  
}

void setup() {

  // DELAY STARTUP UNTIL READY
  while ((!SerialUSB) && (millis() < 10000)){
    // Wait 10 seconds for debugSerial to open
  }

  // SET PIN MODES
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LORA_RESET_PIN, OUTPUT);
  
  SerialUSB.begin(DEBUG_BAUD);

  LED_SET_BLUE();
  delay(1000);
    
  SerialUSB.println("Resetting Lora Module...");
  
  // RESET LORA_MODULE
  digitalWrite(LORA_RESET_PIN, LOW);
  delay(100);
  digitalWrite(LORA_RESET_PIN, HIGH);
  delay(1000);
  
  SerialUSB.println("Reset Lora Module completed");
  
  loraSerial.begin(LORA_BAUD);
  
  setupLoRa();
  
  LED_SET_OFF();
  
  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(17, 00, 10);
  rtc.enableAlarm(rtc.MATCH_SS);

  rtc.attachInterrupt(alarmMatch);

}

void setupLoRa(){
  if(!OTAA){
    // ABP
    setupLoRaABP();
  } else {
    //OTAA
    setupLoRaOTAA();
  }
}

void setupLoRaABP(){  
  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, false))
  {
    SerialUSB.println("ABP Lora Initialization successful.");
  }
  else
  {
    SerialUSB.println("Communication to LoRaBEE failed!");
  }
}

void setupLoRaOTAA(){

  SerialUSB.println("Attempting LoraWAN OTAA Setup...");
  
  SerialUSB.println(LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, false));
  
  if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, false))
  {
    SerialUSB.println("Network connection successful.");
  }
  else
  {
    SerialUSB.println("Network connection failed!");
  }
}

void loop()
{
  for (int i=0; i<=alarm_count; i++)
  {
     LED_SET_GREEN();
     delay(100);
     LED_SET_OFF();
     delay(200);
  }
  
  rtc.standbyMode();    // Sleep until next alarm match
  alarm_count++;
}

void alarmMatch()
{
}


