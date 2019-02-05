/*
 * This is an update from the Sodaq microchip-serial-passthrough example
 * https://github.com/SodaqMoja/Sodaq_RN2483/tree/master/examples/microchip-serial-passthrough
 * Their original copywrite notice below.
 * 
 * This update is specifically aimed at the Sodaq Explorer evaluation board, which is
 * 'Arduino M0 compatible' i.e. SAMD 32-bit Arm, with the serial passthru going to the RN_2483 Lorawan chip.
 * 
 * The RN_2483 command reference is here:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/40001784E.pdf
 * 
 * Update by Ian Lewis ijl20@cam.ac.uk
 */
/*
* Copyright (c) 2015 SODAQ. All rights reserved.
*
* This file is part of Sodaq_RN2483.
*
* Sodaq_RN2483 is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation, either version 3 of
* the License, or(at your option) any later version.
*
* Sodaq_RN2483 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with Sodaq_RN2483.  If not, see
* <http://www.gnu.org/licenses/>.
*/

#include <Sodaq_RN2483.h>

// Autonomo
#define debugSerial SerialUSB // debug console via USB
#define loraSerial Serial2    // RN_2483 Lorwan module
#define btSerial Serial1      // Bluetooth
// this leaves Serial = D1/TX, D0/RX

#define DEBUG_BAUD 57600

// The SoC pin connected to the RN2483 reset pin (active low)
// Sodaq Explorer Rev 3 only !!
#define LORA_RESET_PIN 4
// Note for Rev 5 and above you use the constant LORA_RESET for this value...
// #define LORA_RESET_PIN LORA_RESET

// buffer to hold string entered by user e.g. "sys reset"
String user_command = "";

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

void setup()
{
    // DELAY STARTUP UNTIL READY
  while ((!SerialUSB) && (millis() < 10000)){
    // Wait 10 seconds for debugSerial to open
  }
  
  debugSerial.begin(DEBUG_BAUD);

  if (millis()<10000)
  {
    debugSerial.print("Board started, SerialUSB up in ");
    debugSerial.print(millis());
    debugSerial.println(" milliseconds");
  }
  else
  {
    debugSerial.println("Board started but SerialUSB not ready");
  }

  debugSerial.println("Try commands 'lora reset', 'connect abp', 'send foo'");
  
  // SET PIN MODES
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LORA_RESET_PIN, OUTPUT);
  
  LED_SET_BLUE();
  delay(1000);
  LED_SET_OFF();
  
}

void loop()
{
  while (debugSerial.available()) 
  {
    // read a char from the USB console
    char c = debugSerial.read();
    // add that char to the command buffer
    user_command += c;
    // See if the last char we've received is a CR, if so process complete command
    if (c==0x0A)
    {
      handle_command();
      user_command = "";
    }
  }

  while (loraSerial.available()) 
  {
    debugSerial.write((char)loraSerial.read());
  }
}

void handle_command()
{
      debugSerial.print("Command ");
      debugSerial.print(user_command);
      debugSerial.println(" accepted");

      if (user_command.startsWith("connect abp"))
      {
          // connect to lorawan network using the ABP protocol
          lora_connect_ABP();
      }
      else if (user_command.startsWith("send "))
      {
          // send the user string via the LoraWAN network
          lora_send();
      }
      else if (user_command.startsWith("lora reset"))
      {
          // toggle the RN_2483 reset pin low
          lora_reset();
      }
      else
      {
          // we didn't recognise the command, so send it to the RN_2483 and hope it makes sense there...
          loraSerial.print(user_command);
      }
}
  
void lora_connect_ABP()
{
  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, false))
  {
    debugSerial.println("ABP connection to the LoraWAN network was successful.");
  }
  else
  {
    debugSerial.println("ABP Connection to the LoraWAN network failed!");
  }
}

void lora_reset()
{
  debugSerial.println("Resetting Lora Module...");
  
  // RESET LORA_MODULE
  digitalWrite(LORA_RESET_PIN, LOW);
  delay(100);
  digitalWrite(LORA_RESET_PIN, HIGH);
  delay(1000);

  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  
  debugSerial.println("Reset Lora Module completed");
}  
  
void lora_send()
{
    String send_str = "foo";
    
    switch (LoRaBee.send(1, (uint8_t*)send_str.c_str(), send_str.length()))
    {
    case NoError:
      debugSerial.println("Successful transmission.");
      break;
    case NoResponse:
      debugSerial.println("There was no response from the device.");
      break;
    case Timeout:
      debugSerial.println("Connection timed-out. Check your serial connection to the device!");
      break;
    case PayloadSizeError:
      debugSerial.println("The size of the payload is greater than allowed. Transmission failed!");
      break;
    case InternalError:
      debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! Reset the RN module.");
      break;
    case Busy:
      debugSerial.println("The device is busy.");
      break;
    case NetworkFatalError:
      debugSerial.println("There is a non-recoverable error with the network connection");
      break;
    case NotConnected:
      debugSerial.println("The device is not connected to the network.");
      break;
    case NoAcknowledgment:
      debugSerial.println("There was no acknowledgment sent back!");
      break;
    default:
      break;
    }
}



