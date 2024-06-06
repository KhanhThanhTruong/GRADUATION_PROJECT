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
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <NEO6M.h>
#include <SHT3x.h>
#include <PMS7003.h>


PMS7003 pms(14,12);

void setup()
{
  Serial.begin(9600);
  pms.init();
  pms.setMode(MODE_NAME::PASSIVE_MODE);
}

void loop()
{

  Serial.print("PM2.5: ");
  Serial.println(pms.getData(DATA_TARGET::PM2_5_PMS));
  Serial.print("PM10: ");
  Serial.println(pms.getData(DATA_TARGET::PM10_PMS));
  // pms.getData(DATA_TARGET::PM2_5_PMS);


  delay(5000);

}
