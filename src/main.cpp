
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
#include <Lte_SIM7600.h>

Sim7600G lte;
MqttClient mqtt;
String topic="v1/devices/me/telemetry";
int i=0;
void setup()
{
  lte.begin(115200);
  // lte.restart();
  // delay(15000);

  // lte.restart();
  // const char *payload="{\"key\":15}";
  // if(!mqtt.connected())
  // mqtt.connect("esp32","Thanh","123456","demo.thingsboard.io",1883);
  // delay(500);
  // lte.callnumber("0379745618");
  // delay(10000);
  lte.sendSMS("0379745618","good luck to you");
}
void loop()
{
  // i++;
  // String payload="{\"key\":"+String(i)+"}";
  // mqtt.publish(topic.c_str(),payload.c_str(),topic.length(),payload.length(),1);
  // delay(5000);
}
