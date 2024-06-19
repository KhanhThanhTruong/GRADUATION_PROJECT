#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include "SPIFFS.h" // ESP32 only
#include <Arduino.h>
// Định nghĩa chân kết nối nút nhấn và LED
#define BUTTON_PIN 35
#define LED_PIN 2

// Biến lưu trạng thái của LED
// Định nghĩa chân kết nối nút nhấn và LED
#define BUTTON_PIN 35
#define LED_PIN 2

// Biến lưu trạng thái của LED
volatile bool ledState = false;

// Hàm xử lý ngắt
void IRAM_ATTR handleInterrupt() {
  // Đảo trạng thái LED
  ledState = !ledState;
  // Ghi trạng thái mới cho LED
  digitalWrite(LED_PIN, ledState);
  delay(1000);
}

void setup() {
  // Khởi tạo chân LED là OUTPUT
  pinMode(LED_PIN, OUTPUT);
  // Khởi tạo chân nút nhấn là INPUT
  pinMode(BUTTON_PIN, INPUT);
  // Gắn hàm xử lý ngắt cho chân nút nhấn
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, RISING);
}

void loop() {
  // Không làm gì trong loop
}

