
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
#include <SoftwareSerial.h>
#include <SIMCOM7680C.h>
#define RX_SIM_PIN 25
#define TX_SIM_PIN 26

A7680C a7680c;

void setup()
{
    Serial.begin(9600);
    a7680c.begin(115200);

    // a7680c.reset();
    // delay(15000);
    // delay(10);
    // a7680c.sendATCommand("AT+MQTTSTART");
    // a7680c.mqtt_client_idconfg("ESP8266_SIMCOM_7680C");
    if (a7680c.mqtt_connect("1125636", 60, 1, "tcp://demo.thingsboard.io", 1883, "Thanh", "123456"))
    {
        Serial.println("Connect success");
    }
    else
    {
        Serial.println("Connect failed");
    }

    a7680c.mqtt_publish("v1/devices/me/telemetry", "{\"key\":" + String(15) + "}");
    // Serial.println(a7680c.getResponse());
    // a7680c.sendATCommand("")
}
void loop()
{
}
