
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

#include <MQ7.h>

volatile bool interruptTriggered = false;
unsigned long previousMillis = 0, previous_isr=0;
const long interval = 5000;  // 5 giây
#define SAMPLE_RE_ISR_MQ7 500


IRAM_ATTR void isr_mq7()
{
    interruptTriggered = true; // Đặt cờ báo hiệu rằng ngắt đã xảy ra
}

void readMQ7Sensor()
{
    float _concentration = getConcentration();

    Serial.print("CO Concentration: ");
    Serial.println(_concentration);
}

void setup()
{
    Serial.begin(9600);
    pinMode(MQ7_PIN_DO, INPUT);
    attachInterrupt(digitalPinToInterrupt(MQ7_PIN_DO), isr_mq7, FALLING);
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        // Đo nồng độ khí CO mỗi 5 giây
        previousMillis = currentMillis;
        readMQ7Sensor();
    }

    if (interruptTriggered && currentMillis - previous_isr >= SAMPLE_RE_ISR_MQ7) {
        // Đo ngay lập tức khi có ngắt
        interruptTriggered = false; // Đặt lại cờ
        readMQ7Sensor();
        previous_isr = currentMillis;
    }

    // Các mã khác có thể đặt ở đây
}
