
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
const char* mqtt_user = "JBEmsEvd0rP1itS6WfPh";
const char* mqtt_pass = ""; // Không cần mật khẩu cho ThingsBoard
const char* mqtt_topic = "v1/devices/me/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect();
void callback(char* topic, byte* message, unsigned int length);
void sendTelemetry(float temperature, float humidity, float latitude, float longitude) ;

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

  // Tạo dữ liệu nhiệt độ và độ ẩm ngẫu nhiên
  float temperature = random(20, 31);
  float humidity = random(50, 81);
  float lat=20.99;
  float longti=105.89;
  // Gửi dữ liệu lên ThingsBoard
  sendTelemetry(temperature, humidity, lat, longti);

  delay(5000); // Đợi 5 giây trước khi gửi lại dữ liệu
}

void reconnect() {
  // Vòng lặp để kết nối lại MQTT nếu bị mất kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("esp32Client", mqtt_user, mqtt_pass)) {
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

void sendTelemetry(float temperature, float humidity, float latitude, float longitude) {
  char temperatureStr[8];
  char humidityStr[8];
  char latitudeStr[8];
  char longitudeStr[8];
  dtostrf(temperature, 1, 2, temperatureStr);
  dtostrf(humidity, 1, 2, humidityStr);
  dtostrf(latitude, 1, 2, latitudeStr);
  dtostrf(longitude, 1, 2, longitudeStr);
  String message = "{\"temperature\":";
  message += temperatureStr;
  message += ",\"humidity\":";
  message += humidityStr;
  message += ",\"latitude\":";
  message += latitudeStr;
  message += ",\"longtitude\":";
  message += longitudeStr;
  message += "}";

  client.publish(mqtt_topic, message.c_str());
  Serial.println("Sent telemetry data to ThingsBoard:");
  Serial.println(message);
}