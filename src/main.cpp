#include <TFT_eSPI.h> // Thư viện màn hình
#include <SPI.h>
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
#include <TFT_eSPI.h> // Thư viện màn hình
#include <display_tft_espi.h>

display_tft_espi disp;
// TFT_eSPI tft = TFT_eSPI();
void setup()
{
    Serial.begin(9600);
    // tft.init();
    // tft.setRotation(2);
    // tft.setCursor(120,120);
    disp.begin();
    disp.background();

}

void loop()
{
    disp.process_touch();
}