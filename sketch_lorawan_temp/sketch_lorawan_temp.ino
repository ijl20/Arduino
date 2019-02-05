#include <Sodaq_RN2483.h>

#define debugSerial SerialUSB
#define loraSerial Serial2

bool OTAA = true;

//  ## ABP ACTIVATION ############################################################################
//  ## ABP ACTIVATION ############################################################################
//  ## ABP ACTIVATION ############################################################################
// USE YOUR OWN KEYS!
const uint8_t devAddr[4] =
{
    0x00, 0x00, 0x00, 0x00
};

// USE YOUR OWN KEYS!
const uint8_t appSKey[16] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// USE YOUR OWN KEYS!
const uint8_t nwkSKey[16] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//  ## OTAA ACTIVATION ###########################################################################
//  ## OTAA ACTIVATION ###########################################################################
//  ## OTAA ACTIVATION ###########################################################################
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

void setup()
{
  while ((!debugSerial) && (millis() < 10000)){
    // Wait 10 seconds for debugSerial to open
  }
  
  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  
  setupLoRa();
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
    debugSerial.println("Communication to LoRaBEE successful.");
  }
  else
  {
    debugSerial.println("Communication to LoRaBEE failed!");
  }
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
   String reading = getTemperature();

    switch (LoRaBee.send(1, (uint8_t*)reading.c_str(), reading.length()))
    {
    case NoError:
      debugSerial.println("Successful transmission.");
      break;
    case NoResponse:
      debugSerial.println("There was no response from the device.");
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
    // Delay between readings
    // 60 000 = 1 minute
    delay(10000); 
}

String getTemperature()
{
  //10mV per C, 0C is 500mV
  float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.0 / 1023.0;
  float temp = (mVolts - 500.0) / 10.0;
  
  return String(temp);
}
