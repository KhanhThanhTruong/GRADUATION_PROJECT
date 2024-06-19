
#define TEXT "aA MWyz~12" // Text that will be printed on screen in any font

// #include "Free_Fonts.h" // Include the header file attached to this sketch

#include "SPI.h"
// #include "Ava1.c"
// #include "TFT_eSPI.h"
#include <Arduino.h>
#include <Wire.h>
#include <background.h>
#include <Adafruit_ILI9341.h>
#define TFT_DC 2
#define TFT_CS 15
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 5
#define TFT_MISO 19
// Use hardware SPI
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);


// unsigned long drawTime = 0;

void setup(void) {

  tft.begin();

  tft.setRotation(0);

}

void loop() {

  tft.drawRGBBitmap(0, 0, avatar, BACK_HEIGHT, BACK_WIDTH);

}

// There follows a crude way of flagging that this example sketch needs fonts which
// have not been enabled in the User_Setup.h file inside the TFT_HX8357 library.
//
// These lines produce errors during compile time if settings in User_Setup are not correct
//
// The error will be "does not name a type" but ignore this and read the text between ''
// it will indicate which font or feature needs to be enabled
//
// Either delete all the following lines if you do not want warnings, or change the lines
// to suit your sketch modifications.

#ifndef LOAD_GLCD
//ERROR_Please_enable_LOAD_GLCD_in_User_Setup
#endif

#ifndef LOAD_GFXFF
// ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif

