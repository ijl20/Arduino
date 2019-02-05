#include "Arduino.h"

#define CONSOLE_STREAM SerialUSB
#define LORA_STREAM Serial2

#define LORA_BAUD 57600
#define DEBUG_BAUD 57600

#define TEST_PIN 3

#define LORA_RESET_PIN 4

void setup() {
  // put your setup code here, to run once:
  //Setup streams
  bool console_ready = false;
  while (millis() < 10000)
  {
    // Wait 10 seconds for CONSOLE_STREAM to open
    if (CONSOLE_STREAM)
    {
      console_ready = true;
      break;
    }
  }
  if (console_ready)
  {
    CONSOLE_STREAM.println("Console started ok");
  }

  // SET UP PIN MODES
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LORA_RESET_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);  
  pinMode(LED_BLUE, OUTPUT);
  pinMode(TEST_PIN, OUTPUT);

  digitalWrite(TEST_PIN, HIGH);
  
  digitalWrite(LED_BUILTIN,HIGH);
  delay(3000);
  digitalWrite(LED_BUILTIN,LOW);
  delay(3000);
  
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
  delay(1000);
  
  CONSOLE_STREAM.begin(DEBUG_BAUD);
  LORA_STREAM.begin(LORA_BAUD);

  CONSOLE_STREAM.println("RN_reset starting");
  CONSOLE_STREAM.print("LED_GREEN=");
  CONSOLE_STREAM.println(LED_GREEN);

  // RESET LORA_MODULE
  
  CONSOLE_STREAM.print("Resetting LoRa module via pin ");
  CONSOLE_STREAM.println(LORA_RESET_PIN);
    
//  pinMode(LORA_RESET, OUTPUT);
//  digitalWrite(LORA_RESET, LOW);
//  delay(100);
//  digitalWrite(LORA_RESET, HIGH);
  digitalWrite(LORA_RESET_PIN, LOW);
  delay(400);
  digitalWrite(LORA_RESET_PIN, HIGH);
  delay(1000);
  CONSOLE_STREAM.println("LoRa reset complete");
  
}

void loop() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);

  digitalWrite(TEST_PIN, LOW);
  // put your main code here, to run repeatedly:
  LORA_STREAM.end();
  LORA_STREAM.begin(57600);

  CONSOLE_STREAM.print("Testing LoRa module, sending \"sys get ver\", expecting \"RN2xxx\", received: \"");
  delay(100);
  LORA_STREAM.println("sys reset");
  delay(200);
  LORA_STREAM.println("sys get ver");
  delay(200);

  char buff[7];
  memset(buff, 0, sizeof(buff));
  
  LORA_STREAM.readBytesUntil(0x20, buff, sizeof(buff));
  CONSOLE_STREAM.print(buff);
  CONSOLE_STREAM.print("\"...");

  CONSOLE_STREAM.println();

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  
  digitalWrite(TEST_PIN, HIGH);
  
  delay(2000);
}

