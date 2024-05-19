#include <Arduino.h>
#include <Wire.h>

TwoWire *wire = &Wire;
uint8_t rx_bytes[6]; // Mảng toàn cục để lưu trữ dữ liệu đọc được từ cảm biến

uint8_t* measure() {
  Wire.beginTransmission(0x44);
  Wire.write(0xE0);
  Wire.write(0x00);
  if (Wire.endTransmission(false) != 0) {
    Serial.println("Failed to communicate with SHT3x");
    return nullptr;
  }

  if (Wire.requestFrom(0x44, (uint8_t)6) != 6) { 
    Serial.println("Failed to read from SHT3x");
    return nullptr;
  }

  memset(rx_bytes, 0, 6);
  Wire.readBytes(rx_bytes, 6);
  return rx_bytes; // Trả về mảng dữ liệu toàn cục
}



bool begin() {
  Wire.beginTransmission(0x44);
  Wire.write(0x27); // mps => 10
  Wire.write(0x37); // Repeatability => High
  return Wire.endTransmission() == 0;
}

void setup() {
  Serial.begin(9600);

  Wire.begin();
  while (!begin()) {
    Serial.println("SHT3x not found !");
    delay(1000);
  }
}

void loop() {
  uint8_t *data;
  data = measure();
  if (data == nullptr) {
    delay(2000);
    return;
  }
  for (int i = 0; i < 6; i++) { // In từng byte dữ liệu ra dưới dạng giá trị hex
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println(); // In xuống dòng mới sau khi in tất cả các byte dữ liệu
  delay(3000);      // Đợi 3 giây trước khi thực hiện lần đọc tiếp theo
}
