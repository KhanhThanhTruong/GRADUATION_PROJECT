
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

String readAndRemoveLine(const char *fileName);
void backup_data();
void storage_data_if_not_internet();
void sendTelemetry_4G(float temperature, float humidity, float latitude, float longitude, float pm1_0, float pm2_5, float pm10, float co, float sound_level);
void setup()
{

  // Setup SPI
  pinMode(SD_CS, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  pinMode(TOUCH_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TOUCH_CS, HIGH);
  //
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

  // Setup for SD

  // Setup for GPS
  gps.begin(9600);

  // Setup mq7 interupt
  pinMode(MQ7_PIN_DO, INPUT);
  attachInterrupt(digitalPinToInterrupt(MQ7_PIN_DO), isr_mq7, FALLING);

  // Setup sound Interrupt
  //  Thiết lập timer
  timer = timerBegin(0, 80, true);               // Timer 0, phân chia xung nhịp 80 (1 tick = 1us), tăng lên
  timerAttachInterrupt(timer, &isr_sound, true); // Gắn hàm xử lý ngắt
  timerAlarmWrite(timer, 1000, true);            // 1000 ticks (1ms)
  timerAlarmEnable(timer);                       // Bật ngắt timer

  // readAndRemoveLine("/Backup.json");
  delay(5000);
}

void loop()
{

  // storage_data_if_not_internet();
  String msg=readAndRemoveLine("/Backup.json");
  Serial.println(msg);
  delay(5000);

}

void from_tft_to_sd()
{
  // Kết thúc giao tiếp với màn hình TFT
  disp.end();

  digitalWrite(TFT_CS, HIGH);   // Bỏ chọn màn hình TFT
  digitalWrite(TOUCH_CS, HIGH); // Bỏ chọn cảm ứng (nếu có)
  digitalWrite(SD_CS, LOW);    // Chọn thẻ SD

  if (!SD.begin(SD_CS, (disp.get_tft())->getSPIinstance()))
  {
    Serial.println("Failed to begin SD");
  }

}

void from_sd_to_tft()
{
  SD.end();
  disp.init();
}

void storage_data_if_not_internet()
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
  js["time"] = formattedTime.c_str();

  if (SD_JS::pushData("/Backup.json", js) == "PUSH JSON SUCCESS")
  {
    Serial.println("Storaged data");
    from_sd_to_tft();
    disp.display_notification("Storaged data");
  }
  else
  {
    Serial.println("Storage failed");
    from_sd_to_tft();
    disp.display_notification("Storaged data failed");
  }
}


String readAndRemoveLine(const char *fileName)
{

  from_tft_to_sd();
  // Kiểm tra xem file có tồn tại không
  if (!SD.exists(fileName))
  {
    Serial.println("File does not exist");
    from_sd_to_tft();
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
    return "FILE REMOVED";
  }
  else
  {
    Serial.println("Removed the first line from the file.");
  }
  from_sd_to_tft();
  return firstLine;
}