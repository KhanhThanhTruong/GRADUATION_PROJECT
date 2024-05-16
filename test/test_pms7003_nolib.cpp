#include <Arduino.h>

#define PMS7003_RX 2  // Arduino RX -> PMS7003 TX
#define PMS7003_TX 3  // Arduino TX -> PMS7003 RX
#define CMD_LENGTH 7
#define RESP_LENGTH 32
#define UART_BAUD 9600

// Lệnh kích hoạt chế độ Active
const uint8_t ACTIVE_CMD[] = {0x42, 0x4D, 0xE1, 0x00, 0x01, 0x01, 0x71};

bool checkChecksum(uint8_t* buffer, int length) {
    uint16_t sum = 0;
    for (int i = 0; i < length - 2; i++) {
        sum += buffer[i];
    }
    uint16_t checksum = (buffer[length - 2] << 8) | buffer[length - 1];
    return sum == checksum;
}

void sendRequest() {
    Serial.write(ACTIVE_CMD, sizeof(ACTIVE_CMD));
}

void parsePMData(uint8_t *data) {

    //Parse PM1.0, PM2.5, PM10
    uint16_t concPM1_0_ug_m3 = (data[4] << 8) | data[5];
    uint16_t concPM2_5_ug_m3 = (data[6] << 8) | data[7];
    uint16_t concPM10_ug_m3 = (data[8] << 8) | data[9];
    // Parse PM1.0, PM2.5, PM10 ATM
    uint16_t concPM1_0_ATM = (data[10] << 8) | data[11];
    uint16_t concPM2_5_ATM = (data[12] << 8) | data[13];
    uint16_t concPM10_0_ATM = (data[14] << 8) | data[15];


    Serial.print("PM1.0: ");
    Serial.print(concPM1_0_ug_m3);
    Serial.print(" µg/m³, ");

    Serial.print("PM2.5: ");
    Serial.print(concPM2_5_ug_m3);
    Serial.print(" µg/m³, ");

    Serial.print("PM10: ");
    Serial.print(concPM10_ug_m3);
    Serial.println(" µg/m³");
}

void setup() {
    Serial.begin(UART_BAUD_PMS700);
    delay(1000); // Chờ 1 giây cho module khởi động

    Serial.println("PMS7003 setup complete");

    // Kích hoạt chế độ Active
    sendRequest();
}

void loop() {
    // Đợi một chút để nhận dữ liệu
    delay(1000);

    if (Serial.available() >= RESP_LENGTH) {
        uint8_t buffer[RESP_LENGTH];
        Serial.readBytes(buffer, RESP_LENGTH);

        if (buffer[0] == 0x42 && buffer[1] == 0x4D && checkChecksum(buffer, RESP_LENGTH)) {
            parsePMData(buffer);
        } else {
            Serial.println("Invalid data or checksum error");
        }
    }

    // Delay giữa các lần đọc dữ liệu
    delay(10000);
}
