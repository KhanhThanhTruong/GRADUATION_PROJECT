#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <Wire.h>
#include <SD.h>
/*
  The TFT_eSPI library incorporates an Adafruit_GFX compatible
  button handling class, this sketch is based on the Arduin-o-phone
  example.

  This example displays a keypad where numbers can be entered and
  sent to the Serial Monitor window.

  The sketch has been tested on the ESP8266 (which supports SPIFFS)

  The minimum screen size is 320 x 240 as that is the keypad size.
*/

// The SPIFFS (FLASH filing system) is used to hold touch screen
// calibration data

#include "FS.h"

#include <SPI.h>


#include "SPI.h"
#include <Adafruit_ILI9341.h>
#include <display.h>
#include <SHT3x.h>
#include <PMS7003.h>
#include <SD_mybuit.h>

#define TFT_DC 2
#define TFT_CS 15
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 4
#define TFT_MISO 19
#define UART_PMS7003_RX 17
#define UART_PMS7003_TX 16
#define SD_CS 0
// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
SHT3x sht(SHT3x_I2C_ADDRESS_DEFAULT);
PMS7003 pms(UART_PMS7003_RX, UART_PMS7003_TX);
display disp(tft);
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
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  disp.background();

  //Setup for SD
}
float pre_temp, pre_humi, pre_PM2_5, pre_PM10;
void loop()
{
  // Read temp and hum
  sht.readBytes();
  sht.decode();
  // Display temp and hum
  disp.SHT(sht.get(DATA_SHT3x_TARGET::TEMPERATURE_SHT), sht.get(DATA_SHT3x_TARGET::HUMIDITY_SHT));
  // Read PMS7003
  pms.readBytesBaseOnMode();
  pms.decodeData();
  // Display PMS7003
  // tft.begin();
  // tft.fillScreen(ILI9341_BLACK);
  // disp.background();
  disp.PM(pms.getData(DATA_TARGET::PM2_5_PMS), pms.getData(DATA_TARGET::PM10_PMS));
  // Save data to SD card
  SD_JS::init(SD_CS);
  StaticJsonDocument<200> js;
  js["temperature"]=sht.get(DATA_SHT3x_TARGET::TEMPERATURE_SHT);
  js["humidity"]=sht.get(DATA_SHT3x_TARGET::HUMIDITY_SHT);
  js["PM2_5"]=pms.getData(DATA_TARGET::PM2_5_PMS);
  js["PM10"]=pms.getData(DATA_TARGET::PM10_PMS);
  js["time"]=millis()/1000;

  Serial.println(SD_JS::pushData("/Test.json",js));
  SD.end();
  delay(5000);

}
