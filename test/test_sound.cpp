
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

#define UART_PMS7003_RX 26
#define UART_PMS7003_TX 25
#define SD_CS 0

#define GPS_RX 27
#define GPS_TX 14

#define SAMPLE_SHT 5000
#define SAMPLE_MQ7 5000
#define SAMPLE_GPS 5000
#define SAMPLE_SOUND 5000
#define SAMPLE_PMS7003 5000
#define SAMPLE_ISR_MQ7 500
#define SAMPLE_TELEMETRY 5000
#define PHONE_NUMBER_DEFAULT "0379745618"
#define TIME_CHUP_CALL 5000

// Thông tin WiFi
const char *ssid = "LEE THE HOUSE VT T3";

const char *password = "likeyourhome";

// const char *ssid = "KimBanh";
// const char *password = "thanh2910";

// Thông tin ThingsBoard
const char *mqtt_server = "demo.thingsboard.io";
const int mqtt_port = 1883;
const char *mqtt_user1 = "ZUrH2WD9vnUL0OKOZatg"; // Token 1
// const char* mqtt_user2 = ""; // Token 2
const char *mqtt_pass = ""; // Không cần mật khẩu cho ThingsBoard
const char *mqtt_topic = "v1/devices/me/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);

bool useToken1 = true; // Biến để theo dõi token hiện tại

void reconnect();
void callback(char *topic, byte *message, unsigned int length);
void sendTelemetry(float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level);

// Enviroment component
float co = 0, pm1_0 = 0, pm2_5 = 0, pm10 = 0;
float temperature = 20, humidity = 65;
float lat;
float longti;
float sound_level = -100;
String formattedTime = "";

unsigned long previous_millis_sht = 0;
unsigned long previous_millis_mq7 = 0;
unsigned long previous_millis_gps = 0;
unsigned long previous_millis_sound = 0;
unsigned long previous_millis_pms7003 = 0;
unsigned long previous_millis_isr_mq7 = 0;
unsigned long previous_millis_telemetry = 0;
unsigned long previous_millis_call = 0;
unsigned long previous_millis_sms = 0;

// Interrput declare
volatile bool MQ7_FLAG = false, SOUND_FLAG = false;
int number_sound_triger = 0;
SHT3x sht(SHT3x_I2C_ADDRESS_DEFAULT);
PMS7003 pms(UART_PMS7003_RX, UART_PMS7003_TX);
display_tft_espi disp;
// NEO6M gps(GPS_RX, GPS_TX);
SoftwareSerial gps(GPS_RX, GPS_TX);
hw_timer_t *timer = NULL;
A7680C a7680c;
TinyGPSPlus tool_gps;

IRAM_ATTR void isr_sound()
{
    SOUND_FLAG = true;
}

IRAM_ATTR void isr_mq7()
{
    MQ7_FLAG = true;
}
void sendTelemetry_4G(float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level);
void setup()
{
    Serial.begin(9600);
    // Setup sound Interrupt
    //  Thiết lập timer
    timer = timerBegin(0, 80, true);               // Timer 0, phân chia xung nhịp 80 (1 tick = 1us), tăng lên
    timerAttachInterrupt(timer, &isr_sound, true); // Gắn hàm xử lý ngắt
    timerAlarmWrite(timer, 1000, true);            // 1000 ticks (1ms)
    timerAlarmEnable(timer);                       // Bật ngắt timer
}
void loop()
{
    // Measure sound level
    if (SOUND_FLAG == true)
    {
        number_sound_triger++;
        SOUND_FLAG = false;

        float cur_sound_level = getSoundLevel();
        if (cur_sound_level > sound_level)
        {
            sound_level = cur_sound_level;
        }
        if (number_sound_triger >= 1000)
        {
            // disp.SOUND(sound_level);
            Serial.println(sound_level);
            number_sound_triger = 0;
            sound_level = -100;
        }
    }
}

void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);

    // In ra nội dung tin nhắn
    String messageStr;
    for (int i = 0; i < length; i++)
    {
        messageStr += (char)message[i];
    }
    Serial.println("Message: " + messageStr);
}

void sendTelemetry(float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level)
{
    char temperatureStr[8];
    char humidityStr[8];
    char latitudeStr[8];
    char longitudeStr[8];
    char pm1_0Str[8];
    char pm2_5Str[8];
    char pm10Str[8];
    char coStr[8];
    char soundLevelStr[8];

    dtostrf(temperature, 1, 2, temperatureStr);
    dtostrf(humidity, 1, 2, humidityStr);
    dtostrf(latitude, 1, 2, latitudeStr);
    dtostrf(longitude, 1, 2, longitudeStr);
    dtostrf(pm1_0, 1, 2, pm1_0Str);
    dtostrf(pm2_5, 1, 2, pm2_5Str);
    dtostrf(pm10, 1, 2, pm10Str);
    dtostrf(co, 1, 2, coStr);
    dtostrf(sound_level, 1, 2, soundLevelStr);

    String message = "{\"temperature\":";
    message += temperatureStr;
    message += ",\"humidity\":";
    message += humidityStr;
    message += ",\"latitude\":";
    message += latitudeStr;
    message += ",\"longitude\":";
    message += longitudeStr;
    message += ",\"pm1_0\":";
    message += pm1_0Str;
    message += ",\"pm2_5\":";
    message += pm2_5Str;
    message += ",\"pm10\":";
    message += pm10Str;
    message += ",\"co\":";
    message += coStr;
    message += ",\"sound_level\":";
    message += soundLevelStr;
    message += "}";

    client.publish(mqtt_topic, message.c_str());

    Serial.println("Sent telemetry data to ThingsBoard:");
    Serial.println(message);
    disp.display_notification("Sent ThingsBoard");
}
