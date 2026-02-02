#include <SPI.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h> // https://github.com/olikraus/U8g2_for_Adafruit_GFX

#include "epaper.h"

// Display type
GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(
    GxEPD2_213_Z98c(/*CS=*/ EPD_CS, /*DC=*/ EPD_DC, /*RST=*/ EPD_RST, /*BUSY=*/ EPD_BUSY)
);

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts; // Select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

void initialiseDisplay() {
  display.init(115200, true, 2, false);
  SPI.end();
  SPI.begin(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS);
  display.setRotation(1);                     // Use 1 or 3 for landscape modes
  u8g2Fonts.begin(display);                   // connect u8g2 procedures to Adafruit GFX
  u8g2Fonts.setFontMode(1);                   // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);              // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_inb30_mf);    // Explore u8g2 fonts from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
}

void partialRefresh(int16_t x, int16_t y, int16_t w, int16_t h, int size, int yOffset, String text) {
    display.setPartialWindow(x, y, w, h);
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);  // Rensa området
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        const uint8_t* fontname; 

        switch(size) {
            case 1:
                fontname = u8g2_font_helvR14_tf;
                break;
            case 2:
                fontname = u8g2_font_helvR18_tf;
                break;
            case 3:
                fontname = u8g2_font_helvR24_tf;
                break;
            case 4:
                fontname = u8g2_font_logisoso32_tf; 
                break;
            case 5:
                fontname = u8g2_font_inb42_mn;     
                break;
            default:
                fontname = u8g2_font_helvR14_tf;
                break;
        }
        u8g2Fonts.setFont(fontname);
        u8g2Fonts.setCursor(x, y + yOffset);
        u8g2Fonts.print(text);
    } while (display.nextPage());
    Serial.println("Partial refresh done.");
}
void fullRefresh(int x, int y, String text){
      // 'Wash' display once an hour to remove 'ghosting' leftovers etc.
      display.setFullWindow(); 
      display.firstPage();
      do {
        display.fillScreen(GxEPD_WHITE);
        u8g2Fonts.setCursor(x, y);
        u8g2Fonts.print(text);
      } while (display.nextPage());
      Serial.println("Full refresh done.");
}
void drawString(int x, int y, String text)
{
    int16_t x1, y1; // the bounds of x,y and w and h of the variable 'text' in pixels.
    uint16_t w, h;
    display.setTextWrap(false);
    display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
    u8g2Fonts.setFont(u8g2_font_helvR14_tf); 
    u8g2Fonts.setCursor(x, y + h);
    u8g2Fonts.print(text);
    display.display(false); 
    display.hibernate();
}