#define DEBUG 1

#include <Sodaq_RN2483_internal.h>
#include <Utils.h>

#include <Sodaq_RN2483.h>

#define debugSerial SerialUSB
#define loraSerial Serial2

#define LORA_BAUD 57600
#define DEBUG_BAUD 57600


#define LORA_RESET_PIN 4 // Sodaq Explorer Rev 3 only !!

#include "Arduino.h"

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

bool OTAA = true;

bool blue_led_on = false;

//  ## OTAA ACTIVATION ###########################################################################
//  ## OTAA ACTIVATION ###########################################################################
//  ## OTAA ACTIVATION ###########################################################################
// OTAA
const uint8_t DevEUI[8] =
{
  0x76, 0x3a, 0x53, 0xfa, 0xb3, 0x42, 0xfb, 0xd8
};

const uint8_t AppEUI[16] =
{ // 70 B3 D5 7E D0 01 76 A7
   0x70, 0xb3, 0xd5, 0x7e, 0xd0, 0x00, 0xa7, 0xc8 
};

const uint8_t AppKey[16] =
{
  0x33, 0x19, 0x72, 0x6c, 0x10, 0x0e, 0x49, 0xb6, 0xec, 0xd2, 0xee, 0xc5, 0x52, 0xf1, 0xf5, 0xd9
};

void setup()
{
  // DELAY STARTUP UNTIL READY
  while ((!debugSerial) && (millis() < 10000)){
    // Wait 10 seconds for debugSerial to open
  }

  // SET PIN MODES
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LORA_RESET_PIN, OUTPUT);
  
  debugSerial.begin(DEBUG_BAUD);

  LED_SET_BLUE();
  delay(1000);
    
  debugSerial.println("Resetting Lora Module...");
  
  // RESET LORA_MODULE
  digitalWrite(LORA_RESET_PIN, LOW);
  delay(100);
  digitalWrite(LORA_RESET_PIN, HIGH);
  delay(1000);
  
  debugSerial.println("Reset Lora Module completed");
  
  loraSerial.begin(LORA_BAUD);
  
  setupLoRa();
  
  LED_SET_OFF();
}

void setupLoRa(){
    setupLoRaOTAA();
}


void setupLoRaOTAA(){

  debugSerial.println("Attempting LoraWAN OTAA Setup...");
  
  debugSerial.println(LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, false));
  
  if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, false))
  {
    debugSerial.println("Network connection successful.");
  }
  else
  {
    debugSerial.println("Network connection failed!");
  }
}

void loop()
{
   LED_SET_GREEN();
   
   String reading = getTemperature();

   debugSerial.print("Read temperature:");
   debugSerial.println(reading);
   
    switch (LoRaBee.send(1, (uint8_t*)reading.c_str(), reading.length()))
    {
    case NoError:
      debugSerial.println("Successful transmission.");
      break;
    case NoResponse:
      debugSerial.println("There was no response from the device.");
      setupLoRa();
      break;
    case Timeout:
      debugSerial.println("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("The size of the payload is greater than allowed. Transmission failed!");
      break;
    case InternalError:
      debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! The program will reset the RN module.");
      setupLoRa();
      break;
    case Busy:
      debugSerial.println("The device is busy. Sleeping for 10 extra seconds.");
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("There is a non-recoverable error with the network connection. The program will reset the RN module.");
      setupLoRa();
      break;
    case NotConnected:
      debugSerial.println("The device is not connected to the network. The program will reset the RN module.");
      setupLoRa();
      break;
    case NoAcknowledgment:
      debugSerial.println("There was no acknowledgment sent back!");
      break;
    default:
      break;
    }
    delay(1000);
    LED_SET_OFF();
    // Delay between readings
    // 60 000 = 1 minute
    delay(30000); 
}

String getTemperature()
{
  //10mV per C, 0C is 500mV
  float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.0 / 1023.0;
  float temp = (mVolts - 500.0) / 10.0;
  
  return String(temp);
}
