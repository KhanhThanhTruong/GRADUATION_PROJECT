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
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <NEO6M.h>

NEO6M neo6m(14,27);
void setup()
{
  Serial.begin(9600);
  // Serial2.begin(9600, SERIAL_8N1, GPIO_NUM_14, GPIO_NUM_12);
  neo6m.init();
  // Serial.println(neo6m.getGLL());
}

void loop()
{

unsigned long startTime = millis();
  Serial.println(neo6m.getGLL2());
  Serial.println(millis()-startTime);
  Serial.print("LATTITUDE: ");
  Serial.println(neo6m.get(LATTITUDE_GPS));
  Serial.print("LONGITUDE: ");
  Serial.println(neo6m.get(LONGITUDE_GPS));
  Serial.print("TIME: ");
  Serial.println(neo6m.get(TIME_GPS_NEO));


  delay(2000);

}
