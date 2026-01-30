#pragma once

#include <SPI.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h> // https://github.com/olikraus/U8g2_for_Adafruit_GFX

#define ENABLE_GxEPD2_display 0
#define SCREEN_WIDTH 296
#define SCREEN_HEIGHT 128
#define SCREEN_IS_RBW // Red Black White color

// Connections for ESP32
static const uint8_t EPD_BUSY = 3; // to EPD BUSY 24 is labled as 'TX'
static const uint8_t EPD_CS = 5;   // to EPD CS
static const uint8_t EPD_RST = 2;  // to EPD RST
static const uint8_t EPD_DC = 1;   // to EPD DC
static const uint8_t EPD_SCK = 4;   // to EPD CLK 23 is labled as 'RX'
static const uint8_t EPD_MISO = -1; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 6;  // to EPD DIN / SDA

extern GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display;
extern U8G2_FOR_ADAFRUIT_GFX u8g2Fonts; // Select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

void InitialiseDisplay();
void clearScreen();
void drawString(int x, int y, String text);
