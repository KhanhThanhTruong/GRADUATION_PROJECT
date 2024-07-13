
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
#include <uRTCLib.h>

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
#define SAMPLE_BACKUP 1000
#define PHONE_NUMBER_DEFAULT "0379745618"
#define TIME_CHUP_CALL 5000
#define GPS_TIMEOUT 1000
#define FREQUENCE_SOUND 1000

// Thông tin WiFi
// const char *ssid = "LEE THE HOUSE VT T3";
// const char *password = "likeyourhome";

const char *ssid = "KimBanh";
const char *password = "thanh2910";

// Thông tin ThingsBoard
const char *mqtt_server = "demo.thingsboard.io";
const int mqtt_port = 1883;
const char *mqtt_user1 = "ZUrH2WD9vnUL0OKOZatg"; // Token 1
// const char* mqtt_user2 = ""; // Token 2
const char *mqtt_pass = ""; // Không cần mật khẩu cho ThingsBoard
const char *mqtt_topic = "v1/devices/me/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);

// Enviroment component
float co = 0, pm1_0 = 0, pm2_5 = 0, pm10 = 0;
float temperature = 20, humidity = 65;
float lat = 0;
float longti = 0;
float sound_level = 0;
String formattedTime = "00:00:00";
bool BACKUP_FLAG = false;

unsigned long currentMillis = 0;
unsigned long previous_millis_sht = 0;
unsigned long previous_millis_mq7 = 0;
unsigned long previous_millis_gps = 0;
unsigned long previous_millis_sound = 0;
unsigned long previous_millis_pms7003 = 0;
unsigned long previous_millis_isr_mq7 = 0;
unsigned long previous_millis_telemetry = 0;
unsigned long previous_millis_call = 0;
unsigned long previous_millis_sms = 0;
unsigned long previous_millis_backup = 0;

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
uRTCLib rtc(0x68);

IRAM_ATTR void isr_sound()
{
  SOUND_FLAG = true;
}

IRAM_ATTR void isr_mq7()
{
  MQ7_FLAG = true;
}

// Declare function using in sample
String readAndRemoveLine(const char *fileName);
void from_sd_to_tft();
void from_tft_to_sd();
void backup();
void readSHT();
void readPMS();
void readmq7();
void readgps();
void readsound();
void reconnect();
void storage_data_if_not_internet(String timeStr, float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level);
void callback(char *topic, byte *message, unsigned int length);
void sendTelemetry(String time, float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level);
void setup()
{
  Serial.begin(9600);
  // Setup for 4G
  a7680c.begin(115200);
  // Setup for SHT3x
  sht.init();
  sht.setMode(MODE_SHT_SINGLESHOT);
  // Setup for PMS7003
  pms.init();
  pms.setMode(MODE_NAME::PASSIVE_MODE);

  // Setup for TFT
  disp.begin();
  disp.background();

  // Setup for sound
  pinMode(SOUNDSENSOR_PIN, INPUT);
  // Setup for GPS
  gps.begin(9600);

  // Setup mq7 interupt
  pinMode(MQ7_PIN_DO, INPUT);
  attachInterrupt(digitalPinToInterrupt(MQ7_PIN_DO), isr_mq7, FALLING);

  // Setup sound Interrupt
  //  Thiết lập timer
  timer = timerBegin(0, 80, true);               // Timer 0, phân chia xung nhịp 80 (1 tick = 1us), tăng lên
  timerAttachInterrupt(timer, &isr_sound, true); // Gắn hàm xử lý ngắt
  timerAlarmWrite(timer, FREQUENCE_SOUND, true); // 10000 ticks (10ms)
  timerAlarmEnable(timer);                       // Bật ngắt timer

  // Setup wifi
  if (disp.get_state_function_icon(FUNCTION_ICON::WIFI) == true)
  {

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    reconnect();
  }
}

void loop()
{
  //   currentMillis = millis();
  //   // Response touch
  //   from_tft_to_sd();
  //   from_sd_to_tft();
  //   disp.process_touch();
  //   // Backup data
  //   if (currentMillis - previous_millis_backup >= SAMPLE_BACKUP && disp.get_state_function_icon(FUNCTION_ICON::WIFI) == true)
  //   {
  //     backup();
  //   }
  //   // Measure sound level
  //   if (SOUND_FLAG == true)
  //   {
  //     readsound();
  //   }
  //   // Measure CO if detect
  //   if (MQ7_FLAG == true && currentMillis - previous_millis_isr_mq7 >= SAMPLE_ISR_MQ7)
  //   {
  //     readmq7();
  //   }
  //   // Measrue temp and hum
  //   if (currentMillis - previous_millis_sht >= SAMPLE_SHT)
  //   {
  //     readSHT();
  //   }
  //   // Measure CO
  //   if (currentMillis - previous_millis_mq7 >= SAMPLE_MQ7)
  //   {
  //     readmq7();
  //   }
  //   // Location
  //   if (currentMillis - previous_millis_gps >= SAMPLE_GPS)
  //   {
  //     readgps();
  //   }
  //   // Read PMS7003
  //   if (currentMillis - previous_millis_pms7003 >= SAMPLE_PMS7003)
  //   {
  //     readPMS();
  //   }

  //   // Telemetry
  //   if (currentMillis - previous_millis_telemetry >= SAMPLE_TELEMETRY)
  //   {
  //     previous_millis_telemetry = currentMillis;

  //     // Update time string
  //     rtc.refresh();

  //     // Lấy thời gian hiện tại từ DS3231
  //     int hour = rtc.hour();
  //     int minute = rtc.minute();
  //     int second = rtc.second();

  //     // Thêm số 0 nếu số nhỏ hơn 10
  //     String formattedHour = (hour < 10) ? "0" + String(hour) : String(hour);
  //     String formattedMinute = (minute < 10) ? "0" + String(minute) : String(minute);
  //     String formattedSecond = (second < 10) ? "0" + String(second) : String(second);

  //     String formattedTime = formattedHour + ":" + formattedMinute + ":" + formattedSecond;

  //     if (disp.get_state_function_icon(FUNCTION_ICON::WIFI) == false && disp.get_state_function_icon(FUNCTION_ICON::LTE) == false)
  //     {
  //       disp.display_notification("Internet disconnected");
  //       storage_data_if_not_internet(formattedTime, temperature, humidity, lat, longti, pm1_0, pm2_5, pm10, co, sound_level);
  //     }

  //     if (disp.get_state_function_icon(FUNCTION_ICON::WIFI) == true)
  //     {
  //       if (!client.connected() || WiFi.status() != WL_CONNECTED)
  //       {
  //         reconnect();
  //       }
  //       client.loop();
  //       sendTelemetry(formattedTime, temperature, humidity, lat, longti, pm1_0, pm2_5, pm10, co, sound_level);
  //     }

  //     if (disp.get_state_function_icon(FUNCTION_ICON::LTE) == true && disp.get_state_function_icon(FUNCTION_ICON::WIFI) == false)
  //     {
  //       // sendTelemetry_4G(temperature, humidity, lat, longti, pm1_0, pm2_5, pm10, co, sound_level);
  //     }
  //   }

  //   // Call number
  //   if (disp.get_state_function_icon(FUNCTION_ICON::CALL) == true && millis() - previous_millis_call >= 7000)
  //   {
  //     previous_millis_call = millis();
  //     a7680c.callNumber(String(PHONE_NUMBER_DEFAULT));
  //     disp.display_notification("Calling " + String(PHONE_NUMBER_DEFAULT));
  //   }
  //   // unsigned long now_millis = millis();
  //   if (millis() - previous_millis_call >= TIME_CHUP_CALL && disp.get_state_function_icon(FUNCTION_ICON::CALL) == true)
  //   {
  //     previous_millis_call = millis();
  //     a7680c.sendATCommand("AT+CHUP");
  //     disp.display_notification("Call ended");
  //     disp.set_state_function_icon(FUNCTION_ICON::CALL, false);
  //     disp.draw_call_icon(FIRST_ICON_X, FIRST_ICON_Y + 150, ICON_RADIUS, disp.get_state_function_icon(FUNCTION_ICON::CALL));
  //   }

  //   // Send sms
  //   if (disp.get_state_function_icon(FUNCTION_ICON::SMS) == true)
  //   {
  //     previous_millis_sms = millis();
  //     a7680c.sendSMS(String(PHONE_NUMBER_DEFAULT), "SMS WARNING");
  //     disp.display_notification("Sent SMS to " + String(PHONE_NUMBER_DEFAULT));
  //     disp.set_state_function_icon(FUNCTION_ICON::SMS, false);
  //   }

  //   if (millis() - previous_millis_sms >= 700 && previous_millis_sms != 0)
  //   {
  //     previous_millis_sms = 0;
  //     disp.draw_sms_icon(FIRST_ICON_X, FIRST_ICON_Y + 100, ICON_RADIUS, disp.get_state_function_icon(FUNCTION_ICON::SMS));
  //   }

  disp.process_touch();

  if (millis() - previous_millis_telemetry > 5000)
  {
    previous_millis_telemetry = millis();
    storage_data_if_not_internet("12:20:52", 0, 0, 0, 0, 0, 0, 0, 0, 0);
    digitalWrite(TOUCH_CS, LOW);
    digitalWrite(TFT_CS, HIGH);
    digitalWrite(SD_CS, HIGH);
  }

  // disp.display_notification("Touch");
  }

  void backup()
  {
    previous_millis_backup = millis();
    Serial.println("Backup data");
    from_tft_to_sd();
    String msg = readAndRemoveLine("/Backup.json");
    Serial.println(msg);
    if (msg != "FILE NOT EXIST" && msg != "ERROR" && msg != "FILE REMOVED")
    {
      if (!client.connected() || WiFi.status() != WL_CONNECTED)
      {
        reconnect();
      }
      client.loop();
      client.publish(mqtt_topic, msg.c_str());
      disp.display_notification("Backup data");
    }
  }

  void reconnect()
  {
    unsigned long start = millis();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED && millis() - start < 5000)
    {
      delay(1000);
      Serial.println("Connecting to WiFi...");
      disp.display_notification("Connecting to WiFi...");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("WiFi not found");
      disp.display_notification("Wifi not found");
      goto bb;
    }
    Serial.println("Connected to WiFi");
    disp.display_notification("Connected to WiFi");

    start = millis();
    while (!client.connected() && millis() - start < 200000)
    {
      Serial.print("Attempting MQTT connection...");
      disp.display_notification("Reconnect...");
      if (client.connect("esp32Client", mqtt_user1, mqtt_pass))
      {
        Serial.println("connected");
        disp.display_notification("MQTT connected");
        return;
      }
      else
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        disp.display_notification("MQTT failed, retry in 5s");
        delay(5000);
      }
    }
  bb:
    disp.display_notification("WIFI error, change to LTE");
    disp.set_state_function_icon(FUNCTION_ICON::WIFI, false);
    disp.draw_wifi_icon(FIRST_ICON_X, FIRST_ICON_Y, ICON_RADIUS, false);



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

  void sendTelemetry(String timeStr, float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level)
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
    message += ",\"time\":";
    message += "\"";
    message += timeStr;
    message += "\"";
    message += "}";

    client.publish(mqtt_topic, message.c_str());

    Serial.println("Sent telemetry data to ThingsBoard:");
    Serial.println(message);
    disp.display_notification("Sent ThingsBoard");
  }

  void readgps()
  {
    previous_millis_gps = currentMillis;
    unsigned long start = millis();
    while (millis() - start < GPS_TIMEOUT)
    {
      while (gps.available() > 0)
      {
        if (tool_gps.encode(gps.read()) && tool_gps.location.isUpdated())
        {
          {
            disp.display_notification("GPS found");
            lat = float(tool_gps.location.lat());
            longti = float(tool_gps.location.lng());
            formattedTime =
                // String(tool_gps.date.year()) + "-" +
                //    (tool_gps.date.month() < 10 ? "0" + String(tool_gps.date.month()) : String(tool_gps.date.month())) + "-" +
                //    (tool_gps.date.day() < 10 ? "0" + String(tool_gps.date.day()) : String(tool_gps.date.day())) + "T" +
                ((tool_gps.time.hour() + 7) < 10 ? "0" + String(tool_gps.time.hour()) : String(tool_gps.time.hour())) + ":" +
                (tool_gps.time.minute() < 10 ? "0" + String(tool_gps.time.minute()) : String(tool_gps.time.minute())) + ":" +
                (tool_gps.time.second() < 10 ? "0" + String(tool_gps.time.second()) : String(tool_gps.time.second()));
            // break;
            from_tft_to_sd ();
            from_sd_to_tft();
            disp.GPS(lat, longti, formattedTime.c_str());
          }
        }
      }

      // disp.GPS(20.9983, 105.8479, "13:59:15");
    }
    if (tool_gps.charsProcessed() < 10)
    {
      disp.display_notification("GPS not found");
    }
  }

  void from_tft_to_sd()
  {
    // Kết thúc giao tiếp với màn hình TFT
    disp.end();

    digitalWrite(TFT_CS, HIGH);   // Bỏ chọn màn hình TFT
    digitalWrite(TOUCH_CS, HIGH); // Bỏ chọn cảm ứng (nếu có)
    digitalWrite(SD_CS, LOW);     // Chọn thẻ SD

    if (!SD.begin(SD_CS, (disp.get_tft())->getSPIinstance()))
    {
      Serial.println("Failed to begin SD");
    }


}

void from_sd_to_tft()
{
  SD.end();
  digitalWrite(TFT_CS, LOW);   // Chọn màn hình TFT
  digitalWrite(TOUCH_CS, LOW); // Chọn cảm ứng (nếu có)
  digitalWrite(SD_CS, HIGH);   // bo chọn thẻ SD
  disp.init();
}

void storage_data_if_not_internet(String timeStr, float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level)
{
  from_tft_to_sd();

  StaticJsonDocument<200> js;
  js["temperature"] = temperature;
  js["humidity"] = humidity;
  js["PM1_0"] = pm1_0;
  js["PM2_5"] = pm2_5;
  js["PM10"] = pm10;
  js["CO"] = co;
  js["sound_level"] = sound_level;
  js["latitude"] = lat;
  js["longitude"] = longti;
  js["time"] = timeStr.c_str();

  if (SD_JS::pushData("/Backup.json", js) == "PUSH JSON SUCCESS")
  {
    Serial.println("Storaged data");
    from_sd_to_tft();
    BACKUP_FLAG = true;
    disp.display_notification("Storaged data");
  }
  else
  {
    Serial.println("Storage failed");
    from_sd_to_tft();
    disp.display_notification("Storaged data failed");
  }
}

void readsound()
{
  SOUND_FLAG = false;

  float cur_sound_level = getSoundLevel();
  if (cur_sound_level != 0)
  {
    // Serial.println("SOund");
    number_sound_triger++;

    if (cur_sound_level > sound_level)
    {
      sound_level = cur_sound_level;
    }
  }

  if (number_sound_triger >= 1000000 / FREQUENCE_SOUND)
  {
    Serial.println(sound_level);
    // sound_level = sound_level / 1000;
    sound_level = sound_level * 142.68 - 294.85;
    Serial.println(sound_level);
    if (sound_level < 0.5)
    {
      sound_level = 0.61;
    }
    Serial.println(sound_level);
    sound_level = 20 * log10(sound_level / V_OFFSET) + 94 - GAIN_MAX4466_DB - SENSITIVITY_MICROPHONE_DB;
    Serial.println(sound_level);
    from_tft_to_sd();
    from_sd_to_tft();
    disp.SOUND(sound_level);
    // Serial.println("Sound detect");
    number_sound_triger = 0;
    sound_level = 0;
  }
}

void readmq7()
{
  MQ7_FLAG = false;
  // Đo ngay lập tức khi có ngắt
  if (temperature == 0 || humidity == 0)
  {
    co = getConcentration();
  }
  else
  {
    co = getConcentration(temperature, humidity);
  }
  from_tft_to_sd();
  from_sd_to_tft();
  disp.cacborn_mono(co);
  previous_millis_isr_mq7 = currentMillis;
}

void readSHT()
{
  // Read temp and hum
  sht.readBytes();
  sht.decode();
  // Display temp and hum
  temperature = sht.get(DATA_SHT3x_TARGET::TEMPERATURE_SHT);
  humidity = sht.get(DATA_SHT3x_TARGET::HUMIDITY_SHT);
  from_tft_to_sd();
  from_sd_to_tft();
  disp.SHT(temperature, humidity);
  previous_millis_sht = currentMillis;
}

void readPMS()
{
  pms.readBytesBaseOnMode();
  pms.decodeData();
  // Display PMS7003
  pm1_0 = pms.getData(DATA_TARGET::PM1_0_PMS);
  pm2_5 = pms.getData(DATA_TARGET::PM2_5_PMS);
  pm10 = pms.getData(DATA_TARGET::PM10_PMS);
  from_tft_to_sd();
  from_sd_to_tft();
  disp.PM(pm1_0, pm2_5, pm10);
  previous_millis_pms7003 = currentMillis;
}

String readAndRemoveLine(const char *fileName)
{

  from_tft_to_sd();
  // Kiểm tra xem file có tồn tại không
  if (!SD.exists(fileName))
  {
    Serial.println("File does not exist");
    from_sd_to_tft();
    BACKUP_FLAG = false;
    return "FILE NOT EXIST";
  }

  File file = SD.open(fileName, FILE_READ);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    from_sd_to_tft();
    return "ERROR";
  }

  // Kiểm tra xem file có rỗng hay không
  if (file.size() == 0)
  {
    Serial.println("File is empty");
    file.close();
    from_sd_to_tft();
    return "ERROR";
  }

  String firstLine = "";
  if (file.available())
  {
    firstLine = file.readStringUntil('\n');
    Serial.print("Read line: ");
    Serial.println(firstLine);
  }
  else
  {
    file.close();
    from_sd_to_tft();
    return "ERROR";
  }
  file.close();

  // Đọc toàn bộ nội dung trừ dòng đầu tiên
  file = SD.open(fileName, FILE_READ);
  String remainingContent = "";
  bool firstLineSkipped = false;
  while (file.available())
  {
    String line = file.readStringUntil('\n');
    if (firstLineSkipped)
    {
      remainingContent += line + "\n";
    }
    else
    {
      firstLineSkipped = true;
    }
  }
  file.close();

  // Ghi lại nội dung đã đọc trừ dòng đầu tiên
  file = SD.open(fileName, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    from_sd_to_tft();
    return "ERROR";
  }
  file.print(remainingContent);
  file.close();

  // Kiểm tra nếu tệp tin rỗng và xóa nó
  if (remainingContent.length() == 0)
  {
    SD.remove(fileName);
    Serial.println("File is empty, deleted the file.");
    from_sd_to_tft();
    BACKUP_FLAG = false;
    return "FILE REMOVED";
  }
  else
  {
    Serial.println("Removed the first line from the file.");
  }
  from_sd_to_tft();
  return firstLine;
}