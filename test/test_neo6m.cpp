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

NEO6M neo6m(14,12);
void setup()
{
  Serial.begin(9600);
  // Serial2.begin(9600, SERIAL_8N1, GPIO_NUM_14, GPIO_NUM_12);
  neo6m.init();
  // Serial.println(neo6m.getGLL());
}

void loop()
{


  Serial.println(neo6m.getGLL());
  Serial.print("LATTITUDE: ");
  Serial.println(neo6m.get(LATTITUDE));
  Serial.print("LONGITUDE: ");
  Serial.println(neo6m.get(LONGITUDE));
  Serial.print("TIME: ");
  Serial.println(neo6m.get(TIME));


  delay(2000);

}
