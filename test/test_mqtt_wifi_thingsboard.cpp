
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


#define UART_PMS7003_RX 17
#define UART_PMS7003_TX 16
#define SD_CS 0
#define MQ7_PIN_AO 34
#define MQ7_PIN_DO 35
#define GPS_RX 27
#define GPS_TX 14

// Thông tin WiFi
const char *ssid = "LEE THE HOUSE VT T3";
const char *password = "likeyourhome";

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

// If using the breakout, change pins as desired
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
SHT3x sht(SHT3x_I2C_ADDRESS_DEFAULT);
PMS7003 pms(UART_PMS7003_RX, UART_PMS7003_TX);
display_tft_espi disp;
MQ7 mq7(MQ7_PIN_AO);
NEO6M gps(GPS_RX, GPS_TX);

void setup()
{
    Serial.begin(9600);
    // Setup for SHT3x
    sht.init();
    sht.setMode(MODE_SHT_SINGLESHOT);
    // Setup for PMS7003
    pms.init();
    pms.setMode(MODE_NAME::PASSIVE_MODE);

    // Setup for TFT
    disp.begin();
    disp.background();

    // Setup for GPS
    gps.init();

    // Setup wifi
    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(1000);
    //     Serial.println("Connecting to WiFi...");
    // }
    // Serial.println("Connected to WiFi");

    // client.setServer(mqtt_server, mqtt_port);
    // client.setCallback(callback);
    // reconnect();
}
float pre_temp, pre_humi, pre_PM2_5, pre_PM10;
void loop()
{
    // Wifi
    // if (!client.connected())
    // {
    //     reconnect();
    // }

    // client.loop();
    // Tạo dữ liệu ngẫu nhiên cho các thông số môi trường
    float lat;// = 20.99;
    float longti;// = 105.99;
    // float co = random(0, 10) / 10.0;
    float co;
    float sound_level= random(30, 91);
    // Read temp and hum
    sht.readBytes();
    sht.decode();
    // Display temp and hum
    float temperature = sht.get(DATA_SHT3x_TARGET::TEMPERATURE_SHT);
    float humidity = sht.get(DATA_SHT3x_TARGET::HUMIDITY_SHT);
    disp.SHT(temperature, humidity);
    // Read CO gas concentration
    if (temperature == 0 || humidity == 0)
    {
        co = mq7.getConcentration();
    }
    else
    {
        co=mq7.getConcentration(temperature, humidity);
    }
    //Read GPS
    String lat_str=gps.get(LATTITUDE_GPS);
    String longti_str=gps.get(LONGITUDE_GPS);
    const char *time_gps=gps.get(TIME_GPS_NEO);
    if(lat_str == "#")
    {
        lat = 100;
    }
    else
    {
        lat = lat_str.toFloat();
    }
    
    if(longti_str == "#")
    {
        longti=100;
    }
    else
    {
        longti = longti_str.toFloat();
    }

    // Read PMS7003
    pms.readBytesBaseOnMode();
    pms.decodeData();
    // Display PMS7003
    float pm1_0 = pms.getData(DATA_TARGET::PM1_0_PMS);
    float pm2_5 = pms.getData(DATA_TARGET::PM2_5_PMS);
    float pm10 = pms.getData(DATA_TARGET::PM10_PMS);
    disp.PM(pm1_0, pm2_5, pm10);
    disp.cacborn_mono(co);
    disp.SOUND(sound_level);
    disp.GPS(lat, longti, time_gps);


    // Save data to SD card
    // SD_JS::init(SD_CS);
    // StaticJsonDocument<200> js;
    // js["temperature"] = temperature;
    // js["humidity"] = humidity;
    // js["PM1_0"] = PM1_0;
    // js["PM2_5"] = pm2_5;
    // js["PM10"] = pm10;
    // js["CO"] = co;
    // js["sound_level"] = sound_level;
    // js["latitude"] = lat;
    // js["longitude"] = longti;
    // js["time"] = millis() / 1000;

    // Serial.println(SD_JS::pushData("/Test.json", js));
    // SD.end();

    // Telemetry

    // sendTelemetry(temperature, humidity, lat, longti, pm1_0, pm2_5, pm10, co, sound_level);

    delay(3000);
}

void reconnect()
{
    // Vòng lặp để kết nối lại MQTT nếu bị mất kết nối
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("esp32Client", mqtt_user1, mqtt_pass))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
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
}