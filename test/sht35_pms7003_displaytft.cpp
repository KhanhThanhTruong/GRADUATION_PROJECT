// ILI9341 example with embedded color bitmaps in sketch.
// WILL NOT FIT ON ARDUINO UNO OR OTHER AVR BOARDS;
// uses large bitmap image stored in array!

// Options for converting images to the format used here include:
//   http://www.rinkydinkelectronics.com/t_imageconverter565.php
// or
//  GIMP (https://www.gimp.org/) as follows:
//    1. File -> Export As
//    2. In Export Image dialog, use 'C source code (*.c)' as filetype.
//    3. Press export to get the export options dialog.
//    4. Type the desired variable name into the 'prefixed name' box.
//    5. Uncheck 'GLIB types (guint8*)'
//    6. Check 'Save as RGB565 (16-bit)'
//    7. Press export to save your image.
//  Assuming 'image_name' was typed in the 'prefixed name' box of step 4,
//  you can have to include the c file, then using the image can be done with:
//    tft.drawRGBBitmap(0, 0, image_name.pixel_data, image_name.width, image_name.height);
//  See also https://forum.pjrc.com/threads/35575-Export-for-ILI9341_t3-with-GIMP

#include "SPI.h"
#include <Adafruit_ILI9341.h>
#include <display.h>
#include <SHT3x.h>
#include <PMS7003.h>

#define TFT_DC 2
#define TFT_CS 15
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 5
#define TFT_MISO 19
#define UART_PMS7003_RX 17
#define UART_PMS7003_TX 16
// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
SHT3x sht(SHT3x_I2C_ADDRESS_DEFAULT);
PMS7003 pms(UART_PMS7003_RX, UART_PMS7003_TX);
display disp(tft);
void setup()
{
  // Serial.begin(9600);
  // Setup for SHT3x
  sht.init();
  sht.setMode(MODE_SHT_SINGLESHOT);
  tft.fillScreen(ILI9341_BLACK);
  // Setup for PMS7003
  pms.init();
  pms.setMode(MODE_NAME::PASSIVE_MODE);

  // Setup for TFT
  disp.background();
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
  disp.PM(pms.getData(DATA_TARGET::PM2_5_PMS), pms.getData(DATA_TARGET::PM10_PMS));

  delay(5000);



}
