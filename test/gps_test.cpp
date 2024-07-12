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
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Chân kết nối với module GPS
static const int RXPin = 27, TXPin = 14;
static const uint32_t GPSBaud = 9600;

// Đối tượng TinyGPSPlus
TinyGPSPlus gps;

// Kết nối serial với thiết bị GPS
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop() {
  // Đặt thời gian bắt đầu
  unsigned long startTime = millis();
  const unsigned long timeout = 500; // 1 giây

  // Đọc dữ liệu từ GPS và truyền vào đối tượng TinyGPSPlus với giới hạn thời gian
  while (millis() - startTime < timeout) {
    while (ss.available() > 0) {
      if (gps.encode(ss.read())) 
      {
        Serial.println(millis() - startTime);
        break;
      }
    }
  }

  // Nếu có dữ liệu GPS mới, hiển thị thông tin
  if (gps.location.isUpdated()) {
    Serial.print("LATITUDE: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("LONGITUDE: ");
    Serial.println(gps.location.lng(), 6);

    Serial.print("TIME: ");
    String formattedTime = String(gps.date.year()) + "-" +
                           (gps.date.month() < 10 ? "0" + String(gps.date.month()) : String(gps.date.month())) + "-" + 
                           (gps.date.day() < 10 ? "0" + String(gps.date.day()) : String(gps.date.day())) + "T" + 
                           (gps.time.hour() < 10 ? "0" + String(gps.time.hour()) : String(gps.time.hour())) + ":" + 
                           (gps.time.minute() < 10 ? "0" + String(gps.time.minute()) : String(gps.time.minute())) + ":" +
                           (gps.time.second() < 10 ? "0" + String(gps.time.second()) : String(gps.time.second())) + "Z";
    Serial.println(formattedTime);
  } else {
    Serial.println("No GPS data available");
  }

  // Delay để tránh spam trong Serial Monitor
  delay(1000); // 1 giây
}
