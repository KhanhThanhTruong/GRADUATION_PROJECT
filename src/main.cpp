
#include <Wire.h>
#include <SPI.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <random>
#include <SoftwareSerial.h>

SoftwareSerial gps(14,12);


void setup()
{
  Serial.begin(9600);
  gps.begin(9600);
}

void loop()
{
  if(gps.available())
  {
    Serial.write(gps.read());
  }
}