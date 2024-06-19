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
#include <PubSubClient.h>
#include <random>

// Thông tin WiFi
const char* ssid = "LEE THE HOUSE VT T3";
const char* password = "likeyourhome";

// Thông tin ThingsBoard
const char* mqtt_server = "demo.thingsboard.io";
const int mqtt_port = 1883;
const char* mqtt_user1 = "JBEmsEvd0rP1itS6WfPh"; // Token 1
// const char* mqtt_user2 = ""; // Token 2
const char* mqtt_pass = ""; // Không cần mật khẩu cho ThingsBoard
const char* mqtt_topic = "v1/devices/me/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);

bool useToken1 = true; // Biến để theo dõi token hiện tại

void reconnect();
void callback(char* topic, byte* message, unsigned int length);
void sendTelemetry(float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level);

void setup() {
  Serial.begin(9600);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Kết nối MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // Tạo dữ liệu ngẫu nhiên cho các thông số môi trường
  float temperature = random(20, 31);
  float humidity = random(50, 81);
  float lat = 20.99;
  float longti = 105.89;
  float pm1_0 = random(10, 31);
  float pm2_5 = random(20, 51);
  float pm10 = random(30, 61);
  float co = random(0, 10) / 10.0;
  float sound_level = random(30, 91);

  // Gửi dữ liệu lên ThingsBoard
  sendTelemetry(temperature, humidity, lat, longti, pm1_0, pm2_5, pm10, co, sound_level);

  // Chuyển đổi token
  useToken1 = !useToken1;

  delay(5000); // Đợi 5 giây trước khi gửi lại dữ liệu
}

void reconnect() {
  // Vòng lặp để kết nối lại MQTT nếu bị mất kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("esp32Client", mqtt_user1, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  // In ra nội dung tin nhắn
  String messageStr;
  for (int i = 0; i < length; i++) {
    messageStr += (char)message[i];
  }
  Serial.println("Message: " + messageStr);
}

void sendTelemetry(float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level) {
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
