#pragma once

#define ENABLE_GxEPD2_display 0
#define SCREEN_WIDTH 250
#define SCREEN_HEIGHT 122
#define SCREEN_IS_BW // Black White color

// Connections for ESP32
static const uint8_t EPD_BUSY = 3; // to EPD BUSY 24 is labled as 'TX'
static const uint8_t EPD_CS = 5;   // to EPD CS
static const uint8_t EPD_RST = 2;  // to EPD RST
static const uint8_t EPD_DC = 1;   // to EPD DC
static const uint8_t EPD_SCK = 4;   // to EPD CLK 23 is labled as 'RX'
static const uint8_t EPD_MISO = -1; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 6;  // to EPD DIN / SDA

void initialiseDisplay();
void drawString(int x, int y, String text);
void fullRefresh(int x, int y, String text);
void partialRefresh(int16_t x, int16_t y, int16_t w, int16_t h, int size, int yOffset, String text);
void drawString(int x, int y, String text);
