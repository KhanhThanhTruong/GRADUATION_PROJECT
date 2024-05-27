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
#include <SD_mybuit.h>



void setup() {
  Serial.begin(9600);
  SD.begin(5);
}

void loop() {
  // nothing happens after setup
  StaticJsonDocument<200> js;

  js["test"] = "test";
  js["arg"] = 1;
  pushData("/test.txt", js);

  while(true);
  // File fl;
  // fl=SD.open("/test.txt");
  // if(!fl){
  //   Serial.println("Error opening file");
  //   return;
  // }else
  // {
  //   Serial.println("File opened");
  //   while(fl.available()){
  //     Serial.write(fl.read());
  //   }
  //   fl.close();
  // }
  

}
