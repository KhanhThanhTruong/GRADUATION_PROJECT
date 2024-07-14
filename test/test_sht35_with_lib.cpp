/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit. Pin numbers reflect the default
  SPI pins for Uno and Nano models:
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (For For Uno, Nano: pin 10. For MKR Zero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified  24 July 2020
  by Tom Igoe

  This example code is in the public domain.

*/

#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <random>
#include <FS.h>
#include "SPIFFS.h" // ESP32 only
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>
#include <random>
#include <Update.h>

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <Wire.h>
#include <SD.h>

#include "FS.h"

#include <SPI.h>

#include "SPI.h"
// #include <Adafruit_ILI9341.h>
// #include <display.h>
#include <SHT3x.h>
#include <PMS7003.h>
#include <SD_mybuit.h>
#include <display_tft_espi.h>
#include <MQ7.h>
#include <NEO6M.h>
#include <SOUNDSENSOR.h>
#include <SIMCOM7680C.h>
#include <TinyGPSPlus.h>
#include <uRTCLib.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <NEO6M.h>
#include <SHT3x.h>

SHT3x sht(SHT3x_I2C_ADDRESS_DEFAULT);
// NEO6M neo6m(14,12);
void setup()
{
  Serial.begin(9600);
  //Begin I2C communication
  sht.init();
  sht.setMode(MODE_SHT_SINGLESHOT);
}

void loop()
{

  sht.readBytes();
  sht.decode();

  Serial.print("TEMPERATURE: ");
  Serial.println(sht.get(DATA_SHT3x_TARGET::TEMPERATURE_SHT));
  Serial.print("HUMIDITY: ");
  Serial.println(sht.get(DATA_SHT3x_TARGET::HUMIDITY_SHT));
  // sht.get(DATA_SHT3x_TARGET::TEMPERATURE_SHT);
  // sht.get(DATA_SHT3x_TARGET::HUMIDITY_SHT);
  


  delay(5000);

}
